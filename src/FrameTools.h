#ifndef FRAME_TOOLS_H
#define FRAME_TOOLS_H

#include <Arduino.h>

class FrameTools {
    public:
        static int floatToBytes(float *value, uint8_t *buffer);
        static int intToBytes(int *value, uint8_t *buffer);
        static int longToBytes(long *value, uint8_t *buffer);

        static int bytesToFloat(float *value, uint8_t *buffer);
        static int bytesToInt(int *value, uint8_t *buffer);
        static int bytesToLong(long *value, uint8_t *buffer);
};


#endif FRAME_TOOLS_H