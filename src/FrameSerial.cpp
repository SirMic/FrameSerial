#include "FrameSerial.h"



FrameSerial::FrameSerial(SoftwareSerial *serial)
{
    _serial = serial; 
    _isHwSerial = false;
    _head = 0;
    _tail = 0;
    _receivedFrameBytes = 0;
    _receivingBuffer[1] = 0;
    _rxState = RX_WAITING_FOR_START_BYTE;
}

FrameSerial::FrameSerial(HardwareSerial *serial)
{
    _serial = serial; 
    _isHwSerial = true;
    _head = 0;
    _tail = 0;
    _receivedFrameBytes = 0;
    _receivingBuffer[1] = 0;
    _rxState = RX_WAITING_FOR_START_BYTE;
}

FrameSerial::~FrameSerial()
{
}


void FrameSerial::begin(long baudrate) 
{
    if(_isHwSerial)
    {
        static_cast<HardwareSerial*>(_serial)->begin(baudrate);
    }
    else
    {
        static_cast<SoftwareSerial*>(_serial)->begin(baudrate);
    }

}

uint8_t FrameSerial::available() 
{
    processIncomingData();
    return framesInBuffer();
}

int FrameSerial::sendFrame(const uint8_t *frame, int len) 
{
    uint8_t buffer[40] = {0};
    buffer[0] = FRAME_START;
    buffer[1] = len + 4;
    memcpy(&buffer[2], frame, len);
    buffer[len+2] = calculateChecksum(buffer);
    buffer[len+3] = FRAME_STOP;
    for(int i = 0; i < buffer[1]; i++) {
        if(_isHwSerial)
        {
            static_cast<HardwareSerial*>(_serial)->write(buffer[i]);
        }
        else
        {
            static_cast<SoftwareSerial*>(_serial)->write(buffer[i]);
        }
    }
    return buffer[1];
}

int FrameSerial::getFrameFromBuffer(uint8_t *buffer) 
{
    int readBytesCount = -1;
    if(framesInBuffer() > 0)
    {
        readBytesCount = _lengthOfFramesInBuffer[_tail];
        memcpy(buffer, &_frames[_tail], readBytesCount);
        _tail++;
        if (_tail >= FRAME_BUFFER_SIZE) _tail = 0;
    }
    return readBytesCount;
}


void FrameSerial::processIncomingData() {
    int _bytesInBuffer = _serial->available();
    if(_bytesInBuffer > 0) 
    {
        for(int i = 0; i < _bytesInBuffer; i++) 
        {
            uint8_t c = _serial->read();
            if(_rxState == RX_WAITING_FOR_START_BYTE && c == FRAME_START) 
            {
                _rxState = RX_READING_FRAME;
                _receivingBuffer[_receivedFrameBytes] = c;
                _receivedFrameBytes++;
                continue;
            }
            else if(_rxState == RX_READING_FRAME) 
            {
                _receivingBuffer[_receivedFrameBytes] = c;
                _receivedFrameBytes++;
            }
            else 
            {
                continue;
            }
            if((_receivingBuffer[1]) > MAX_FRAME_LENGTH) 
            {
                _receivingBuffer[1] = 0;
                _receivedFrameBytes = 0;
                _rxState = RX_WAITING_FOR_START_BYTE;
            }
            else if(_receivingBuffer[1] == _receivedFrameBytes) 
            {
                if(_receivingBuffer[0] == FRAME_START 
                    && _receivingBuffer[_receivedFrameBytes - 1] == FRAME_STOP
                    && _receivingBuffer[_receivedFrameBytes - 2] == calculateChecksum(_receivingBuffer))
                {
                    memcpy(&_frames[_head], &_receivingBuffer[2], _receivingBuffer[1]-4);
                    _lengthOfFramesInBuffer[_head] = _receivingBuffer[1]-4;
                    _head++;
                    if(_head >= FRAME_BUFFER_SIZE) _head = 0;
                }
                _receivingBuffer[1] = 0;
                _receivedFrameBytes = 0;
                _rxState = RX_WAITING_FOR_START_BYTE;
            }
        }
    }
}

int FrameSerial::framesInBuffer() 
{
    int size = 0;
    if(_head >= _tail)
    {
        size = _head - _tail;
    }
    else 
    {
        size = FRAME_BUFFER_SIZE - _tail + _head;
    }
    return size;
}

uint8_t FrameSerial::calculateChecksum(const uint8_t * const buffer)
{
    uint8_t checkSum = 0;
    for(int i = 1; i < buffer[1] - 2; i++) {
        checkSum ^= buffer[i]; 
    }
    return checkSum;
}



