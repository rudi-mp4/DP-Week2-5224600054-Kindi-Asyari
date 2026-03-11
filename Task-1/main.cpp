#include "RunSession.h"

int main() {
    RandomInputGenerator inputGen;
    BasicScoringRule scoringRule;
    BasicRewardRule rewardRule;
    ShopSystem shop;

    RunSession session(inputGen, scoringRule, rewardRule, shop);
    session.run();

    return 0;
}
