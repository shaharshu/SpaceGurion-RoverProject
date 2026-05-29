#ifndef RC_CONTROLLER_H
#define RC_CONTROLLER_H
#include <Arduino.h>
#include <IBusBM.h>

/**
 * FLYSKY FS-i6X 6-CHANNEL MAPPING:
 * -----------------------------------------------
 * Channel 0 (Roll):      camera Left/Right (Right Stick X-axis) 
 * Channel 1 (Pitch):     camera up/down (Right Stick Y-axis)
 * Channel 2 (Throttle):  forward drive (Left Stick Y-axis) stays at same position when released, start cntroller when set to minimum.
 * Channel 3 (Yaw):       steer Left/Right (Left Stick X-axis)
 * * Note: Values range from 1000 to 2000. 
 * Center is ~1500.
 */
int noiseMargine = 10;
int controllerMinValue = 1000;
int controllerMaxValue = 2000;
int controllerMidValue = (controllerMinValue + controllerMaxValue) / 2;

// connect iBus receiver to Serial1 (RX1: pin 19) on Arduino Mega 
class RCController {
    private:
        IBusBM ibus;
    public:
        void begin(HardwareSerial &serial); // initializes the controller with given serial port, stops the program if colntroller is not connected.
        void update(); // updates the controller state, needs to be called at least once in the main loop to refresh the data.
        int readChannel(int channelIndex, int minValue, int maxValue); // returns value of selected channel index, in operation range of minValue to maxValue, and noise margine.
        bool isConnected(); // returns true if controller is connected



};

#endif // RC_CONTROLLER_H