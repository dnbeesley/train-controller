/**
 * Controllers the points motors using a pair of demultiplexers controll MOSFET which inturn allow current to flow
 * through the solenoids. Can controll 14 channels.
 **/
class PointsControllerClass
{
public:
    /**
         * Initialises a new instance of the PointsController class.
         * @param addr The address of the TWI device that controls the points
         **/
    void begin(uint8_t addr);

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
    uint16_t state;
    uint8_t addr;

    void calculateNewState(uint8_t channel);
};

extern PointsControllerClass PointsController;
