#define SENSOR_BUFFER_SIZE 4

typedef struct Sensor
{
    uint8_t address;
    uint8_t length;
};

class SensorControllerClass
{
public:
    /**
     * Reads the device state and outputs the value to the serial port
     *
     * @param sensor
     * @return int The number of bytes read
     */
    int readDeviceState(Sensor *sensor);

private:
    uint8_t buffer[SENSOR_BUFFER_SIZE];
};

extern SensorControllerClass SensorController;
