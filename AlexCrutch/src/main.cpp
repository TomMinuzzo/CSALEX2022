/*
 * CANopen main program file for Linux SocketCAN.
 *
 * @file        main
 * @author      Janez Paternoster
 * @copyright   2015 Janez Paternoster
 *
 * This file is part of CANopenSocket, a Linux implementation of CANopen
 * stack with master functionality. Project home page is
 * <https://github.com/CANopenNode/CANopenSocket>. CANopenSocket is based
 * on CANopenNode: <https://github.com/CANopenNode/CANopenNode>.
 *
 * CANopenSocket is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CANopen.h"
#include "CO_OD_storage.h"
#include "CO_Linux_tasks.h"
#include "CO_time.h"
#include "application.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>
#include <net/if.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include "CO_command.h"
#include <pthread.h>
#include <sys/time.h>

#include <iostream>

/*For master-> code SDO direct messaging*/
// #define CO_COMMAND_SDO_BUFFER_SIZE 100000
// #define STRING_BUFFER_SIZE (CO_COMMAND_SDO_BUFFER_SIZE * 4 + 100)

#define NSEC_PER_SEC (1000000000)      /* The number of nanoseconds per second. */
#define NSEC_PER_MSEC (1000000)        /* The number of nanoseconds per millisecond. */
#define TMR_TASK_INTERVAL_NS (1000000) /* Interval of taskTmr in nanoseconds */
#define TMR_TASK_OVERFLOW_US (5000)    /* Overflow detect limit for taskTmr in microseconds */
#define INCREMENT_1MS(var) (var++)     /* Increment 1ms variable in taskTmr */
#define NODEID (101)
#define CANMESSAGELENGTH (100)

/* Global variable increments each millisecond. */
volatile uint32_t CO_timer1ms = 0U;

/* Mutex is locked, when CAN is not valid (configuration state). May be used
 *  from other threads. RT threads may use CO->CANmodule[0]->CANnormal instead. */
pthread_mutex_t CO_CAN_VALID_mtx = PTHREAD_MUTEX_INITIALIZER;

/* Other variables and objects */
static int rtPriority = 2; /* Real time priority, configurable by arguments. (-1=RT disabled) */
static int rtControlPriority = 80;
static int mainline_epoll_fd;                        /* epoll file descriptor for mainline */
static CO_OD_storage_t odStor;                       /* Object Dictionary storage object for CO_OD_ROM */
static CO_OD_storage_t odStorAuto;                   /* Object Dictionary storage object for CO_OD_EEPROM */
static char *odStorFile_rom = "od4_storage";         /* Name of the file */
static char *odStorFile_eeprom = "od4_storage_auto"; /* Name of the file */
static CO_time_t CO_time;                            /* Object for current time */
int commCount = 0;
bool readyToStart = false;

uint32_t tmr1msPrev = 0;

// RT Tast timer
struct period_info
{
    struct timespec next_period;
    long period_ns;
};
// Forward declartion of timer functions
static void inc_period(struct period_info *pinfo);
static void periodic_task_init(struct period_info *pinfo);
static void wait_rest_of_period(struct period_info *pinfo);

// Forward declaration of helper functions
void configureCANopen(int nodeId, int rtPriority, int CANdevice0Index, char *CANdevice);


/* Realtime thread */
static void *rt_thread(void *arg);
static pthread_t rt_thread_id;
static int rt_thread_epoll_fd;
/* Realtime control thread */
static void *rt_control_thread(void *arg);
static pthread_t rt_control_thread_id;
static int rt_control_thread_epoll_fd;
/* Signal handler */
volatile sig_atomic_t CO_endProgram = 0;
static void sigHandler(int sig)
{
    CO_endProgram = 1;
}


/* Helper functions ***********************************************************/
void CO_errExit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/* send CANopen generic emergency message */
void CO_error(const uint32_t info)
{
    CO_errorReport(CO->em, CO_EM_GENERIC_SOFTWARE_ERROR, CO_EMC_SOFTWARE_INTERNAL, info);
    fprintf(stderr, "canopend generic error: 0x%X\n", info);
}

