#include <Arduino.h>
#include "command-reader.h"

#define BUFFER_LENGTH 8
#define TERMINATOR 0xFF

CommandReaderClass CommandReader;
char buffer[BUFFER_LENGTH];

int CommandReaderClass::readCommand(Command *command)
{
    int i;
    for (i = 0; i < BUFFER_LENGTH;)
    {
        int c = Serial.read();
        if (c < 0)
        {
            continue;
        }
        else if (c == TERMINATOR)
        {
            break;
        }

        buffer[i] = (char)c;
        i++;
    }

    if (i < 1)
    {
        return -1;
    }

    command->CommandType = buffer[0];
    switch (buffer[0])
    {
    case MOTOR_COMMAND:
        if (i < 4)
        {
            Serial.println("A motor command must have at least 4 bytes plus a terminator.");
            return -1;
        }

        command->Channel = buffer[1];
        command->Value = ((uint16_t)buffer[3]) + 0x40 * ((uint16_t)(buffer[2] % 0x04));
        command->IsReversed = (buffer[2] & 0x04) != 0;

        Serial.print("Motor ");
        Serial.print(command->Channel, HEX);
        Serial.print(command->IsReversed ? " set to move backward at speed " : " set to move forward at speed ");
        Serial.println(command->Value, HEX);
        break;

    case POINTS_COMMAND:
        if (i < 2)
        {
            Serial.println("A points command must have at least 2 bytes plus a terminator.");
            return -1;
        }

        command->Channel = buffer[1];
        command->Value = 0x0000;
        command->IsReversed = false;

        Serial.print("A current pulse with be output on MOSFET output ");
        Serial.println(command->Channel, HEX);
        break;

    case READ_CURRENT_COMMAND:
        if (i < 2)
        {
            Serial.println("A read current command must have at least 2 bytes plus a terminator.");
            return -1;
        }

        command->Channel = buffer[1];
        command->Value = 0x0000;
        command->IsReversed = false;

        Serial.print("Read the amount of current drawn from motor channel ");
        Serial.println(command->Channel, HEX);
        break;

    case READ_DEVICE_COMMAND:
        if (i < 2)
        {
            Serial.println("A read device command must have at least 2 bytes plus a terminator.");
            return -1;
        }

        command->Channel = buffer[1];
        command->IsReversed = false;

        Serial.print("Read the the state of device ");
        Serial.println(command->Channel, HEX);
        break;

    default:
        Serial.print("Invalid command: ");
        Serial.println(buffer[0], HEX);
        return -1;
    }

    return 0;
}
