#pragma once

#include "Fruit.hpp"

namespace {
    constexpr const size_t DAYS_TO_ROTTEN_DRY_FRUIT { 10 };
    constexpr const size_t MULTIPLIER_OF_THE_PRICE_OF_FRUIT { 3 };
}


class DryFruit : public Fruit {
public:
    DryFruit(const std::string& name, size_t amount, size_t basePrice);
    
    // override form Cargo (via Fruit)
    std::size_t getPrice() const override;
    bool isExpired() const override;
    
    // override from Fruit
    DryFruit& operator--() override;
    size_t getRottenTimeForDryFruit() const { return rottenDryFruit_; }
    size_t getTimeElapsed() const override { return timeElapsed_; }

    // override from Subscriber (via Fruit & Cargo)
    void nextDay() override;

private:
    size_t rottenDryFruit_ { DAYS_TO_ROTTEN_DRY_FRUIT };
    size_t multiplierDryFruit_ { MULTIPLIER_OF_THE_PRICE_OF_FRUIT };
    
};
