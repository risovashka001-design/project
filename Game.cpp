#include "Game.h"

#include "Intro.h"
#include "MainMenu.h"
#include "TextManager.h"

void Game::Run() {
  if (!TextManager::GetInstance().Load("data/texts.txt")) return;
  Intro intro;
  intro.Show();
  MainMenu menu;
  menu.Run();
}
