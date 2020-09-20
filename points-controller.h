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
        unsigned char enablePin,
        unsigned char output0Pin,
        unsigned char output1Pin,
        unsigned char output2Pin,
        unsigned char output3Pin,
        unsigned char output4Pin,
        unsigned char output5Pin,
        unsigned short pulseTime);

    /**
         * Returns the state of the set of points as a bit set.
         * @return The state as a bitset.
         */
    unsigned short getState();

    /**
         * Allows a pulse of current to flow through the specified channel.
         * @param channel The channel on which to allow the output.
         * @return The new state if channel number is valid, -1 otherwise
         **/
    int outputPulse(unsigned char channel);

private:
    unsigned short pulseTime;
    unsigned short state;
    unsigned char enablePin;
    unsigned char output0Pin;
    unsigned char output1Pin;
    unsigned char output2Pin;
    unsigned char output3Pin;
    unsigned char output4Pin;
    unsigned char output5Pin;

    void calculateNewState(unsigned char channel);
    void outputPulseImpl(unsigned char channel);
    void setPin(uint8_t pin, uint8_t value);
};

extern PointsControllerClass PointsController;
