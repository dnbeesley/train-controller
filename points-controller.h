/**
 * Controllers the points motors using a pair of demultiplexers controll MOSFET which inturn allow current to flow
 * through the solenoids. Can controll 14 channels.
 **/
class PointsControllerClass
{
public:
    /**
         * Initialises a new instance of the PointsController class.
         * @param enablePin The arduino pin to enable the output on the multiplexers
         * @param output0Pin The input pin 0 of the first multiplexer
         * @param output1Pin The input pin 1 of the first multiplexer
         * @param output2Pin The input pin 2 of the first multiplexer
         * @param output3Pin The input pin 0 of the second multiplexer
         * @param output4Pin The input pin 1 of the second multiplexer
         * @param output5Pin The input pin 2 of the second multiplexer
         * @param pulseTime The number of milliseconds which the output pulse should last.
         **/
    void begin(
        uint8_t enablePin,
        uint8_t output0Pin,
        uint8_t output1Pin,
        uint8_t output2Pin,
        uint8_t output3Pin,
        uint8_t output4Pin,
        uint8_t output5Pin,
        uint16_t pulseTime);

    /**
         * Returns the state of the set of points as a bit set.
         * @return The state as a bitset.
         */
    uint16_t getState();

    /**
         * Allows a pulse of current to flow through the specified channel.
         * @param channel The channel on which to allow the output.
         * @return The new state if channel number is valid, -1 otherwise
         **/
    int outputPulse(uint8_t channel);

private:
    uint16_t pulseTime;
    uint16_t state;
    uint8_t enablePin;
    uint8_t output0Pin;
    uint8_t output1Pin;
    uint8_t output2Pin;
    uint8_t output3Pin;
    uint8_t output4Pin;
    uint8_t output5Pin;

    void calculateNewState(uint8_t channel);
    void outputPulseImpl(uint8_t channel);
    void setPin(uint8_t pin, uint8_t value);
};

extern PointsControllerClass PointsController;
