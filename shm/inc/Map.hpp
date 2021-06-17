#pragma once

#include "Island.hpp"
#include <vector>

#include <memory>
#include <vector>

class Map {
public:
    Map();

private:
    std::vector<Island> vectorOfIslands_;
    std::unique_ptr<Island> currentPosition_;
};
