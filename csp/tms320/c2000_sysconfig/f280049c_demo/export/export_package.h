/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef INCLUDE_EXPORTPACKAGE_H_
#define INCLUDE_EXPORTPACKAGE_H_

#include <stdbool.h>
#include <stdint.h>
#include <export/export.h>
#include <transfer_utils.h>




// Generic Function Prototypes

extern int EXPORTPACKAGE_sendKeyValuePointerPairs(ExportKeyValue** keyValues, uint16_t numKeyValues);
extern int EXPORTPACKAGE_sendKeyValuePairs(ExportKeyValue* keyValues, uint16_t numKeyValues);
extern int EXPORTPACKAGE_sendKeyValuePair(ExportKeyValue keyValue);

extern int EXPORTPACKAGE_startSendObject(char * key);
extern int EXPORTPACKAGE_endSendObject();
extern int EXPORTPACKAGE_appendKeyValuePairWithEndingChar(ExportKeyValue keyValue, bool isStr, char end);

extern int EXPORTPACKAGE_sendString(char *cmd, uint16_t cmdLength, char *value, uint16_t valueLength);

extern int EXPORTPACKAGE_sendUInt16(char *cmd, uint16_t cmdLength, uint16_t val);
extern int EXPORTPACKAGE_sendInt16(char *cmd, uint16_t cmdLength, int16_t val);
extern int EXPORTPACKAGE_sendUInt32(char *cmd, uint16_t cmdLength, uint32_t val);
extern int EXPORTPACKAGE_sendInt32(char *cmd, uint16_t cmdLength, int32_t val);
extern int EXPORTPACKAGE_sendUInt64(char *cmd, uint16_t cmdLength, uint64_t val);
extern int EXPORTPACKAGE_sendInt64(char *cmd, uint16_t cmdLength, int64_t val);

extern int EXPORTPACKAGE_sendCharacterArrayAsByteArray(char *cmd, uint16_t cmdLength, char* data, uint32_t size);
extern int EXPORTPACKAGE_sendUInt32ArrayAsByteArray(char *cmd, uint16_t cmdLength, uint32_t* data, uint32_t size);
extern int EXPORTPACKAGE_sendInt32ArrayAsByteArray(char *cmd, uint16_t cmdLength, int32_t* data, uint32_t size);
extern int EXPORTPACKAGE_sendUInt16ArrayAsByteArray(char *cmd, uint16_t cmdLength, uint16_t* data, uint32_t size);
extern int EXPORTPACKAGE_sendInt16ArrayAsByteArray(char *cmd, uint16_t cmdLength, int16_t* data, uint32_t size);
extern int EXPORTPACKAGE_sendFloatArrayAsByteArray(char *cmd, uint16_t cmdLength, float* data, uint32_t size);

extern int EXPORTPACKAGE_sendCharacterAsByteArray(char *cmd, uint16_t cmdLength, char val);
extern int EXPORTPACKAGE_sendUInt32AsByteArray(char *cmd, uint16_t cmdLength, uint32_t val);
extern int EXPORTPACKAGE_sendInt32AsByteArray(char *cmd, uint16_t cmdLength, int32_t val);
extern int EXPORTPACKAGE_sendUInt16AsByteArray(char *cmd, uint16_t cmdLength, uint16_t val);
extern int EXPORTPACKAGE_sendInt16AsByteArray(char *cmd, uint16_t cmdLength, int16_t val);
extern int EXPORTPACKAGE_sendFloatAsByteArray(char *cmd, uint16_t cmdLength, float val);
extern int EXPORTPACKAGE_sendBoolAsByteArray(char *cmd, uint16_t cmdLength, bool val);

extern int EXPORTPACKAGE_sendBool(char *cmd, uint16_t cmdLength, bool val);


#endif /* INCLUDE_EXPORTPACKAGE_H_ */
