#ifndef ISCORINGRULE_H
#define ISCORINGRULE_H

#include "IInputGenerator.h"

class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int computeScore(const TurnInput& input) = 0;
};

class BasicScoringRule : public IScoringRule {
public:
    int computeScore(const TurnInput& input) override {
        return input.value;
    }
};

#endif
