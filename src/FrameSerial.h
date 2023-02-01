#ifndef FRAME_SERIAL_H
#define FRAME_SERIAL_H

#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "FrameTools.h"

#define FRAME_BUFFER_SIZE 20
#define MAX_FRAME_LENGTH 20
#define FRAME_START 0xE8
#define FRAME_STOP 0xFE

typedef enum {
    TYPE_CHAR,
    TYPE_UINT8,
    TYPE_INT,
    TYPE_LONG,
    TYPE_UNSIGNED_LONG,
    TYPE_FLOAT,
    TYPE_CHAR_ARRAY,
    TYPE_UINT8_ARRAY,
    TYPE_INT_ARRAY,
    TYPE_LONG_ARRAY,
    TYPE_UNSIGNED_LONG_ARRAY,
    TYPE_FLOAT_ARRAY,
} FrameType;


typedef enum {
    RX_WAITING_FOR_START_BYTE,
    RX_READING_FRAME,
} RXState;

class FrameSerial
{
public:
    FrameSerial(SoftwareSerial *serial);
    FrameSerial(HardwareSerial *serial);
    ~FrameSerial();
    void begin(long baudrate);
    uint8_t available();
    int getFrameFromBuffer(uint8_t *buffer);
    int sendFrame(const uint8_t *frame, int len);
 
private:
    Stream *_serial;
    uint8_t _frames[FRAME_BUFFER_SIZE][MAX_FRAME_LENGTH];
    uint8_t _receivingBuffer[MAX_FRAME_LENGTH*2];
    uint8_t _lengthOfFramesInBuffer[FRAME_BUFFER_SIZE];
    uint8_t _receivedFrameBytes;
    uint8_t _head;
    uint8_t _tail;
    RXState _rxState;
    bool _isHwSerial;

    void processIncomingData();
    uint8_t calculateChecksum(const uint8_t * const buffer);
    int framesInBuffer();
};




#endif FRAME_SERIAL_H

