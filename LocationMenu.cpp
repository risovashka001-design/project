#include "LocationMenu.h"

#include <iostream>

#include "BoundlessTowers.h"
#include "ChillingTaverns.h"
#include "ElizabethCradle.h"
#include "MainMenu.h"
#include "Player.h"
#include "Screen.h"
#include "ShadowKeep.h"
#include "TextManager.h"

using namespace std;

void LocationMenu::Run() {
  ChillingTaverns chilling;
  BoundlessTowers boundless;
  ElizabethCradle elizabeth;
  ShadowKeep shadow;

  while (true) {
    Screen::Clear();
    Screen::DrawBorder(TextManager::GetInstance().Get("location_menu_title"));

    cout << "|  1. " << TextManager::GetInstance().Get("location_menu_1")
         << "\n";
    cout << "|  2. " << TextManager::GetInstance().Get("location_menu_2")
         << "\n";
    cout << "|  3. " << TextManager::GetInstance().Get("location_menu_3")
         << "\n";
    cout << "|  4. " << TextManager::GetInstance().Get("location_menu_4")
         << "\n";
    cout << "|  0. " << TextManager::GetInstance().Get("location_menu_back")
         << "\n";
    cout << "+----------------------------------------------------------+\n";

    const auto& inv = Player::GetInstance().GetInventory();
    int enchanted_count = inv.EnchantedCount();
    bool all_enchanted = (enchanted_count >= 3);

    cout << "\n  " << TextManager::GetInstance().Get("progress_title") << "\n";
    cout << "  " << TextManager::GetInstance().Get("progress_chilling") << " "
         << (chilling.IsCompleted()
                 ? TextManager::GetInstance().Get("location_completed")
                 : TextManager::GetInstance().Get("progress_not_completed"))
         << "\n";
    cout << "  " << TextManager::GetInstance().Get("progress_boundless") << " "
         << (boundless.IsCompleted()
                 ? TextManager::GetInstance().Get("location_completed")
                 : TextManager::GetInstance().Get("progress_not_completed"))
         << "\n";
    cout << "  " << TextManager::GetInstance().Get("progress_elizabeth") << " "
         << (elizabeth.IsCompleted()
                 ? TextManager::GetInstance().Get("location_completed")
                 : TextManager::GetInstance().Get("progress_not_completed"))
         << "\n";
    cout << "  " << TextManager::GetInstance().Get("progress_shadow") << " "
         << (all_enchanted
                 ? TextManager::GetInstance().Get("location_open")
                 : TextManager::GetInstance().Get("progress_not_completed"))
         << "\n";

    cout << "\n  " << TextManager::GetInstance().Get("shadow_weapon_count")
         << ": " << enchanted_count << "/3\n";
    cout << "    " << TextManager::GetInstance().Get("shadow_weapon_knives")
         << ": "
         << (inv.knives.enchanted
                 ? TextManager::GetInstance().Get("enchanted_status")
                 : TextManager::GetInstance().Get("normal_status"))
         << "\n";
    cout << "    " << TextManager::GetInstance().Get("shadow_weapon_bow")
         << ": "
         << (inv.bow.enchanted
                 ? TextManager::GetInstance().Get("enchanted_status")
                 : TextManager::GetInstance().Get("normal_status"))
         << "\n";
    cout << "    " << TextManager::GetInstance().Get("shadow_weapon_staff")
         << ": "
         << (inv.staff.enchanted
                 ? TextManager::GetInstance().Get("enchanted_status")
                 : TextManager::GetInstance().Get("normal_status"))
         << "\n";

    cout << "\n  " << TextManager::GetInstance().Get("main_menu_press_enter")
         << " ";
    int choice;
    cin >> choice;

    if (choice == 0)
      return;
    else if (choice == 1)
      chilling.Play();
    else if (choice == 2)
      boundless.Play();
    else if (choice == 3)
      elizabeth.Play();
    else if (choice == 4) {
      if (all_enchanted) {
        shadow.Play();
      } else {
        cout << "\n  " << TextManager::GetInstance().Get("location_locked")
             << "\n";
        Screen::WaitForEnter();
      }
    } else {
      cout << "\n  " << TextManager::GetInstance().Get("main_menu_error")
           << "\n";
      Screen::WaitForEnter();
    }

    if (!Player::GetInstance().IsAlive()) {
      cout << "\n  " << TextManager::GetInstance().Get("player_death_restart")
           << "\n";
      Screen::WaitForEnter();
      Player::GetInstance().ResetAllWeapons();
      MainMenu menu;
      menu.Run();
      return;
    }
  }
}
