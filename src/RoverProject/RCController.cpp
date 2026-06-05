#include "RCController.h"


void RCController::begin(HardwareSerial &serial)
{
  // Initialize USB Serial for debugging at a fast speed
  // Serial.begin(115200); should be defined in main file, not here.
    ibus.begin(serial); 
}

void RCController::update() 
{
    lastFrame = ibus.cnt_rec; // Update the previous frame countfor connection status check.
    // Update the iBus data
    ibus.loop(); // This should be called regularly in the main loop to refresh the data from the receiver
}


bool RCController::isconnected()
{
    return lastFrame < ibus.cnt_rec;
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

    
    if (channelIndex == 2 || channelIndex == 4 || channelIndex == 5) { // Throttle (2) and dials(4,5), no midpoint, only edge noise margin.
        return map(inputValue, controllerMinValue + noiseMargine, controllerMaxValue - noiseMargine, minValue, maxValue);

    }
        // For other channels, apply noise margin and map by midpoint   
    else {
        if ( inputValue < controllerMidValue - noiseMargine ) {
            return map(inputValue, controllerMinValue + noiseMargine, controllerMidValue - noiseMargine, minValue, (minValue + maxValue) / 2);
        }
        else if (inputValue > controllerMidValue + noiseMargine ) {
            return map(inputValue, controllerMidValue + noiseMargine, controllerMaxValue - noiseMargine, (minValue + maxValue) / 2, maxValue);
        }

    return (minValue + maxValue) / 2; // Return midpoint value.
    }
}

