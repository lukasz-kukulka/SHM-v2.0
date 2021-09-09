#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Alcohol.hpp"
#include "DryFruit.hpp"
#include "Fruit.hpp"
#include "Item.hpp"
#include "Subscriber.hpp"
#include "Time.hpp"

class Cargo;
class Delegate;
class Player;
class Store;

// class responsible for manage ship(s) in game
class Ship : public Subscriber {
public:
    Ship() = default;
    Ship(int id, const std::string& name, size_t speed, size_t maxCrew, size_t capacity, Time* time, Delegate* delegate = nullptr);
    Ship(int id, size_t speed, size_t maxCrew, Time* time, Delegate* delegate);
    ~Ship();

    Ship& operator+=(const size_t amount);
    Ship& operator-=(const size_t amount);
    friend std::ostream& operator<<(std::ostream& out, const Ship& ship);

    using CargoStorage = std::vector<std::unique_ptr<Cargo>>;

    int getId() const { return id_; };
    std::string getName() const { return name_; };
    size_t getSpeed() const { return speed_; };
    size_t getMaxCrew() const { return maxCrew_; };
    size_t getCrew() const { return crew_; };
    size_t getCapacity() const { return capacity_; };
    Cargo* getCargo(size_t index) const;
    Cargo* getCargo(const std::string& name) const;
    CargoStorage* getCargos() { return &cargo_; };
    void addCargo(Cargo* cargo, size_t amount);

    void setName(const std::string& name);
    void load(Cargo* cargo, size_t amount);
    void unload(Cargo* cargo, size_t amount);
    void changeDelegate(Player* player);

    // overload form Subscriber
    void nextDay() override;

private:
    const int id_ { -1 };
    std::string name_;
    size_t speed_ { 50 };
    const size_t maxCrew_ { 100 };
    const size_t capacity_ { 80 };
    Time* time_;
    Delegate* delegate_;
    CargoStorage cargo_;
    size_t crew_ { 50 };
    Store* store_;
};
