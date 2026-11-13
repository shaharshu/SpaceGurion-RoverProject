#ifndef RC_CONTROLLER_H
#define RC_CONTROLLER_H
#include <Arduino.h>
#include <IBusBM.h>
#include "config.h"

/*
 FLYSKY FS-i6X 6-CHANNEL MAPPING:
 -----------------------------------------------
 * Channel 0 : Right Stick X
 * Channel 1 : Right Stick Y
 * Channel 2 : Left Stick Y  stays at same position when released, start controller when set to minimum.
 * Channel 3 : Left Stick X
 * channel 4 : Left Dial 
 * channel 5 : Right Dial 
 ------------------------------------------------
 Values range from 1000 to 2000. 
 ------------------------------------------------
 conetions on the receiver:
 voltege(5V): middle row of reciever, usually on B/Vcc column
 ground: middle row of reciever, usually on B/Vcc column
 controller signal: CH1 
-------------------------------------------------
connections on board:
controller signal input: Serial1 RX pin (pin 19) on Arduino Mega
*/

// connect iBus receiver to Serial1 (RX1: pin 19) on Arduino Mega 
class RCController {
    private:
        IBusBM ibus;
        uint32_t lastFrame = 0;      // last seen value of ibus.cnt_rec
        uint32_t lastFrameTime = 0;  // millis() when a new frame was last seen
        bool frameReceived = false;  // true once at least one frame has arrived
        
    public:
        void begin(HardwareSerial &serial); // initializes the controller with given serial port.
        void update(); // updates the controller state, must be called every main loop iteration (iBus frames arrive every ~7ms).
        bool isconnected(); // returns true if a frame was received within the last linkTimeoutLimit ms, false otherwise.
        int readChannel(int channelIndex, int minValue = controllerMinValue, int maxValue = controllerMaxValue); // returns value of selected channel index, in operation range of minValue to maxValue, and noise margine.
        static int mapChannelValue(int channelIndex, int inputValue, int minValue = controllerMinValue, int maxValue = controllerMaxValue); // maps a raw channel value (1000..2000) the same way readChannel does; usable without a receiver (bench tests).

};

#endif // RC_CONTROLLER_H