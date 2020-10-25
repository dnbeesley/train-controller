#include <Arduino.h>

#include "command-reader.h"
#include "motor-controller.h"
#include "points-controller.h"
#include "signal-controller.h"

SingalCondition signalSet0Contions[4] = {
    {.motorState = 0x01,
     .pointsState = 0x01,
     .result = 0x1A},
    {.motorState = 0x01,
     .pointsState = 0x02,
     .result = 0x7A},
    {.motorState = 0x02,
     .pointsState = 0x01,
     .result = 0xA9},
    {.motorState = 0x02,
     .pointsState = 0x02,
     .result = 0xA6}};

SignalSet signalSets[1] = {
    {.conditions = signalSet0Contions,
     .conditionCount = 4,
     .address = 0x60,
     .currentState = 0x0,
     .defaultState = 0xFF}};

Command command;
int returnValue;
bool potentialSignalChange;

void setup()
{
  Serial.begin(9600);
  MotorController.begin();
  PointsController.begin(16, 10, 7, 6, 5, 4, 2, 500);
  SignalController.begin(signalSets, 1);
}

void loop()
{
  potentialSignalChange = false;
  returnValue = CommandReader.readCommand(&command);
  if (returnValue == 0)
  {
    switch (command.CommandType)
    {
    case MOTOR_COMMAND:
      potentialSignalChange = true;
      returnValue = MotorController.setState(command.Channel, command.Value, command.IsReversed);
      Serial.print("Motor state ");
      Serial.println(returnValue, HEX);
      break;
    case POINTS_COMMAND:
      potentialSignalChange = true;
      returnValue = PointsController.outputPulse(command.Channel);
      Serial.print("Points state ");
      Serial.println(returnValue, HEX);
      break;
    case READ_CURRENT_COMMAND:
      returnValue = MotorController.getCurrent(command.Channel);
      Serial.print("Current drawn ");
      Serial.println(returnValue, HEX);
      break;
    }

    if (potentialSignalChange)
    {
      SignalController.updateSignalSets(MotorController.getState(), PointsController.getState());
    }
  }
}
