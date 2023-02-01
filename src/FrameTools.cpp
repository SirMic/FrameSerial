#include "FrameTools.h"




int FrameTools::floatToBytes(float *value, uint8_t *buffer)
{
    if(buffer == NULL || value == NULL) return -1;
    memcpy(buffer, value, sizeof(float));
    return sizeof(float);
}

int FrameTools::bytesToFloat(float *value, uint8_t *buffer)
{
    if(buffer == NULL || value == NULL) return -1;
    memcpy(value, buffer, sizeof(float));
    return sizeof(float);
}

int FrameTools::intToBytes(int *value, uint8_t *buffer)
{
    if(buffer == NULL || value == NULL) return -1;
    memcpy(buffer, value, sizeof(int));
    return sizeof(int);
}

int FrameTools::bytesToInt(int *value, uint8_t *buffer)
{
    if(buffer == NULL || value == NULL) return -1;
    memcpy(value, buffer, sizeof(int));
    return sizeof(int);
}

int FrameTools::longToBytes(long *value, uint8_t *buffer)
{
    if(buffer == NULL || value == NULL) return -1;
    memcpy(buffer, value, sizeof(long));
    return sizeof(long);
}

int FrameTools::bytesToLong(long *value, uint8_t *buffer)
{
    if(buffer == NULL || value == NULL) return -1;
    memcpy(value, buffer, sizeof(long));
    return sizeof(long);
}