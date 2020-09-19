#include <Arduino.h>

#include "command-reader.h"
#include "motor-controller.h"
#include "points-controller.h"

Command command;
int returnValue;

void setup()
{
  Serial.begin(9600);
  MotorController.begin();
  PointsController.begin(16, 10, 7, 6, 5, 4, 2, 500);
}

void loop()
{
  returnValue = CommandReader.readCommand(&command);
  if (returnValue == 0)
  {
    switch (command.CommandType)
    {
    case MOTOR_COMMAND:
      returnValue = MotorController.setState(command.Channel, command.Value, command.IsReversed);
      Serial.print("Motor state ");
      Serial.println(returnValue, HEX);
      break;
    case POINTS_COMMAND:
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
  }
}
