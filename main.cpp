#include "Game.hpp"

constexpr size_t start_money = 10'000;
constexpr size_t game_days = 100;
constexpr size_t final_goal = 20'000;

int main() {
    Game game(start_money, game_days, final_goal);
    game.startGame();

    return 0;
}
