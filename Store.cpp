#include "Store.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "Alcohol.hpp"
#include "Cargo.hpp"
#include "DryFruit.hpp"
#include "Fruit.hpp"
#include "Player.hpp"

Store::Store(const size_t & storeSize, Time* time) : time_(time)
{
    time_->addSubscriber(this);
    cargo_.reserve(storeSize);
    generateAllCargo();
}

std::ostream& operator<<(std::ostream& out, const Store& store){
    for (size_t i = 0; i < store.cargo_.size(); i++) {
        out << "|" << std::setfill('-') << std::setw (100) << "|\n";
        out << std::setfill(' ')<< std::setw (10) << "| ID: " << i + 1;
        out << std::setw (30) << " | CARGO NAME: " << store.cargo_[i]->getName();
        out << std::setw (10) << " | AMOUNT: " << store.cargo_[i]->getAmount();
        out << std::setw (10) << " | PRICE: " << store.cargo_[i]->getPrice() << " |\n";
    }
    out << "|" << std::setfill('-') << std::setw (100) << "|\n";
    return out;
}

Store::Response Store::buy(Cargo* cargo, size_t amount, Player* player) {
    if (!cargo) {
        std::cout << "-----------------------  ERROR CARGO -------------------------------" << std::endl;
        return Response::lack_of_cargo;
    }
    if (amount * cargo->getPrice() > player->getMoney()){
        std::cout << "-----------------------  ERROR MONEY -------------------------------" << std::endl;
        return Response::lack_of_money;
    }
    if (cargo->getAmount() < amount) {
        std::cout << "-----------------------  ERROR AMOUNT -------------------------------" << std::endl;
        return Response::lack_of_cargo;
    }
    if (player->getAvailableSpace() < amount) {
        std::cout << "-----------------------  ERROR SPACE -------------------------------" << std::endl;
        return Response::lack_of_space;
    }
    player->buy(cargo, amount);
    return Response::done;   
}

Store::Response Store::sell(Cargo* cargo, size_t amount, Player* player) {
    if (!cargo) {
        return Response::lack_of_cargo;
    }
    if (cargo->getAmount() + amount > STORE_CAPACITY) {
        return Response::lack_of_space;
    }
    player->sell(cargo, amount);
    return Response::done;
}

void Store::cargoFromShip(Cargo* cargo, size_t amount) {
    for (auto & searchCargo : cargo_) {
        if (searchCargo->getName() == cargo->getName()) {
            searchCargo->setAmount(searchCargo->getAmount() + amount);
        }
    }
}

void Store::cargoToShip(Cargo* cargo, size_t amount) {
    for (auto & searchCargo : cargo_) {
        if (searchCargo->getName() == cargo->getName()) {
            searchCargo->setAmount(searchCargo->getAmount() - amount);
        }
    }
}

Cargo* Store::getCargo(const std::string& name) const {
    auto result = std::find_if(cargo_.begin(), cargo_.end(),
                             [&name](const auto& cargo) {
                                return cargo->getName() == name ;
                             });
    
    return result != cargo_.end() ? result->get() : nullptr;
}

Store::CargoStorage::iterator Store::findCargoInStore(Cargo* cargo) {
    return std::find_if(begin(cargo_), end(cargo_),
                        [&cargo](const auto& unique) {
                            return unique.get() == cargo;
                        });
}

void Store::removeCargo(Cargo* cargo) {
    auto unique = std::find_if(begin(cargo_), end(cargo_),
                              [&cargo](const auto& unique) {
                                  return unique.get() == cargo;
                              });
    if (unique != cargo_.end()) {
        cargo_.erase(unique);
    }
}

void Store::addCargo(Cargo* cargo, size_t amount) {
    if (typeid(cargo) == typeid(Alcohol)) {
        Alcohol* alcohol = static_cast<Alcohol*>(cargo);
        cargo_.push_back(std::make_unique<Alcohol>(alcohol->getName(),
                                                   amount,
                                                   alcohol->getBasePrice(),
                                                   alcohol->getPercentage()));
    } else if (typeid(cargo) == typeid(Fruit)) {
        Fruit* fruit = static_cast<Fruit*>(cargo);
        cargo_.push_back(std::make_unique<Fruit>(fruit->getName(),
                                                 amount,
                                                 fruit->getBasePrice()));
    } else if (typeid(cargo) == typeid(DryFruit)) {
        DryFruit* fruit = static_cast<DryFruit*>(cargo);
        cargo_.push_back(std::make_unique<DryFruit>(fruit->getName(),
                                                    amount,
                                                    fruit->getBasePrice()));
    } else if (typeid(cargo) == typeid(Item)) {
        Item* item = static_cast<Item*>(cargo);
        cargo_.push_back(std::make_unique<Item>(item->getName(),
                                                amount,
                                                item->getBasePrice(),
                                                item->getRarity()));
    }
}

