#include <Arduino.h>
#include "motor-controller.h"

MotorControllerClass MotorController;

void MotorControllerClass::begin()
{
    this->state = 0;
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

int MotorControllerClass::getCurrent(uint8_t channel)
{
    if (channel > 1)
    {
        return -1;
    }

    return analogRead(channel != 0 ? this->senseBPin : this->senseAPin);
}

uint8_t MotorControllerClass::getState()
{
    return this->state;
}

int MotorControllerClass::setState(uint8_t channel, uint8_t speed, bool reversed)
{
    if (channel > 1)
    {
        return -1;
    }

    analogWrite(channel != 0x00 ? this->pwmBPin : this->pwmAPin, speed);
    digitalWrite(channel != 0x00 ? this->directionBPin : this->directionAPin, reversed ? HIGH : LOW);

    if(speed == 0)
    {
        this->state &= ~(0x01 << (channel * 2));
        this->state &= ~(0x01 << (channel * 2 + 1));
    }
    else if (reversed)
    {
        this->state &= ~(0x01 << (channel * 2));
        this->state |= 0x01 << (channel * 2 + 1);
    }
    else
    {
        this->state |= 0x01 << (channel * 2);
        this->state &= ~(0x01 << (channel * 2 + 1));
    }

    return this->state;
}
