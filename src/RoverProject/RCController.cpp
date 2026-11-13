#include "RCController.h"


void RCController::begin(HardwareSerial &serial)
{
  // Initialize USB Serial for debugging at a fast speed
  // Serial.begin(115200); should be defined in main file, not here.
    // NOTIMER: update() is the only caller of ibus.loop(); the default timer ISR would also call it and corrupt frames.
    ibus.begin(serial, IBUSBM_NOTIMER);
}

void RCController::update() 
{
    // Update the iBus data
    ibus.loop(); // This should be called regularly in the main loop to refresh the data from the receiver

    // cnt_rec increments (and wraps) on every received frame, so any change means a new frame.
    if (ibus.cnt_rec != lastFrame) {
        lastFrame = ibus.cnt_rec;
        lastFrameTime = millis();
        frameReceived = true;
    }
}


bool RCController::isconnected()
{
    return frameReceived && (millis() - lastFrameTime < linkTimeoutLimit);
}

int RCController::readChannel(int channelIndex, int minValue, int maxValue)
{
    // Return the value of the specified channel
    if (channelIndex < 0 || channelIndex >= 6) {
        return 0; // Return 0 for invalid index
    }
    return mapChannelValue(channelIndex, ibus.readChannel(channelIndex), minValue, maxValue);
}

int RCController::mapChannelValue(int channelIndex, int inputValue, int minValue, int maxValue)
{
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

