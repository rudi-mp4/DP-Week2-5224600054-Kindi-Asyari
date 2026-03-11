#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include <iostream>
#include <string>

class ShopSystem {
public:
    void offer(int& money) {
        std::string itemName = "Bonus(+2)";
        int cost = 2;

        std::cout << "[SHOP] offered: " << itemName << " cost " << cost << std::endl;

        if (money >= cost) {
            money -= cost;
            std::cout << "[SHOP] purchased: " << itemName << std::endl;
        } else {
            std::cout << "[SHOP] skipped" << std::endl;
        }
    }
};

#endif
