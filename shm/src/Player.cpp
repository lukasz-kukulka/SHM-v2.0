#include "shm/inc/Player.hpp"

Player::Player(std::unique_ptr<Ship> ship, size_t money, size_t availableSpace)
    : ship_(std::move(ship)), money_(money), availableSpace_(availableSpace) 
{}

size_t Player::getSpeed() const {
    return ship_->getSpeed();
}
  
std::shared_ptr<Cargo> Player::getCargo(size_t index) const {
    return ship_->getCargo(index);
}

size_t Player::countAvailableSpace() const {
    return ship_->getCapacity() - 
            std::accumulate(ship_->getVectorCargo().cbegin(),
                    ship_->getVectorCargo().cend(),
                    static_cast<size_t>(0),
                    [](size_t sum, const auto& cargo) {
                        return sum += cargo->getAmount();
                    });
}