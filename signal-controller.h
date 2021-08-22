
struct SignalCondition
{
    uint16_t motorState;
    uint16_t pointsState;
    uint8_t result;
};

struct SignalSet
{
    SignalCondition *conditions;
    unsigned int conditionCount;
    uint8_t address;
    uint8_t currentState;
    uint8_t defaultState;
};

class SignalControllerClass
{
public:
    void begin(SignalSet *signalSets, unsigned int signalSetCount);
    int readDeviceState(uint8_t address, uint8_t* buffer, uint8_t quantity);
    void updateSignalSets(uint16_t motorState, uint16_t pointsState);

private:
    uint8_t calculateNewState(SignalSet *signalSet, uint16_t motorState, uint16_t pointsState);
    bool statesMatch(uint16_t expected, uint16_t actual);
    SignalSet *signalSets;
    unsigned int signalSetCount;
};

extern SignalControllerClass SignalController;