/******************************************************************************/
/** Mainline and RT thread                                                   **/
/******************************************************************************/
int main(int argc, char *argv[])
{
    CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
    CO_ReturnError_t odStorStatus_rom, odStorStatus_eeprom;
    int opt;
    bool_t firstRun = true;
    bool_t nodeIdFromArgs = true; /* True, if program arguments are used for CANopen Node Id */
    bool_t rebootEnable = false;  /* Configurable by arguments */
    bool_t commandEnable = false; /* Configurable by arguments */

     int nodeId = NODEID;                                           /*!< CAN Network NODEID */

    int can_dev_number=6;
    char CANdeviceList[can_dev_number][10] = {"vcan0\0", "can0\0", "can1\0", "can2\0", "can3\0", "can4\0"};    /*!< linux CAN device interface for app to bind to: change to can1 for bbb, can0 for BBAI vcan0 for virtual can*/
    char CANdevice[10]="";
    int CANdevice0Index;
    //Rotate through list of interfaces and select first one existing and up
    for(unsigned i=0; i<can_dev_number; i++) {
        printf("%s: ", CANdeviceList[i]);
        //Check if interface exists
        CANdevice0Index = if_nametoindex(CANdeviceList[i]);/*map linux CAN interface to corresponding int index return zero if no interface exists.*/
        if(CANdevice0Index!=0) {
            char operstate_filename[255], operstate_s[25];
            snprintf(operstate_filename, 254, "/sys/class/net/%s/operstate", CANdeviceList[i]);
            //Check if it's up
            FILE* operstate_f = fopen(operstate_filename, "r");
            fscanf(operstate_f, "%s", &operstate_s);
            printf("%s\n", operstate_s);
            //Check if not "down" as will be "unknown" if up
            if(strcmp(operstate_s, "down")!=0) {
                snprintf(CANdevice, 9, "%s", CANdeviceList[i]);
                printf("Using: %s (%d)\n", CANdeviceList[i], CANdevice0Index);
                break;
            }
            else {
                CANdevice0Index=0;
            }
        }
        else {
            printf("-\n");
        }

    }
    configureCANopen(nodeId, rtPriority, CANdevice0Index, CANdevice);

    /* Catch signals SIGINT and SIGTERM */
    if (signal(SIGINT, sigHandler) == SIG_ERR)
        CO_errExit("Program init - SIGINIT handler creation failed");
    if (signal(SIGTERM, sigHandler) == SIG_ERR)
        CO_errExit("Program init - SIGTERM handler creation failed");

    /* increase variable each startup. Variable is automatically stored in non-volatile memory. */
    printf(", count=%u ...\n", ++OD_powerOnCounter);

    while (reset != CO_RESET_APP && reset != CO_RESET_QUIT && CO_endProgram == 0)
    {
        /* CANopen communication reset - initialize CANopen objects *******************/
        CO_ReturnError_t err;

        printf("Canopend- communication reset ...\n");

        /* Wait other threads (command interface). */
        pthread_mutex_lock(&CO_CAN_VALID_mtx);

        /* Wait rt_thread. */
        if (!firstRun)
        {
            CO_LOCK_OD();
            CO->CANmodule[0]->CANnormal = false;
            CO_UNLOCK_OD();
        }

        /* Enter CAN configuration. */
        CO_CANsetConfigurationMode(CANdevice0Index);

        /* initialize CANopen */
        if (!nodeIdFromArgs)
        {
            /* use value from Object dictionary, if not set by program arguments */
            nodeId = OD_CANNodeID;
        }
        err = CO_init(CANdevice0Index, nodeId, 0);
        if (err != CO_ERROR_NO)
        {
            char s[120];
            snprintf(s, 120, "Communication reset - CANopen initialization failed, err=%d", err);
            CO_errExit(s);
        }

        /* Configure callback functions for task control */
        CO_EM_initCallback(CO->em, taskMain_cbSignal);
        CO_SDO_initCallback(CO->SDO[0], taskMain_cbSignal);
        CO_SDOclient_initCallback(CO->SDOclient, taskMain_cbSignal);

        /* Initialize time */
        CO_time_init(&CO_time, CO->SDO[0], &OD_time.epochTimeBaseMs, &OD_time.epochTimeOffsetMs, 0x2130);

        /* First time only initialization. */
        if (firstRun)
        {
            firstRun = false;

            /* Configure epoll for mainline */
            mainline_epoll_fd = epoll_create(4);
            if (mainline_epoll_fd == -1)
                CO_errExit("Program init - epoll_create mainline failed");

            /* Init mainline */
            taskMain_init(mainline_epoll_fd, &OD_performance[ODA_performance_mainCycleMaxTime]);

            /* Configure epoll for rt_thread */
            rt_thread_epoll_fd = epoll_create(2);
            if (rt_thread_epoll_fd == -1)
                CO_errExit("Program init - epoll_create rt_thread failed");

            /* Init taskRT */
            CANrx_taskTmr_init(rt_thread_epoll_fd, TMR_TASK_INTERVAL_NS, &OD_performance[ODA_performance_timerCycleMaxTime]);

            OD_performance[ODA_performance_timerCycleTime] = TMR_TASK_INTERVAL_NS / 1000; /* informative */

            /* Initialize socket command interface */
            if (commandEnable)
            {
                if (CO_command_init() != 0)
                {
                    CO_errExit("Socket command interface initialization failed");
                }
                printf("Canopend - Command interface on socket '%s' started ...\n", CO_command_socketPath);
            }
            /*OLD THREAD CREATION*/
            /* Create rt_thread */
            if (pthread_create(&rt_thread_id, NULL, rt_thread, NULL) != 0)
                CO_errExit("Program init - rt_thread creation failed");
            /* Set priority for rt_thread */
            if (rtPriority > 0)
            {
                struct sched_param param;

                param.sched_priority = rtPriority;
                if (pthread_setschedparam(rt_thread_id, SCHED_FIFO, &param) != 0)
                    CO_errExit("Program init - rt_thread set scheduler failed");
            }
            /* Create rt_control_thread */
            if (pthread_create(&rt_control_thread_id, NULL, rt_control_thread, NULL) != 0)
                CO_errExit("Program init - rt_thread_control creation failed");
            /* Set priority for rt_thread */
            if (rtPriority > 0)
            {
                struct sched_param paramc;

                paramc.sched_priority = rtControlPriority;
                if (pthread_setschedparam(rt_thread_id, SCHED_FIFO, &paramc) != 0)
                    CO_errExit("Program init - rt_thread set scheduler failed");
            }
        }

        /* start CAN */
        CO_CANsetNormalMode(CO->CANmodule[0]);
        pthread_mutex_unlock(&CO_CAN_VALID_mtx);

        /* Execute optional additional application code */

        // app_programStart();

        reset = CO_RESET_NOT;
        // Create Statemachine Object -> will be loaded by taskmanager in end program.

        /* Execute optional additional application code */
        app_communicationReset();

        // Initialise the last time variable
        //gettimeofday(&last_tv,NULL);
        //struct timeval first_tv = last_tv;

        printf("Canopend- running ...\n");
        readyToStart = true;

        while (reset == CO_RESET_NOT && CO_endProgram == 0)
        {
            /* loop for normal program execution ******************************************/
            int ready;
            int first = 0;
            struct epoll_event ev;

            ready = epoll_wait(mainline_epoll_fd, &ev, 1, -1);

            if (ready != 1)
            {
                if (errno != EINTR)
                {
                    CO_error(0x11100000L + errno);
                }
            }

            else if (taskMain_process(ev.data.fd, &reset, CO_timer1ms))
            {
                uint32_t timer1msDiff;
                timer1msDiff = CO_timer1ms - tmr1msPrev;
                tmr1msPrev = CO_timer1ms;
                app_programAsync(timer1msDiff);
                CO_OD_storage_autoSave(&odStorAuto, CO_timer1ms, 60000);

                /* Execute optional additional application code */
            }

            else
            {
                /* No file descriptor was processed. */
                CO_error(0x11200000L);
                /* CHANGE TO FILE!*/
            }
        }
    }

    /* program exit ***************************************************************/
    /* join threads */
    if (commandEnable)
    {
        if (CO_command_clear() != 0)
        {
            CO_errExit("Socket command interface removal failed");
        }
    }

    CO_endProgram = 1;
    if (pthread_join(rt_thread_id, NULL) != 0)
    {
        CO_errExit("Program end - pthread_join failed");
    }

    /* Execute optional additional application code */
    app_programEnd();

    /* Store CO_OD_EEPROM */
    CO_OD_storage_autoSave(&odStorAuto, 0, 0);
    CO_OD_storage_autoSaveClose(&odStorAuto);

    /* delete objects from memory */
    CANrx_taskTmr_close();
    taskMain_close();
    CO_delete(CANdevice0Index);

    printf("Canopend on %s (nodeId=0x%02X) - finished.\n\n", CANdevice, nodeId);
    /* Flush all buffers (and reboot) */
    if (rebootEnable && reset == CO_RESET_APP)
    {
        sync();
        if (reboot(LINUX_REBOOT_CMD_RESTART) != 0)
        {
            CO_errExit("Program end - reboot failed");
        }
    }

    exit(EXIT_SUCCESS);
}

