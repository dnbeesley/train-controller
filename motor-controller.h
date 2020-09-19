/**
 * Uses the arduino motor shield to control power to the tracks.
 **/
class MotorControllerClass
{
public:
    /**
     * Initialise instance.
     * */
    void begin();

    /**
         * Gets the ammount of current being drawn through the specified channel.
         * @param channel The channel to monitor.
         * @return The amount of current being drawn, if the channel is valid, -1 otherise.
         **/
    int getCurrent(unsigned char channel);

    /**
         * Gets the state of the motor controller.
         * @returns The state as a bitset.
         **/
    unsigned char getState();

    /**
         * Sets the state for the specified channel.
         * @param channel The channel to use. (0x0 for A and 0x1 for B).
         * @param speed The PWM output.
         * @param reversed Whether the direction of the current should be reversed.
         * @return The new state, if input is valid, -1 otherwise.
         **/
    int setState(unsigned char channel, unsigned char speed, bool reversed);

private:
    const unsigned char brakeAPin = 9;
    const unsigned char brakeBPin = 8;
    const unsigned char directionAPin = 12;
    const unsigned char directionBPin = 13;
    const unsigned char pwmAPin = 3;
    const unsigned char pwmBPin = 11;
    const unsigned char senseAPin = 0;
    const unsigned char senseBPin = 1;
    unsigned char state;
};

extern MotorControllerClass MotorController;