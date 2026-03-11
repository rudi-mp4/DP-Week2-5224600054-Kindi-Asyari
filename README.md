# DP-Week2-5224600054-Kindi-Asyari

## Task-1

### Game Concept:
**Round-Based Scoring & Shop Game**

### Step 1 — Core Loop
Game ini adalah permainan scoring berbasis ronde dengan sistem shop.
Core loop-nya adalah:
1. Sistem menghasilkan nilai input acak (1–10)
2. Skor dasar dihitung dari input menggunakan aturan scoring
3. Reward dihitung dari skor dasar (dengan bonus +2)
4. Uang pemain di-update dengan reward
5. Di shop phase sistem menawarkan item. Jika pemain memiliki cukup uang, item dibeli secara otomatis
6. Lanjut ke ronde berikutnya
7. Ulangi sampai semua ronde selesai (default: 3 ronde)

Loop ini berulang setiap ronde sampai sesi berakhir, kemudian uang akhir ditampilkan.

### Step 2 — Invariant Identification

Urutan yang tidak boleh berubah:
1. Input generation
2. Score computation
3. Reward calculation
4. Money update
5. Shop phase
6. Round advance
Jika urutan ini berubah, logika gameplay akan rusak.

**Komponen yang Dibutuhkan**

Game harus selalu memiliki:
1. `IInputGenerator` — menghasilkan input setiap ronde
2. `IScoringRule` — menghitung skor dasar dari input
3. `IRewardRule` — menghitung reward dari skor dasar
4. `ShopSystem` — menangani fase penawaran/pembelian toko
5. `RunSession` — mengontrol loop dan menghubungkan semuanya
6. Struct `TurnInput` — membawa data antar fase

Tanpa komponen-komponen ini, loop tidak dapat berfungsi.

### Step 3 — Mutable Elements

Bagian-bagian yang bersifat mutable (dapat diubah):

1. **Input generation strategy**
   Dapat diubah dari acak (1–10) menjadi tetap, berbobot, atau berbasis input pengguna.
   Tidak memengaruhi struktur loop — `RunSession` hanya memanggil `generate()`.

2. **Scoring formula**
   Dapat diubah dari `return input.value` menjadi rumus yang lebih kompleks (misalnya, `input.value * multiplier`).
   Tetap terjadi setelah pembuatan input dan sebelum perhitungan reward.

3. **Reward formula**
   Dapat diubah dari `baseScore + 2` menjadi logika bonus lainnya (misalnya, bonus beruntun, diminishing returns).
   Tetap berada di dalam fase perhitungan reward.

4. **Shop behavior**
   Dapat menawarkan item berbeda, biaya bervariasi, atau beberapa item per ronde.
   Tidak mengubah urutan fase — fase toko selalu datang setelah pembaruan uang.

Bagian-bagian ini bersifat mutable(dapat berubah) karena mengubah output numerik/perilaku tetapi tidak mengubah urutan struktural.

### Step 4 — C++ Core Loop Skeleton

Berikut adalah kerangka OOP minimal saya yang mencerminkan invariant loop.

**Struktur file:**
```
IInputGenerator.h
IRewardRule.h
IScoringRule.h
ShopSystem.h
RunSession.h
main.cpp
```

**IInputGenerator.h**
```cpp
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
```

**IScoringRule.h**
```cpp
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
```

**IRewardRule.h**
```cpp
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

class BonusRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore) override {
        return baseScore + 2;
    }
};

#endif
```

**ShopSystem.h**
```cpp
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
```

**RunSession.h**
```cpp
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
        for (int round = 1; round <= totalRounds; ++round) {
            std::cout << "Round " << round << std::endl;
            TurnInput input = inputGenerator.generate();
            int baseScore = scoringRule.computeScore(input);
            int reward = rewardRule.computeReward(baseScore);
            money += reward;
            shop.offer(money);
        }
        std::cout << "=== RUN END ===" << std::endl;
        std::cout << "Final money: " << money << std::endl;
    }
};

#endif
```

**main.cpp**
```cpp
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
```

### Explanation of Architectural Discipline

1. `RunSession` hanya mengontrol urutan fase.
2. Perhitungan skor dikontrol `IScoringRule`.
3. Perhitungan rewar dikontrol `IRewardRule`.
4. Pembuatan input dikontrol `IInputGenerator`.
5. Shop behaviour dikontrol `ShopSystem`.
6. Urutan loop ditetapkan di dalam `RunSession::run()`.
7. Jika saya ingin mengubah rumus scoring, saya hanya perlu memodifikasi `IScoringRule`.
8. Jika saya ingin mengubah logika reward, saya hanya perlu memodifikasi `IRewardRule`.
9. Jika saya ingin strategi input yang berbeda (misalnya, nilai tetap/input manual untuk testing), saya hanya perlu merubah kode di `RandomInputGenerator` supaya bisa input manual — `RunSession` tidak berubah.

Invariant loop tetap stabil.

### Final Reflection

**1. What is the invariant structure in your program?**

Struktur invariant adalah urutan fase di dalam `RunSession::run()`: input generation → score computation → reward calculation → money update → shop phase → round advance. Urutan ini tidak pernah berubah. `RunSession` menetapkan urutan ini setiap ronde.

**2. Which parts are mutable?**

Bagian-bagian yang mutable adalah:
- input generation strategy (`RandomInputGenerator` dapat ditukar dengan implementasi baru lainnya).
- scoring formula (`BasicScoringRule` dapat diganti dengan aturan yang lebih kompleks).
- scoring formula (`BonusRewardRule` dapat diganti dengan aturan kustom).
- shop behavior (nama item, biaya, dan logika pembelian dapat berubah secara independen).

**3. When you replaced the InputGenerator, why didn't RunSession change?**

Karena `RunSession` bergantung pada interface abstrak `IInputGenerator`, bukan pada class tertentu. `RunSession` hanya memanggil `generate()`. Menukar `RandomInputGenerator` dengan `FixedInputGenerator` tidak memerlukan modifikasi pada `RunSession` sama sekali.

**4. What would happen if scoring logic was placed inside RunSession?**

Jika logika scoring berada di dalam `RunSession`, maka setiap kali kita ingin mengubah cara perhitungan skor, kita harus memodifikasi `RunSession` itu sendiri. Ini melanggar Single Responsibility Principle — `RunSession` akan bertanggung jawab atas kontrol loop dan perhitungan scoring sekaligus. Dengan mengontrol scoring ke `IScoringRule`, kita menjaga `RunSession` tetap fokus pada coreloop.