/* Realtime thread for CAN receive and taskTmr ********************************/
static void *rt_thread(void *arg)
{
    /* Endless loop */
    while (CO_endProgram == 0)
    {
        int ready;
        struct epoll_event ev;

        ready = epoll_wait(rt_thread_epoll_fd, &ev, 1, -1);

        if (ready != 1)
        {
            if (errno != EINTR)
            {
                CO_error(0x12100000L + errno);
            }
        }

        else if (CANrx_taskTmr_process(ev.data.fd))
        {

            /* code was processed in the above function. Additional code process below */
            INCREMENT_1MS(CO_timer1ms);
            /* Monitor variables with trace objects */
            CO_time_process(&CO_time);
#if CO_NO_TRACE > 0
            for (i = 0; i < OD_traceEnable && i < CO_NO_TRACE; i++)
            {
                CO_trace_process(CO->trace[i], *CO_time.epochTimeOffsetMs);
            }
#endif

            /* Execute optional additional application code */
            // app_program1ms();

            /* Detect timer large overflow */
            // if (OD_performance[ODA_performance_timerCycleMaxTime] > TMR_TASK_OVERFLOW_US && rtPriority > 0 && CO->CANmodule[0]->CANnormal)
            // {
            //     CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0x22400000L | OD_performance[ODA_performance_timerCycleMaxTime]);
            //     printf("Timer large overflow \n");
            // }
        }

        else
        {
            /* No file descriptor was processed. */
            CO_error(0x12200000L);
        }
    }

    return NULL;
}
/*Control thread*/
static void *rt_control_thread(void *arg)
{
    struct period_info pinfo;
    periodic_task_init(&pinfo);
    app_programStart();
    while (!readyToStart)
    {
        wait_rest_of_period(&pinfo);
    }
    while (CO_endProgram == 0)
    {
        app_program1ms();
        wait_rest_of_period(&pinfo);
    }
    return NULL;
}
// RT Tast timer
// Make sure RT control loop runs slow enough for bit flip messages to be sent and changed in each drive.

