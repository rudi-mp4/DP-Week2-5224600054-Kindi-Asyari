#ifndef IINPUTGENERATOR_H
#define IINPUTGENERATOR_H

#include <cstdlib>
#include <ctime>

struct TurnInput {
    int value;
};

class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput generate() = 0;
};

class FixedInputGenerator : public IInputGenerator {
public:
    TurnInput generate() override {
        TurnInput input;
        input.value = 5;
        return input;
    }
};

class RandomInputGenerator : public IInputGenerator {
public:
    RandomInputGenerator() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    TurnInput generate() override {
        TurnInput input;
        input.value = (std::rand() % 10) + 1;
        return input;
    }
};

#endif
