#ifndef IREWARDRULE_H
#define IREWARDRULE_H

class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int computeReward(int baseScore) = 0;
};

class BasicRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore) override {
        return baseScore;
    }
};

#endif
