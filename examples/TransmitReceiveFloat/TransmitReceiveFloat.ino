#include <FrameSerial.h>


/* 
 *  Create FrameSerial instance
 *  and pass reference to Serial object 
*/
FrameSerial frameSerial(&Serial);


/*
 * Declaration of transmit and receive buffers
*/
uint8_t frameReceive[20];
uint8_t frameTransmit[20];

/*
 * Declaration of float variable to send
*/
float simTemp = 0.0;
/*
 * Declaration of variable for non-blocking delay
*/
long sendingTime = 0;

//initialization
void setup() {
  //begin the FrameSerial with 115200 bauds/s
  frameSerial.begin(115200);
  //LED as output -- enable if your board has LED_BUILTIN
  //pinMode(LED_BUILTIN, OUTPUT);

}
//endless loop
void loop() 
{
  //sending once per second without blocking
  if(millis() - sendingTime >= 1000) 
  {
    simTemp = random(1, 40) * 1.324;
    //insert ID = 3 at first position, 
    //to have possibility to send different frames
    frameTransmit[0] = 3;
    //convert float to bytes and insert to buffer
    //starting from position [1] of buffer
    FrameTools::floatToBytes(&simTemp, &frameTransmit[1]);
    //send frame with length 5 (one byte id, and four byte float)
    frameSerial.sendFrame(frameTransmit, 5);
    sendingTime = millis();
  }
  
  //check if is something to read
  if(frameSerial.available() > 0) 
  {
    //read frame
    frameSerial.getFrameFromBuffer(frameReceive);
    //check if frame ID is 3
    if(frameReceive[0] == 3) 
    {
      float receivedValue  = 0.0;
      //convert bytes to float, starting from positon [1] 
      //because we expect that [0] is an ID
      FrameTools::bytesToFloat(&receivedValue, &frameReceive[1]);
      //.....do something with received value
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}
