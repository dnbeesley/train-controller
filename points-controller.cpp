#include <Arduino.h>
#include "points-controller.h"

PointsControllerClass PointsController;

void PointsControllerClass::begin(
    unsigned char enablePin,
    unsigned char output0Pin,
    unsigned char output1Pin,
    unsigned char output2Pin,
    unsigned char output3Pin,
    unsigned char output4Pin,
    unsigned char output5Pin,
    unsigned short pulseTime)
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

void PointsControllerClass::calculateNewState(unsigned char channel)
{
    unsigned char complementaryChannel = channel ^ 0x01;
    unsigned short bitSet = 0x0001 << channel;
    unsigned short complementaryBitSet = ~(0x0001 << complementaryChannel);

    this->state |= bitSet;
    this->state &= complementaryBitSet;
}

unsigned short PointsControllerClass::getState()
{
    return this->state;
}

int PointsControllerClass::outputPulse(unsigned char channel)
{
    if (channel > 13)
    {
        return -1;
    }

    this->outputPulseImpl(channel);
    this->calculateNewState(channel);
    return this->state;
}

void PointsControllerClass::outputPulseImpl(unsigned char channel)
{
    // 7 output on each multiplexer connected to MOSFETs.
    unsigned char multiplexer = channel / 7;

    // Output 0 of each multiplexer is not connected to anything.
    unsigned char multiplexerInput = (channel % 7) + 1;
    digitalWrite(
        multiplexer != 0 ? this->output0Pin : this->output3Pin,
        (multiplexerInput & 0x01) != 0x00 ? HIGH : LOW);
    digitalWrite(multiplexer != 0 ? this->output3Pin : this->output0Pin, LOW);

    digitalWrite(
        multiplexer != 0 ? this->output1Pin : this->output4Pin,
        (multiplexerInput & 0x02) != 0x00 ? HIGH : LOW);
    digitalWrite(multiplexer != 0 ? this->output4Pin : this->output1Pin, LOW);

    digitalWrite(
        multiplexer != 0 ? this->output1Pin : this->output4Pin,
        (multiplexerInput & 0x04) != 0x00 ? HIGH : LOW);
    digitalWrite(multiplexer != 0 ? this->output5Pin : this->output2Pin, LOW);

    digitalWrite(this->enablePin, HIGH);
    delay(this->pulseTime);
    digitalWrite(this->enablePin, LOW);

    digitalWrite(this->output0Pin, LOW);
    digitalWrite(this->output1Pin, LOW);
    digitalWrite(this->output2Pin, LOW);
    digitalWrite(this->output3Pin, LOW);
    digitalWrite(this->output4Pin, LOW);
    digitalWrite(this->output5Pin, LOW);
}
