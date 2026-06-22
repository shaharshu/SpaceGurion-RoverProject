#include "RCController.h"


RCController::RCController(const FailsefeRule* failesafeRules, uint8_t rulesCount, uint8_t totalChannels)
{
    fsRules = failesafeRules;
    this->rulesCount = rulesCount;
    activeChannelsCount = totalChannels;
}

void RCController::begin(HardwareSerial &serial)
{
  // Initialize USB Serial for debugging at a fast speed
  // Serial.begin(115200); should be defined in main file, not here.
    ibus.begin(serial);
    isConnected = false;
    fsTime = 0;
}

bool RCController::update() // This should be called by the main Arduino loop 
{
    // Update the iBus data
    ibus.loop(); // This should be called regularly in the main loop to refresh the data from the receiver
    if(ibus.readChannel(0) <= noiseMargine)
    {
        isConnected = false;
        fsTime = 0;
        return false;
    }

    if (_isFailsafeTriggered())
    {
        if (fsTime == 0) {
            fsTime = millis();
        }
        else if (millis() - fsTime >= linkTimeoutLimit) {
            isConnected = false;
        }
    }
    else
    {
        isConnected = true;
        fsTime = 0;
    }

    return isConnected;
}

bool RCController::_isFailsafeTriggered()
{
    bool isFailsafeTriggered = true;
    int i;

    if (rulesCount == 0 || fsRules == nullptr)
    {
        isConnected = true;
        return false;
    }
    
    for(i = 0; i < rulesCount; i++){
        if(ibus.readChannel(fsRules[i].channel) != fsRules[i].value)
        {
            isFailsafeTriggered = false;
            break;
        }
    }

    return isFailsafeTriggered;
}

bool RCController::isconnected()
{
    return isConnected;
}

int RCController::readChannel(int channelIndex, int minValue, int maxValue)
{
    // Return the value of the specified channel
    if (channelIndex < 0 || channelIndex >= activeChannelsCount) {
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

