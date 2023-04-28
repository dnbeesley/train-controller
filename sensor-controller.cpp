#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "command.h"
#include "sensor-controller.h"

SensorControllerClass SensorController;

int SensorControllerClass::readDeviceState(Sensor *sensor)
{
    StaticJsonDocument<64> doc;
    Wire.requestFrom(sensor->address, sensor->length);
    int available = Wire.available();
    Serial.print(available);
    Serial.println(" bytes available to read");
    for (int i = 0; (i < available && i < sensor->length); i++)
    {
        this->buffer[i] = Wire.read();
    }

    while (Wire.read() != -1)
    {
    }

    doc["cmd"] = READ_DEVICE_COMMAND;
    doc["address"] = sensor->address;
    JsonArray data = doc.createNestedArray("states");
    for (int i = 0; (i < available && i < sensor->length); i++)
    {
        data.add(this->buffer[i]);
    }

    serializeJson(doc, Serial);
    Serial.println();

    return available;
}
