#include "RCController.h"

RCController::RCController()
{
}

void RCController::begin(HardwareSerial &serial)
{
    // Initialize iBus receiver on the specified serial port
    ibus.begin(serial);
}

void RCController::update()
{
    // Update the iBus receiver data
    ibus.loop();
}

int RCController::readChannel(int channelIndex)
{
    // Return the value of the specified channel
    return ibus.readChannel(channelIndex);
}

bool RCController::isConnected()
{
    // Return connection status
    return ibus.isConnected();
}
