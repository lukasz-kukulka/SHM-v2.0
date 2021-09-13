#pragma once

#include "Island.hpp"

#include <vector>

namespace {
    constexpr size_t COORDINATE_MIN = 0;
    constexpr size_t COORDINATE_MAX = 99;
    constexpr size_t ISLANDS_COUNT = 10;
    constexpr char DEFAULT_FILE_NAME[] { "settings/items.txt" };
}

class Map {
public:
    Map(Time* time);
    Island* getIsland(const Island::Coordinates&);
    std::vector<Island> getIslands() { return islands_; }
   
private:
    std::vector<Island> islands_;
    void countRecordsInFile(size_t &counter);
    size_t storeSize_{};
};
