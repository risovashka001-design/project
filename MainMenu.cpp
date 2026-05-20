#include "MainMenu.h"

#include <iostream>

#include "LocationMenu.h"
#include "Player.h"
#include "Screen.h"
#include "TextManager.h"

using namespace std;

void MainMenu::Run() {
  while (true) {
    Screen::Clear();
    Screen::DrawBorder(TextManager::GetInstance().Get("main_menu_title"));
    cout << "|  1. " << TextManager::GetInstance().Get("main_menu_new_game")
         << "\n";
    cout << "|  2. " << TextManager::GetInstance().Get("main_menu_exit")
         << "\n";
    cout << "+----------------------------------------------------------+\n";
    cout << "\n  " << TextManager::GetInstance().Get("main_menu_press_enter")
         << " ";

    int choice;
    cin >> choice;
    if (choice == 1)
      StartNewGame();
    else if (choice == 2) {
      ExitGame();
      return;
    } else {
      cout << "\n  " << TextManager::GetInstance().Get("main_menu_error")
           << "\n";
      Screen::WaitForEnter();
    }
  }
}

void MainMenu::StartNewGame() {
  Player::GetInstance().ResetAllWeapons();

  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get("new_game_title"));
  cout << "  " << TextManager::GetInstance().Get("new_game_line1") << "\n";
  cout << "  " << TextManager::GetInstance().Get("new_game_line2") << "\n";
  cout << "  " << TextManager::GetInstance().Get("new_game_line3") << "\n";
  cout << "  " << TextManager::GetInstance().Get("new_game_line4") << "\n";
  cout << "  " << TextManager::GetInstance().Get("new_game_line5") << "\n";
  cout << "  " << TextManager::GetInstance().Get("new_game_line6") << "\n";
  Screen::WaitForEnter(TextManager::GetInstance().Get("new_game_continue"));

  LocationMenu menu;
  menu.Run();
}

void MainMenu::ExitGame() {
  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get("exit_title"));
  cout << "  " << TextManager::GetInstance().Get("exit_subtitle") << "\n";
  Screen::WaitForEnter();
}
