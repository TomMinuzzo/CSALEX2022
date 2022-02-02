/**
 * Command interface for CANopenSocket - ASCII helper functions.
 *
 * @file        CO_comm_helpers.h
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

#ifndef CO_COMM_HELPERS_H
#define CO_COMM_HELPERS_H

#include <CO_driver.h> /* for some typedefs */

#include "CO_SDO.h" /* for CO_memcpySwap */

/* String with delimiters for blank characters, used in strtok. */
extern const char spaceDelim[];

/** Response errors as specified by CiA 309-3. */
typedef enum {
    respErrorNone = 0,
    respErrorReqNotSupported = 100,
    respErrorSyntax = 101,
    respErrorInternalState = 102,
    respErrorNoDefaultNodeSet = 105,
    respErrorUnsupportedNet = 106,
    respErrorUnsupportedNode = 107
} respErrorCode_t;

/** @brief Data types structure - Defined in CANOpen Code */
typedef struct {
    char *syntax;
    int length;
    int (*dataTypePrint)(char *strout, int stroutSize, char *bufSdo, int bufLen);
    int (*dataTypeScan)(char *bufSdo, int bufSdoSize, char *strin);
} dataType_t;

/**
 * Data type print function.
 *
 * Functions writes binary data from bufSdo to strout (with sprintf).
 * dtpHex outputs in hex format (two hex digits, space separated).
 * There are other dtp functions inside dataTypes[] array.
 *
 * @param strout Output string.
 * @param bufSdo Input binary data.
 * @param bufLen Size of input binary data.
 *
 * @return Number of bytes written to strout.
 */
int dtpHex(char *strout, int stroutSize, char *bufSdo, int bufLen);

/**
 * Data type scan function.
 *
 * Functions scans input string string and writes binary data to bufSdo.
 * dtsHex scans from hex format (two hex digits, space separated).
 * There are other dts functions inside dataTypes[] array.
 *
 * @param bufSdo Output binary data.
 * @param strin Input string.
 *
 * @return Number of bytes written to bufSdo, 0 on failure.
 */
int dtsHex(char *bufSdo, int bufSdoSize, char *strin);

/**
 * Get token from currently parsing string.
 *
 * Function calls thread safe strtok_r function and returns token.
 * getTok itself may be used from one thread only.
 *
 * @param initStr Same as first argument to strtok_r.
 * @param delim Same as second argument to strtok_r.
 * @param err Input and returning parameter sets to true in case of error.
 *            If err is already true, function returns NULL immediately.
 *
 * @return Next token or NULL.
 */
char *getTok(char *initStr, const char *delim, int *err);

/**
 * Verify, if there are no more tokens.
 *
 * Function calls getTok and if there is another token and that token doesn't
 * start with '#', it sets err to 1. (Comments are ignored.)
 *
 * @param initStr Same as in getTok.
 * @param delim Same as in getTok.
 * @param err Same as in getTok.
 */
void lastTok(char *initStr, const char *delim, int *err);

/**
 * Get number from token.
 *
 * Token must contain no other characters and number must be inside limits.
 *
 * @param token Input string.
 * @param min Minimum value for the output number.
 * @param max Maximum value for the output number.
 * @param err Input and returning parameter sets to true in case of error.
 *            If err is already true, function returns 0 immediately.
 *
 * @return Number.
 */
uint32_t getU32(char *token, uint32_t min, uint32_t max, int *err);
int32_t getI32(char *token, int32_t min, int32_t max, int *err);
uint64_t getU64(char *token, uint64_t min, uint64_t max, int *err);
int64_t getI64(char *token, int64_t min, int64_t max, int *err);
float32_t getR32(char *token, int *err);
float64_t getR64(char *token, int *err);

/**
 * Get data type from token.
 *
 * Token must exactly match one of the 'syntax' from dataTypes[].
 *
 * @param token Input string.
 * @param err Input and returning parameter sets to true in case of error.
 *            If err is already true, function returns 0 immediately.
 *
 * @return Number.
 */
const dataType_t *getDataType(char *token, int *err);

#endif
