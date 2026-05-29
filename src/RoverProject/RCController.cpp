#include "RCController.h"


void RCController::begin(HardwareSerial &serial)
{
  // Initialize USB Serial for debugging at a fast speed
  // Serial.begin(115200); should be defined in main file, not here.
    myRemote.begin(Serial1); 
}

void RCController::update() 
{
    // Update the iBus data
    ibus.loop(); // This should be called regularly in the main loop to refresh the data from the receiver
}

int RCController::readChannel(int channelIndex, int minValue, int maxValue)
{
    // Return the value of the specified channel
    if (channelIndex < 0 || channelIndex >= 6) {
        return 0; // Return 0 for invalid index
    }
    int inputValue = ibus.readChannel(channelIndex);
    // Apply noise margin at the edges of the controller range
    if (inputValue < controllerMinValue + noiseMargine) {
            return minValue; // Treat as minimum value
        } else if (inputValue > controllerMaxValue - noiseMargine) {
            return maxValue; // Treat as maximum value
        }

    
    if (channelIndex == 2 || channelIndex <= 5 ) { // Throttle and dials, no midpoint, only edge noise margin.
        return map(inputValue, controllerMinValue + noiseMargine, controllerMaxValue - noiseMargine, minValue, maxValue);

    }
    // For other channels, apply noise margin and map by midpoint
    else {
        if ( inputValue < controllerMidValue - noiseMargine ) 
            return map(inputValue, controllerMinValue + noiseMargine, controllerMidValue - noiseMargine, minValue, (minValue + maxValue) / 2);
        }
        else if (inputValue > controllerMidValue + noiseMargine ) {
            return map(inputValue, controllerMidValue + noiseMargine, controllerMaxValue - noiseMargine, (minValue + maxValue) / 2, maxValue);
        }

    return (minValue + maxValue) / 2; // Return midpoint value.
}

bool RCController::isConnected()
{
    // Return connection status
    return ibus.isConnected();
}