void Store::nextDay() {
    for (const auto& cargo : cargo_) {
        cargo->setAmount(randomGenerate(MIN_CARGO_IN_STORE, MAX_CARGO_IN_STORE));
    }
}

size_t Store::randomGenerate(size_t min, size_t max) {
    std::mt19937 gen(std::random_device{}()); 
    std::uniform_int_distribution<size_t> distribution(min, max);
    return distribution(gen);
}

Item::Rarity Store::rarityConversion(size_t numberForConversion) {
    size_t multiplyRarity = 4;
    size_t addRarity = 1;
    return static_cast<Item::Rarity>((numberForConversion * multiplyRarity) + addRarity);
}

void Store::generateAllCargo() {
    std::string line;
    std::fstream file;
    file.open("settings/items.txt", std::ios::in);
    if (file.fail()) {
        std::cout << "File not exist store\n";
    }
    while (getline(file, line)) {
        convertDataFromFile(line);
    }
    file.close();
}

void Store::convertDataFromFile(std::string lineFromFile) {
    std::string singleWordFromFile{};
    size_t wordNumber{};
    for (size_t i = 0; i <= lineFromFile.size(); i++) {
        if (lineFromFile[i] != ' ' && i < lineFromFile.size()) {
            singleWordFromFile += lineFromFile[i];
        } else {
            wordNumber++;
            valueOneLineAssignment(wordNumber, singleWordFromFile);
            singleWordFromFile = {};
            std::cout << "\n";
        }
    }
    generateSingleCargo();
}

void Store::valueOneLineAssignment(const size_t & wordNumber, const std::string & singleWordFromFile) {
    switch (wordNumber) {
        case 1 : {
            className_ = singleWordFromFile;
        } break;
        case 2 : {
            itemName_ = singleWordFromFile;
        } break;
        case 3 : {
            minAmount_ = std::stol(singleWordFromFile); 
        } break;
        case 4 : {
            maxAmount_ = std::stol(singleWordFromFile);
        } break;
        case 5 : {
            minPrice_ = std::stol(singleWordFromFile);
        } break;
        case 6 : {
            maxPrice_ = std::stol(singleWordFromFile);
        } break;
        case 7 : {
            minUniqueStat_ = std::stol(singleWordFromFile);
        } break;
        case 8 : {
            maxUniqueStat_= std::stol(singleWordFromFile);
        } break;
        default : {
        } break;
    }
}

void Store::generateSingleCargo() {
    
    if (className_ == "Fruit"){
        generateFruit();
    } else if (className_ == "Alcohol"){
        generateAlcohol();
    } else if (className_ == "Item"){
        generateItem();
    } else {
        generateDryFruits();
    }
}

void Store::generateFruit() {
    cargo_.push_back(std::make_unique<Fruit>(Fruit(itemName_, 
                        randomGenerate(minAmount_, maxAmount_), 
                        randomGenerate(minPrice_, maxPrice_))));
}

void Store::generateAlcohol() {
    cargo_.push_back(std::make_unique<Alcohol>(Alcohol(itemName_, 
                        randomGenerate(minAmount_, maxAmount_), 
                        randomGenerate(minPrice_, maxPrice_), minUniqueStat_)));
}

void Store::generateItem() {
    cargo_.push_back(std::make_unique<Item>(Item(itemName_, 
                        randomGenerate(minAmount_, maxAmount_), 
                        randomGenerate(minPrice_, maxPrice_), 
                        rarityConversion(randomGenerate(minUniqueStat_, maxUniqueStat_)))));
}

void Store::generateDryFruits() {
    cargo_.push_back(std::make_unique<DryFruit>(DryFruit(itemName_, 
                        randomGenerate(minAmount_, maxAmount_), 
                        randomGenerate(minPrice_, maxPrice_))));
}
