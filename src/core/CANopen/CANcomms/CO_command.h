/**
 * Command interface for CANopenSocket.
 *
 * @file        CO_command.h
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

#ifndef CO_COMMAND_H
#define CO_COMMAND_H
#ifndef _UINT32_T
#define _UINT32_T
typedef unsigned int uint32_t;
#endif /* _UINT32_T */
#ifndef _UINT16_T
#define _UINT16_T
typedef unsigned short uint16_t;
#endif /* _UINT16_T */
#ifndef _UINT8_T
#define _UINT8_T
typedef unsigned char uint8_t;
#endif /* _UINT8_T */

#ifdef __cplusplus
extern "C" {
#endif
/* Functions from external */
void CO_errorR(const uint32_t info);

/**
 * Path for local type  socket. External process can access command interface
 * over this socket. By default its path is "/tmp/CO_command_socket".
 */
extern char *CO_command_socketPath;

/**
 * Initialize thread and create socket for command interface.
 *
 * Make sure, that global variable CO was properly initialized before this call.
 *
 * @return 0 on success.
 */
int CO_command_init(void);

/**
 * Terminate thread and remove socket.
 *
 * @return 0 on success.
 */
int CO_command_clear(void);
/**
 * Allow main thread to send SDO messages to nodes
 *
 * @return message recieved on success 
 * @return error on failure
 */
void cancomm_socketFree(char *command, char **ret);
/**
 * Using cancomm_socketFree(const char* command, char* ret)
 * initialize nodes for PDO messaging
 *
 * @return 0 on success
 * @return error on failure
 */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

/** @} */
#endif