static void inc_period(struct period_info *pinfo)
{
    pinfo->next_period.tv_nsec += pinfo->period_ns;

    while (pinfo->next_period.tv_nsec >= 1000000000)
    {
        /* timespec nsec overflow */
        pinfo->next_period.tv_sec++;
        pinfo->next_period.tv_nsec -= 1000000000;
    }
}
static void periodic_task_init(struct period_info *pinfo)
{
    /* for simplicity, hardcoding a 1ms period */
    pinfo->period_ns = 1000000;

    clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}
static void wait_rest_of_period(struct period_info *pinfo)
{
    inc_period(pinfo);

    /* for simplicity, ignoring possibilities of signal wakes */
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}

void configureCANopen(int nodeId, int rtPriority, int CANdevice0Index, char *CANdevice) {
    if (nodeId < 1 || nodeId > 127) {
        fprintf(stderr, "NODE ID outside range (%d)\n", nodeId);
        exit(EXIT_FAILURE);
    }
    // rt Thread priority sanity check
    if (rtPriority != -1 && (rtPriority < sched_get_priority_min(SCHED_FIFO) || rtPriority > sched_get_priority_max(SCHED_FIFO))) {
        fprintf(stderr, "Wrong RT priority (%d)\n", rtPriority);
        exit(EXIT_FAILURE);
    }

    if (CANdevice0Index == 0) {
        char s[120];
        snprintf(s, 120, "Can't find CAN device \"%s\"", CANdevice);
        CO_errExit(s);
    }

    /* Verify, if OD structures have proper alignment of initial values */
    if (CO_OD_RAM.FirstWord != CO_OD_RAM.LastWord) {
        fprintf(stderr, "Program init - Canopend- Error in CO_OD_RAM.\n");
        exit(EXIT_FAILURE);
    }
};