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
 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <export/export.h>
#include <export/export_package.h>
static const char cmdTemplate[] = "{\"\":}\n";

static char cATXString[20];

//
// These values are used to concatenate the command and data to the command template
// when using the EXPORTPACKAGE_send functions
//
#define CMD_INDEX 1
#define DATA_INDEX 3
#define CMD_TEMPLATE_LENGTH 6



int EXPORTPACKAGE_sendKeyValuePointerPairs(ExportKeyValue** keyValues, uint16_t numKeyValues)
{
    uint16_t keyValueI = 0;
    EXPORT_transmitCharBlocking('{');
    for (keyValueI = 0; keyValueI < numKeyValues; keyValueI++)
    {
        EXPORT_transmitCharBlocking('"');
        EXPORT_transmitStringLengthBlocking(keyValues[keyValueI]->key, keyValues[keyValueI]->lengthKey);
        EXPORT_transmitStringLengthBlocking("\":\"", strlen("\":\""));
        EXPORT_transmitStringLengthBlocking(keyValues[keyValueI]->value, keyValues[keyValueI]->lengthValue);
        EXPORT_transmitCharBlocking('"');

        if (keyValueI != (numKeyValues - 1))
        {
            EXPORT_transmitCharBlocking(',');
        }
    }
    EXPORT_transmitStringLengthBlocking("}\n", strlen("}\n"));
    return 0;
}

int EXPORTPACKAGE_sendKeyValuePairs(ExportKeyValue* keyValues, uint16_t numKeyValues)
{
    uint16_t keyValueI = 0;
    EXPORT_transmitCharBlocking('{');
    for (keyValueI = 0; keyValueI < numKeyValues; keyValueI++)
    {
        EXPORT_transmitCharBlocking('"');
        EXPORT_transmitStringLengthBlocking(keyValues[keyValueI].key, keyValues[keyValueI].lengthKey);
        EXPORT_transmitStringLengthBlocking("\":\"", strlen("\":\""));
        EXPORT_transmitStringLengthBlocking(keyValues[keyValueI].value, keyValues[keyValueI].lengthValue);
        EXPORT_transmitCharBlocking('"');

        if (keyValueI != (numKeyValues - 1))
        {
            EXPORT_transmitCharBlocking(',');
        }
    }
    EXPORT_transmitStringLengthBlocking("}\n", strlen("}\n"));
    return 0;
}

//
// To send an object with objects
// 1. Call EXPORTPACKAGE_startSendObject with the key for the top level object
// 2. Call EXPORTPACKAGE_appendKeyValuePairWithEndingChar(kv, ',') for as many sub object key/value pairs
// 2. On the last key/value EXPORTPACKAGE_appendKeyValuePairWithEndingChar(kv, '') for as many sub object key/value pairs
// 3. Call EXPORTPACKAGE_sendSendObject
//
int EXPORTPACKAGE_startSendObject(char * key)
{
    EXPORT_transmitStringLengthBlocking("{\"", strlen("{\""));
    EXPORT_transmitStringLengthBlocking(key, strlen(key));
    EXPORT_transmitStringLengthBlocking("\":{", strlen("\":{"));
    return 0;
}
int EXPORTPACKAGE_endSendObject()
{
    EXPORT_transmitStringLengthBlocking("}}\n", strlen("}}\n"));
    return 0;
}

int EXPORTPACKAGE_appendKeyValuePairWithEndingChar(ExportKeyValue keyValue, bool isStr, char end)
{
    EXPORT_transmitCharBlocking('"');
    EXPORT_transmitStringLengthBlocking(keyValue.key, keyValue.lengthKey);
    EXPORT_transmitStringLengthBlocking("\":", strlen("\":"));
    if (isStr)
    {
        EXPORT_transmitCharBlocking('"');
    }
    EXPORT_transmitStringLengthBlocking(keyValue.value, keyValue.lengthValue);
    if (isStr)
    {
        EXPORT_transmitCharBlocking('"');
    }
    
    if (end) {
        EXPORT_transmitCharBlocking(end);
    }

    return 0;
}

