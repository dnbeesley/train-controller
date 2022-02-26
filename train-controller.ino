#include <Arduino.h>
#include <ArduinoJson.h>

#include "command-reader.h"
#include "motor-controller.h"
#include "points-controller.h"
#include "signal-controller.h"

# define MAX_BYTES_TO_READ 4

SignalCondition signalSet0Contions[4] = {
    {.motorState = 0x01,
     .pointsState = 0x01,
     .result = 0xA9},
    {.motorState = 0x05,
     .pointsState = 0x02,
     .result = 0xA6},
    {.motorState = 0x02,
     .pointsState = 0x01,
     .result = 0x4A},
    {.motorState = 0x0A,
     .pointsState = 0x02,
     .result = 0xDA}};

SignalCondition signalSet1Contions[4] = {
    {.motorState = 0x01,
     .pointsState = 0x04,
     .result = 0xA6},
    {.motorState = 0x09,
     .pointsState = 0x08,
     .result = 0xA9},
    {.motorState = 0x02,
     .pointsState = 0x04,
     .result = 0x4A},
    {.motorState = 0x06,
     .pointsState = 0x18,
     .result = 0xDA}};

SignalCondition signalSet2Contions[7] = {
    {.motorState = 0x09,
     .pointsState = 0x18,
     .result = 0xA1},
    {.motorState = 0x08,
     .pointsState = 0x10,
     .result = 0xA3},
    {.motorState = 0x08,
     .pointsState = 0x20,
     .result = 0xA6},
    {.motorState = 0x05,
     .pointsState = 0x62,
     .result = 0x9A},
    {.motorState = 0x04,
     .pointsState = 0x60,
     .result = 0xBA},
    {.motorState = 0x05,
     .pointsState = 0xA2,
     .result = 0x6A},
    {.motorState = 0x04,
     .pointsState = 0xA0,
     .result = 0xEA}};

SignalSet signalSets[3] = {
    {.conditions = signalSet0Contions,
     .conditionCount = 4,
     .address = 0x60,
     .currentState = 0x0,
     .defaultState = 0xAA},
    {.conditions = signalSet1Contions,
     .conditionCount = 4,
     .address = 0x61,
     .currentState = 0x0,
     .defaultState = 0xAA},
    {.conditions = signalSet2Contions,
     .conditionCount = 7,
     .address = 0x62,
     .currentState = 0x0,
     .defaultState = 0xAA}};

Command command;
int returnValue;
uint8_t state[MAX_BYTES_TO_READ];
uint8_t bytesToRead;
bool potentialSignalChange;

void setup()
{
  Serial.begin(9600);
  MotorController.begin();
  PointsController.begin(16, 10, 7, 6, 5, 4, 2, 500);
  SignalController.begin(signalSets, 3);
}

void loop()
{
  StaticJsonDocument<64> doc;
  potentialSignalChange = false;
  returnValue = CommandReader.readCommand(&command);
  if (returnValue == 0)
  {
    doc["cmd"] = command.CommandType;
    switch (command.CommandType)
    {
    case MOTOR_COMMAND:
      potentialSignalChange = true;
      returnValue = MotorController.setState(command.Channel, command.Value, command.IsReversed);
      doc["state"] = returnValue;
      break;
    case POINTS_COMMAND:
      potentialSignalChange = true;
      returnValue = PointsController.outputPulse(command.Channel);
      doc["state"] = returnValue;
      break;
    case READ_CURRENT_COMMAND:
      returnValue = MotorController.getCurrent(command.Channel);
      doc["state"] = returnValue;
      break;
    case READ_DEVICE_COMMAND:
      if (command.Value < MAX_BYTES_TO_READ) {
        bytesToRead = command.Value;
      } else {
        bytesToRead = MAX_BYTES_TO_READ;
      }

      returnValue = SignalController.readDeviceState(command.Channel, state, command.Value);
      doc["address"] = command.Channel;
      JsonArray data = doc.createNestedArray("states");
      for(int i = 0; (i < returnValue && i < bytesToRead); i++) {
        data.add(state[i]);
      }

      break;
    }

    if (potentialSignalChange)
    {
      SignalController.updateSignalSets(MotorController.getState(), PointsController.getState());
    }

    serializeJson(doc, Serial);
    Serial.println();
  }
}
