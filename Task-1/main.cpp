#include "RunSession.h"

int main() {
    RandomInputGenerator inputGen;
    BasicScoringRule scoringRule;
    BonusRewardRule rewardRule;
    ShopSystem shop;

    RunSession session(inputGen, scoringRule, rewardRule, shop);
    session.run();

    return 0;
}