int EXPORTPACKAGE_sendKeyValuePair(ExportKeyValue keyValue)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(keyValue.key, keyValue.lengthKey);
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('"');
            EXPORT_transmitStringLengthBlocking(keyValue.value, keyValue.lengthValue);
            EXPORT_transmitCharBlocking('"');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendString(char *cmd, uint16_t cmdLength, char *value, uint16_t valueLength)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('"');
            EXPORT_transmitStringLengthBlocking(value, strlen(value));
            EXPORT_transmitCharBlocking('"');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendInt16(char *cmd, uint16_t cmdLength, int16_t val)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            EXPORT_transmitStringLengthBlocking(cATXString, strlen(cATXString));
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendUInt16(char *cmd, uint16_t cmdLength, uint16_t val)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            utoa(val, cATXString);
            EXPORT_transmitStringLengthBlocking(cATXString, strlen(cATXString));
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendCharacterAsByteArray(char *cmd, uint16_t cmdLength, char val)
{
    //
    // Work in progress
    //
    return 0;
}

int EXPORTPACKAGE_sendUInt32AsByteArray(char *cmd, uint16_t cmdLength, uint32_t val)
{
    uint16_t bytes[UINT32_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];
    u32toBytes(val, bytes);
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            for (byteIndex = 0; byteIndex < UINT32_BYTE_COUNT; byteIndex++){
                utoa(bytes[byteIndex], valStr);
                EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                if (byteIndex < (UINT32_BYTE_COUNT - 1)){
                    EXPORT_transmitCharBlocking(',');
                }
            }
            EXPORT_transmitCharBlocking(']');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendInt32AsByteArray(char *cmd, uint16_t cmdLength, int32_t val)
{
    uint16_t bytes[INT32_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];

    i32toBytes(val, bytes);
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            for (byteIndex = 0; byteIndex < INT32_BYTE_COUNT; byteIndex++){
                utoa(bytes[byteIndex], valStr);
                EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                if (byteIndex < (INT32_BYTE_COUNT - 1)){
                    EXPORT_transmitCharBlocking(',');
                }
            }
            EXPORT_transmitCharBlocking(']');
        }
    }

    return 0;
}

int EXPORTPACKAGE_sendUInt16AsByteArray(char *cmd, uint16_t cmdLength, uint16_t val)
{
    uint16_t bytes[UINT16_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];

    u16toBytes(val, bytes);
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            for (byteIndex = 0; byteIndex < UINT16_BYTE_COUNT; byteIndex++){
                utoa(bytes[byteIndex], valStr);
                EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                if (byteIndex < (UINT16_BYTE_COUNT - 1)){
                    EXPORT_transmitCharBlocking(',');
                }
            }
            EXPORT_transmitCharBlocking(']');
        }
    }

    return 0;
}

int EXPORTPACKAGE_sendInt16AsByteArray(char *cmd, uint16_t cmdLength, int16_t val)
{
    uint16_t bytes[INT16_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;

    uint16_t i;
    char valStr[50];

    i16toBytes(val, bytes);
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            for (byteIndex = 0; byteIndex < INT16_BYTE_COUNT; byteIndex++){
                utoa(bytes[byteIndex], valStr);
                EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                if (byteIndex < (INT16_BYTE_COUNT - 1)){
                    EXPORT_transmitCharBlocking(',');
                }
            }
            EXPORT_transmitCharBlocking(']');
        }
    }

    return 0;
}

int EXPORTPACKAGE_sendFloatAsByteArray(char *cmd, uint16_t cmdLength, float val)
{
    uint16_t bytes[FLOAT32_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];

    f32toBytes(val, bytes);
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            for (byteIndex = 0; byteIndex < FLOAT32_BYTE_COUNT; byteIndex++){
                utoa(bytes[byteIndex], valStr);
                EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                if (byteIndex < (FLOAT32_BYTE_COUNT - 1)){
                    EXPORT_transmitCharBlocking(',');
                }
            }
            EXPORT_transmitCharBlocking(']');
        }
    }

    return 0;
}

int EXPORTPACKAGE_sendBoolAsByteArray(char *cmd, uint16_t cmdLength, bool val)
{
    //
    // Work in progress
    //
    return 0;
}

int EXPORTPACKAGE_sendCharacterArrayAsByteArray(char *cmd, uint16_t cmdLength, char* data, uint32_t size)
{
    //
    // Work in progress
    //
    return 0;
}

