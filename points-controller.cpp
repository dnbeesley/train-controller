#include <Arduino.h>
#include "points-controller.h"

PointsControllerClass PointsController;

void PointsControllerClass::begin(
    uint8_t enablePin,
    uint8_t output0Pin,
    uint8_t output1Pin,
    uint8_t output2Pin,
    uint8_t output3Pin,
    uint8_t output4Pin,
    uint8_t output5Pin,
    uint16_t pulseTime)
{
    this->enablePin = enablePin;
    this->output0Pin = output0Pin;
    this->output1Pin = output1Pin;
    this->output2Pin = output2Pin;
    this->output3Pin = output3Pin;
    this->output4Pin = output4Pin;
    this->output5Pin = output5Pin;
    this->pulseTime = pulseTime;
    this->state = 0x1555;

    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
    pinMode(output0Pin, OUTPUT);
    digitalWrite(output0Pin, LOW);
    pinMode(output1Pin, OUTPUT);
    digitalWrite(output1Pin, LOW);
    pinMode(output2Pin, OUTPUT);
    digitalWrite(output2Pin, LOW);
    pinMode(output3Pin, OUTPUT);
    digitalWrite(output3Pin, LOW);
    pinMode(output4Pin, OUTPUT);
    digitalWrite(output4Pin, LOW);
    pinMode(output5Pin, OUTPUT);
    digitalWrite(output5Pin, LOW);
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

    this->outputPulseImpl(channel);
    this->calculateNewState(channel);
    return this->state;
}

void PointsControllerClass::outputPulseImpl(uint8_t channel)
{
    // 7 outputs on each multiplexer connected to MOSFETs.
    uint8_t multiplexer = channel / 7;
    Serial.print("Using multiplexer: ");
    Serial.println(multiplexer, DEC);

    // Output 0 of each multiplexer is not connected to anything.
    uint8_t multiplexerInput = (channel % 7) + 1;
    Serial.print("Outputting signature: ");
    Serial.println(multiplexerInput, DEC);
    this->setPin(
        multiplexer == 0 ? this->output0Pin : this->output3Pin,
        (multiplexerInput & 0x01) != 0x00 ? HIGH : LOW);
    this->setPin(multiplexer == 0 ? this->output3Pin : this->output0Pin, LOW);

    this->setPin(
        multiplexer == 0 ? this->output1Pin : this->output4Pin,
        (multiplexerInput & 0x02) != 0x00 ? HIGH : LOW);
    this->setPin(multiplexer == 0 ? this->output4Pin : this->output1Pin, LOW);

    this->setPin(
        multiplexer == 0 ? this->output2Pin : this->output4Pin,
        (multiplexerInput & 0x04) != 0x00 ? HIGH : LOW);
    this->setPin(multiplexer == 0 ? this->output5Pin : this->output2Pin, LOW);

    this->setPin(this->enablePin, HIGH);
    delay(this->pulseTime);
    this->setPin(this->enablePin, LOW);

    this->setPin(this->output0Pin, LOW);
    this->setPin(this->output1Pin, LOW);
    this->setPin(this->output2Pin, LOW);
    this->setPin(this->output3Pin, LOW);
    this->setPin(this->output4Pin, LOW);
    this->setPin(this->output5Pin, LOW);
}

void PointsControllerClass::setPin(uint8_t pin, uint8_t value)
{
    Serial.print("Setting pin: ");
    Serial.print(pin, DEC);
    Serial.println(value == HIGH ? " high" : " low");
    digitalWrite(pin, value);
}
