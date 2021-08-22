#include <Arduino.h>
#include <Wire.h>
#include "signal-controller.h"

SignalControllerClass SignalController;

void SignalControllerClass::begin(SignalSet *signalSets, unsigned int signalSetCount)
{
    Wire.begin();
    this->signalSets = signalSets;
    this->signalSetCount = signalSetCount;

    for (int i = 0; i < this->signalSetCount; i++)
    {
        Wire.beginTransmission(this->signalSets[i].address);
        Wire.write(0x44);
        Wire.write(this->signalSets[i].defaultState);
        Wire.endTransmission();
        this->signalSets[i].currentState = this->signalSets[i].defaultState;

        Serial.print("Initialised state of signal @ address: ");
        Serial.print(this->signalSets[i].address, HEX);
        Serial.print(" to: ");
        Serial.println(this->signalSets[i].defaultState, HEX);
    }
}

int SignalControllerClass::readDeviceState(uint8_t address)
{
  Wire.requestFrom(address, (uint8_t)2);
  Serial.print("Bytes available: ");
  Serial.println(Wire.available());
  Serial.print("Device ");
  Serial.print(address, HEX);
  Serial.print(" state: ");
  Serial.print(Wire.read());
  Serial.print(", ");
  Serial.println(Wire.read());
  return 0;
}

void SignalControllerClass::updateSignalSets(unsigned short motorState, unsigned short pointsState)
{
    for (int i = 0; i < this->signalSetCount; i++)
    {
        unsigned char newState = this->calculateNewState(&this->signalSets[i], motorState, pointsState);
        if (newState != this->signalSets[i].currentState)
        {
            Wire.beginTransmission(this->signalSets[i].address);
            Wire.write(0x44);
            Wire.write(newState);
            Wire.endTransmission();
            this->signalSets[i].currentState = newState;

            Serial.print("Changed state of signal @ address: ");
            Serial.print(this->signalSets[i].address, HEX);
            Serial.print(" to: ");
            Serial.println(newState, HEX);
        }
    }
}

unsigned char SignalControllerClass::calculateNewState(SignalSet *signalSet, unsigned short motorState, unsigned short pointsState)
{
    for (int i = 0; i < signalSet->conditionCount; i++)
    {
        if (this->statesMatch(signalSet->conditions[i].motorState, motorState) &&
            this->statesMatch(signalSet->conditions[i].pointsState, pointsState))
        {
            return signalSet->conditions[i].result;
        }
    }

    return signalSet->defaultState;
}

bool SignalControllerClass::statesMatch(unsigned short expected, unsigned short actual)
{
    return (expected & actual) == expected;
}