int EXPORTPACKAGE_sendUInt32ArrayAsByteArray(char *cmd, uint16_t cmdLength, uint32_t* data, uint32_t size)
{
    uint32_t datai;
    uint16_t bytes[UINT32_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            
            for (datai = 0; datai < size; datai++) {
                u32toBytes(data[datai], bytes);
                for (byteIndex = 0; byteIndex < UINT32_BYTE_COUNT; byteIndex++){
                    utoa(bytes[byteIndex], valStr);
                    EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                    if ((datai < (size - 1)) || (byteIndex < (UINT32_BYTE_COUNT - 1))){
                        EXPORT_transmitCharBlocking(',');
                    }
                }
            }

            EXPORT_transmitCharBlocking(']');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendInt32ArrayAsByteArray(char *cmd, uint16_t cmdLength, int32_t* data, uint32_t size)
{
    uint32_t datai;
    uint16_t bytes[INT32_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            
            for (datai = 0; datai < size; datai++) {
                i32toBytes(data[datai], bytes);
                for (byteIndex = 0; byteIndex < INT32_BYTE_COUNT; byteIndex++){
                    utoa(bytes[byteIndex], valStr);
                    EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                    if ((datai < (size - 1)) || (byteIndex < (INT32_BYTE_COUNT - 1))){
                        EXPORT_transmitCharBlocking(',');
                    }
                }
            }

            EXPORT_transmitCharBlocking(']');
        }
    }
    return 0;
}


int EXPORTPACKAGE_sendUInt16ArrayAsByteArray(char *cmd, uint16_t cmdLength, uint16_t* data, uint32_t size)
{
    uint32_t datai;
    uint16_t bytes[UINT16_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;

    uint16_t i;
    char valStr[50];
    
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            
            for (datai = 0; datai < size; datai++) {
                u16toBytes(data[datai], bytes);
                for (byteIndex = 0; byteIndex < UINT16_BYTE_COUNT; byteIndex++){
                    utoa(bytes[byteIndex], valStr);
                    EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                    if ((datai < (size - 1)) || (byteIndex < (UINT16_BYTE_COUNT - 1))){
                        EXPORT_transmitCharBlocking(',');
                    }
                }
            }

            EXPORT_transmitCharBlocking(']');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendInt16ArrayAsByteArray(char *cmd, uint16_t cmdLength, int16_t* data, uint32_t size)
{
    uint32_t datai;
    uint16_t bytes[INT16_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    uint16_t i;
    char valStr[50];
    
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            
            for (datai = 0; datai < size; datai++) {
                i16toBytes(data[datai], bytes);
                for (byteIndex = 0; byteIndex < INT16_BYTE_COUNT; byteIndex++){
                    utoa(bytes[byteIndex], valStr);
                    EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                    if ((datai < (size - 1)) || (byteIndex < (INT16_BYTE_COUNT - 1))){
                        EXPORT_transmitCharBlocking(',');
                    }
                }
            }

            EXPORT_transmitCharBlocking(']');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendFloatArrayAsByteArray(char *cmd, uint16_t cmdLength, float* data, uint32_t size)
{
    uint32_t datai;
    uint16_t bytes[FLOAT32_BYTE_COUNT] = {0};
    uint16_t byteIndex = 0;
    char valStr[50];
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            EXPORT_transmitCharBlocking('[');
            
            for (datai = 0; datai < size; datai++) {
                f32toBytes(data[datai], bytes);
                for (byteIndex = 0; byteIndex < FLOAT32_BYTE_COUNT; byteIndex++){
                    utoa(bytes[byteIndex], valStr);
                    EXPORT_transmitStringLengthBlocking(valStr, strlen(valStr));
                    if ((datai < (size - 1)) || (byteIndex < (FLOAT32_BYTE_COUNT - 1))){
                        EXPORT_transmitCharBlocking(',');
                    }
                }
            }

            EXPORT_transmitCharBlocking(']');
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendBoolArrayAsByteArray(char *cmd, uint16_t cmdLength, bool* data, uint32_t size)
{
    //
    // Work in progress
    //  
    return 0;
}

int EXPORTPACKAGE_sendUInt32(char *cmd, uint16_t cmdLength, uint32_t val)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            utoa(val, cATXString);
            EXPORT_transmitStringLengthBlocking(cATXString, strlen(cATXString));
        }
    }
    return 0;
}

int EXPORTPACKAGE_sendInt32(char *cmd, uint16_t cmdLength, int32_t val)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            EXPORT_transmitStringLengthBlocking(cATXString, strlen(cATXString));
        }
    }

    return 0;
}

int EXPORTPACKAGE_sendBool(char *cmd, uint16_t cmdLength, bool val)
{
    uint16_t i;
    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        EXPORT_transmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            EXPORT_transmitStringLengthBlocking(cmd, strlen(cmd));
        } else if (i == DATA_INDEX) {
            if (val == true) {
                EXPORT_transmitStringLengthBlocking("true", strlen("true"));
            } else {
                EXPORT_transmitStringLengthBlocking("false", strlen("false"));
            }
        }
    }
    return 0;
}

