#include <Arduino.h>
#include "motor-controller.h"

MotorControllerClass MotorController;

void MotorControllerClass::begin()
{
    pinMode(this->brakeAPin, OUTPUT);
    digitalWrite(this->brakeAPin, LOW);
    pinMode(this->brakeBPin, OUTPUT);
    digitalWrite(this->brakeBPin, LOW);

    pinMode(this->directionAPin, OUTPUT);
    digitalWrite(this->directionAPin, LOW);
    pinMode(this->directionBPin, OUTPUT);
    digitalWrite(this->directionBPin, LOW);

    pinMode(this->pwmAPin, OUTPUT);
    analogWrite(this->directionAPin, 0x00);
    pinMode(this->directionBPin, OUTPUT);
    analogWrite(this->directionBPin, 0x00);

    pinMode(this->senseAPin, INPUT);
    pinMode(this->senseBPin, INPUT);
}

int MotorControllerClass::getCurrent(unsigned char channel)
{
    if (channel > 1)
    {
        return -1;
    }

    return analogRead(channel != 0 ? this->senseBPin : this->senseAPin);
}

unsigned char MotorControllerClass::getState()
{
    return this->state;
}

int MotorControllerClass::setState(unsigned char channel, unsigned char speed, bool reversed)
{
    if (channel > 1)
    {
        return -1;
    }

    analogWrite(channel != 0x00 ? this->pwmBPin : this->pwmAPin, speed);
    digitalWrite(channel != 0x00 ? this->directionBPin : this->directionAPin, reversed ? HIGH : LOW);

    if (reversed)
    {
        this->state |= 0x01 << channel;
    }
    else
    {
        this->state &= ~(0x01 << channel);
    }

    return this->state;
}
