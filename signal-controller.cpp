#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "signal-controller.h"

SignalControllerClass SignalController;

void SignalControllerClass::begin(SignalSet *signalSets, unsigned int signalSetCount)
{
    StaticJsonDocument<64> doc;
    JsonObject obj = doc.createNestedObject("twi");
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

        obj[String(this->signalSets[i].address, HEX)] = this->signalSets[i].defaultState;
    }

    serializeJson(doc, Serial);
    Serial.println();
}

int SignalControllerClass::readDeviceState(uint8_t address, uint8_t* buffer, uint8_t quantity)
{
    Wire.requestFrom(address, quantity);
    int available = Wire.available();
    Serial.print(available);
    Serial.println(" bytes availabe to read");
    for(int i = 0; (i < available && i < quantity); i++) {
        buffer[i] = Wire.read();
    }

    while (Wire.read() != -1) {        
    }

    return available;
}

void SignalControllerClass::updateSignalSets(uint16_t motorState, uint16_t pointsState)
{
    StaticJsonDocument<64> doc;
    JsonObject obj = doc.createNestedObject("twi");
    bool hasEntries = false;

    for (int i = 0; i < this->signalSetCount; i++)
    {
        uint8_t newState = this->calculateNewState(&this->signalSets[i], motorState, pointsState);
        if (newState != this->signalSets[i].currentState)
        {
            Wire.beginTransmission(this->signalSets[i].address);
            Wire.write(0x44);
            Wire.write(newState);
            Wire.endTransmission();
            this->signalSets[i].currentState = newState;

            hasEntries = true;
            obj[String(this->signalSets[i].address, HEX)] = newState;
        }
    }

    if(hasEntries) {
        serializeJson(doc, Serial);
        Serial.println();
    }
}

uint8_t SignalControllerClass::calculateNewState(SignalSet *signalSet, uint16_t motorState, uint16_t pointsState)
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

bool SignalControllerClass::statesMatch(uint16_t expected, uint16_t actual)
{
    return (expected & actual) == expected;
}
