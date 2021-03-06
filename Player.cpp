#include "Player.hpp"

#include <numeric>
#include <iostream>

#include "Cargo.hpp"
#include "Map.hpp"

Player::Player(std::shared_ptr<Map> map, size_t money, size_t availableSpace, Time* time)
    : map_(map)
    , ship_(std::make_shared<Ship>(1, 25, 100, time, this))
    , money_(money)
{
    availableSpace_ = availableSpace;
    currentPosition_ = map_->getIsland(map_->getIslands()[0].getCoordinates());
}

size_t Player::getSpeed() const {
    return ship_->getSpeed();
}
  
Cargo* Player::getCargo(size_t index) const {
    if (ship_) {
        return ship_->getCargo(index);
    }
    return nullptr;
}

Island* Player::getCurrentPosition() const {
    return currentPosition_;
}

void Player::setCurrentPosition(Island* island) {
    currentPosition_ = island;
}

Cargo* Player::getCargo(const std::string& name) const {
    return ship_->getCargo(name);
}

void Player::countAvailableSpace() {
    availableSpace_ = ship_->getCapacity() - std::accumulate(ship_->getCargos()->cbegin(),
                                                             ship_->getCargos()->cend(),
                                                             static_cast<size_t>(0),
                                                             [](size_t sum, const auto& cargo) {
                                                                 return sum += cargo->getAmount();
                                                             });

}

void Player::payCrew(const size_t payCrew) {
    money_ <= payCrew 
        ? money_ = 0 
        : money_ -= payCrew;
}

void Player::setPlayerPtr() {
    ship_->changeDelegate(this);
}

void Player::buy(Cargo* cargo, size_t amount) {
    money_ -= amount * cargo->getPrice();
    ship_->load(cargo, amount);
    countAvailableSpace();
 }

void Player::sell(Cargo* cargo, size_t amount) {
    money_ += amount * cargo->getPrice();
    ship_->unload(cargo, amount);
    countAvailableSpace();
}