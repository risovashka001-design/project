#include "ShadowKeep.h"

#include <cstdlib>
#include <iostream>

#include "Player.h"
#include "Screen.h"
#include "TextManager.h"

using namespace std;

ShadowKeep::ShadowKeep() : completed_(false) {}

void ShadowKeep::Play() {
  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get("shadow_title"));
  cout << "  " << TextManager::GetInstance().Get("shadow_entry") << "\n";
  cout << "  " << TextManager::GetInstance().Get("shadow_entry2") << "\n";
  cout << "  " << TextManager::GetInstance().Get("shadow_entry3") << "\n\n";

  ShowWeaponsStatus();

  auto& player = Player::GetInstance();
  int enchanted_count = player.GetInventory().EnchantedCount();
  cout << "\n  " << TextManager::GetInstance().Get("shadow_weapon_count")
       << ": " << enchanted_count << "/3\n\n";

  if (enchanted_count >= 3) {
    cout << "  " << TextManager::GetInstance().Get("shadow_weapons_ready")
         << "\n";
    Screen::WaitForEnter();
    BattleWithAzazel();
  } else {
    cout << "  " << TextManager::GetInstance().Get("shadow_no_weapons") << "\n";
    ShowDefeat();
    Screen::WaitForEnter();
  }

  completed_ = true;
}

void ShadowKeep::ShowWeaponsStatus() {
  const auto& inv = Player::GetInstance().GetInventory();
  cout << "  +----------------------------------------------------------+\n";
  cout << "  | " << TextManager::GetInstance().Get("shadow_weapons_title")
       << "\n";
  cout << "  |   " << TextManager::GetInstance().Get("shadow_weapon_knives")
       << ": "
       << (inv.knives.enchanted
               ? TextManager::GetInstance().Get("enchanted_status")
               : TextManager::GetInstance().Get("normal_status"))
       << "\n";
  cout << "  |   " << TextManager::GetInstance().Get("shadow_weapon_bow")
       << ": "
       << (inv.bow.enchanted
               ? TextManager::GetInstance().Get("enchanted_status")
               : TextManager::GetInstance().Get("normal_status"))
       << "\n";
  cout << "  |   " << TextManager::GetInstance().Get("shadow_weapon_staff")
       << ": "
       << (inv.staff.enchanted
               ? TextManager::GetInstance().Get("enchanted_status")
               : TextManager::GetInstance().Get("normal_status"))
       << "\n";
  cout << "  +----------------------------------------------------------+\n";
}

void ShadowKeep::BattleWithAzazel() {
  Azazel azazel;
  auto& player = Player::GetInstance();
  bool has_all_enchanted = (player.GetInventory().EnchantedCount() >= 3);
  bool victory = Combat::StartAzazelFight(azazel, has_all_enchanted);
  if (victory) {
    ShowVictory();
    ShowEndGameMenu();
  } else {
    ShowDefeat();
  }
}

void ShadowKeep::ShowVictory() {
  Screen::Clear();
  cout << "\n  " << TextManager::GetInstance().Get("victory_border_top")
       << "\n";
  cout << "  " << TextManager::GetInstance().Get("victory_title_center")
       << "\n";
  cout << "  " << TextManager::GetInstance().Get("victory_border_mid") << "\n";
  cout << "  " << TextManager::GetInstance().Get("victory_text_1") << "\n";
  cout << "  " << TextManager::GetInstance().Get("victory_text_2") << "\n";
  cout << "  " << TextManager::GetInstance().Get("victory_border_bottom")
       << "\n";
  Screen::WaitForEnter();
}

void ShadowKeep::ShowDefeat() {
  Screen::Clear();
  cout << "\n  " << TextManager::GetInstance().Get("defeat_border_top") << "\n";
  cout << "  " << TextManager::GetInstance().Get("defeat_title_center") << "\n";
  cout << "  " << TextManager::GetInstance().Get("defeat_border_mid") << "\n";
  cout << "  " << TextManager::GetInstance().Get("defeat_text_1") << "\n";
  cout << "  " << TextManager::GetInstance().Get("defeat_text_2") << "\n";
  cout << "  " << TextManager::GetInstance().Get("defeat_border_bottom")
       << "\n";
  Screen::WaitForEnter();
}

void ShadowKeep::ShowEndGameMenu() {
  while (true) {
    Screen::Clear();
    cout << "\n  " << TextManager::GetInstance().Get("end_border_top") << "\n";
    cout << "  " << TextManager::GetInstance().Get("end_title") << "\n";
    cout << "  " << TextManager::GetInstance().Get("end_border_mid") << "\n";
    cout << "  " << TextManager::GetInstance().Get("end_option_1") << "\n";
    cout << "  " << TextManager::GetInstance().Get("end_option_2") << "\n";
    cout << "  " << TextManager::GetInstance().Get("end_border_bottom") << "\n";
    cout << "\n  " << TextManager::GetInstance().Get("main_menu_press_enter")
         << " ";

    int choice;
    cin >> choice;

    if (choice == 1) {
      Screen::Clear();
      cout << "\n  " << TextManager::GetInstance().Get("exit_border_top")
           << "\n";
      cout << "  " << TextManager::GetInstance().Get("exit_title") << "\n";
      cout << "  " << TextManager::GetInstance().Get("exit_border_mid") << "\n";
      cout << "  " << TextManager::GetInstance().Get("exit_text") << "\n";
      cout << "  " << TextManager::GetInstance().Get("exit_border_bottom")
           << "\n";
      Screen::WaitForEnter();
      exit(0);
    } else if (choice == 2) {
      Player::GetInstance().ResetAllWeapons();
      Screen::Clear();
      cout << "\n  " << TextManager::GetInstance().Get("restart_text") << "\n";
      Screen::WaitForEnter();
      return;
    } else {
      cout << "\n  " << TextManager::GetInstance().Get("main_menu_error")
           << "\n";
      Screen::WaitForEnter();
    }
  }
}
