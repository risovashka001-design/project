#include "Game.h"

#include <iostream>

#include "AssetManager.h"
#include "Intro.h"
#include "MainMenu.h"
#include "TextManager.h"

void Game::Run() {
  if (!TextManager::GetInstance().Load("data/texts.txt")) return;

  if (!AssetManager::GetInstance().LoadAll("assets/")) {
    std::cerr << "Failed to load game assets!" << std::endl;
    return;
  }

  Intro intro;
  intro.Show();
  MainMenu menu;
  menu.Run();
}
