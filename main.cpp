#include <clocale>

#include "Game.h"

int main() {
  std::setlocale(LC_ALL, "ru_RU.UTF-8");
  Game game;
  game.Run();
  return 0;
}
