#define MOTOR_COMMAND 0x01
#define POINTS_COMMAND 0x02
#define READ_CURRENT_COMMAND 0x03
#define READ_DEVICE_COMMAND 0x04

struct Command
{
    uint8_t CommandType;
    uint8_t Channel;
    uint16_t Value;
    bool IsReversed;
};
