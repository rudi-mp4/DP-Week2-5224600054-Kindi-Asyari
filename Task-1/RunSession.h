#ifndef RUNSESSION_H
#define RUNSESSION_H

#include <iostream>
#include "IInputGenerator.h"
#include "IScoringRule.h"
#include "IRewardRule.h"
#include "ShopSystem.h"

class RunSession {
private:
    IInputGenerator& inputGenerator;
    IScoringRule& scoringRule;
    IRewardRule& rewardRule;
    ShopSystem& shop;
    int money;
    int totalRounds;

public:
    RunSession(IInputGenerator& ig, IScoringRule& sr, IRewardRule& rr, ShopSystem& sh, int rounds = 3)
        : inputGenerator(ig), scoringRule(sr), rewardRule(rr), shop(sh), money(0), totalRounds(rounds) {}

    void run() {
        std::cout << "=== RUN START ===" << std::endl;
        std::cout << std::endl;

        for (int round = 1; round <= totalRounds; ++round) {
            std::cout << "Round " << round << std::endl;

            // 1. Generate input
            TurnInput input = inputGenerator.generate();
            std::cout << "[PLAY] input generated: " << input.value << std::endl;

            // 2. Compute base score
            int baseScore = scoringRule.computeScore(input);
            std::cout << "[SCORE] base score: " << baseScore << std::endl;

            // 3. Compute reward
            int reward = rewardRule.computeReward(baseScore);

            // 4. Update money
            money += reward;
            std::cout << "[REWARD] gain: " << reward << " | money: " << money << std::endl;

            // 5. Shop phase
            shop.offer(money);

            // 6. Advance round
            std::cout << std::endl;
        }

        std::cout << "=== RUN END ===" << std::endl;
        std::cout << "Final money: " << money << std::endl;
    }
};

#endif
