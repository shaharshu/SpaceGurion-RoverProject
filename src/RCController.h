#ifndef RC_CONTROLLER_H
#define RC_CONTROLLER_H
#include <Arduino.h>
#include <iBusBM.h>

/**
 * FLYSKY FS-i6X 6-CHANNEL MAPPING:
 * -----------------------------------------------
 * Channel 0 (Roll):      Steer Left/Right (Right Stick X-axis)
 * Channel 1 (Pitch):     Forward/Backward (Right Stick Y-axis)
 * Channel 2 (Throttle):  Camera Up/Down   (Left Stick Y-axis)
 * Channel 3 (Yaw):       Camera Left/Right (Left Stick X-axis)
 * * Note: Values range from 1000 to 2000. 
 * Center is ~1500.
 */

// connect iBus receiver to Serial1 (RX1: pin 19) on Arduino Mega 
class RCController {
    private:
        iBusBM ibus;
    public:
        RCController();
        void begin(HardwareSerial &serial); // initializes the controller with given serial port
        void update(); // updates the controller state
        int readChannel(int channelIndex); // returns channel value of selected channel index
        bool isConnected(); // returns true if controller is connected



};

#endif // RC_CONTROLLER_H