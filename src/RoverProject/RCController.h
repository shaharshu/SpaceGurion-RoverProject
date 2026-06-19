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
        uint32_t lastFrame = 0;
        
    public:
        void begin(HardwareSerial &serial); // initializes the controller with given serial port, stops the program if colntroller is not connected.
        void update(); // updates the controller state, needs to be called at least once in the main loop to refresh the data.
        bool isconnected(); // returns true if the controller comunication is connected, false otherwise.
        int readChannel(int channelIndex, int minValue = controllerMinValue, int maxValue = controllerMaxValue); // returns value of selected channel index, in operation range of minValue to maxValue, and noise margine.

};

#endif // RC_CONTROLLER_H