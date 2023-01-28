#include <Arduino.h>
#include <Wire.h>
#include "points-controller.h"

PointsControllerClass PointsController;

void PointsControllerClass::begin(uint8_t addr)
{
    this->addr = addr;
    this->state = 0x1555;

    Wire.beginTransmission(this->addr);
    Wire.write((uint8_t*)&this->state, 2);
    Wire.endTransmission();
}

void PointsControllerClass::calculateNewState(uint8_t channel)
{
    uint8_t complementaryChannel = channel ^ 0x01;
    uint16_t bitSet = 0x0001 << channel;
    uint16_t complementaryBitSet = ~(0x0001 << complementaryChannel);

    this->state |= bitSet;
    this->state &= complementaryBitSet;
}

uint16_t PointsControllerClass::getState()
{
    return this->state;
}

int PointsControllerClass::outputPulse(uint8_t channel)
{
    if (channel > 13)
    {
        return -1;
    }

    this->calculateNewState(channel);
    Wire.beginTransmission(this->addr);
    Wire.write((uint8_t*)&this->state, 2);
    Wire.endTransmission();
    return this->state;
}
