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
    int getCurrent(uint8_t channel);

    /**
         * Gets the state of the motor controller.
         * @returns The state as a bitset.
         **/
    uint8_t getState();

    /**
         * Sets the state for the specified channel.
         * @param channel The channel to use. (0x0 for A and 0x1 for B).
         * @param speed The PWM output.
         * @param reversed Whether the direction of the current should be reversed.
         * @return The new state, if input is valid, -1 otherwise.
         **/
    int setState(uint8_t channel, uint8_t speed, bool reversed);

private:
    const uint8_t brakeAPin = 9;
    const uint8_t brakeBPin = 8;
    const uint8_t directionAPin = 12;
    const uint8_t directionBPin = 13;
    const uint8_t pwmAPin = 3;
    const uint8_t pwmBPin = 11;
    const uint8_t senseAPin = 0;
    const uint8_t senseBPin = 1;
    uint8_t state;
};

extern MotorControllerClass MotorController;