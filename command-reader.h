#include "command.h"

#define MOTOR_COMMAND 0x01
#define POINTS_COMMAND 0x02
#define READ_CURRENT_COMMAND 0x03
#define READ_DEVICE_COMMAND 0x04

/**
 * Class for reding commands from the serial interface.
 **/
class CommandReaderClass
{
public:
    /**
     * Reads a command from the serial interface.
     * @param serial The stream from which to read.
     * @param command A data structure to store the command information inside.
     * @return O if successful, -1 otherwise.
     **/
    int readCommand(Command* command);
};

extern CommandReaderClass CommandReader;
