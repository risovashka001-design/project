#include "Combat.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Player.h"
#include "Screen.h"
#include "TextManager.h"

using namespace std;

Enemy::Enemy(const string& name, Health health, Damage damage)
    : name_(name), health_(health), max_health_(health), base_damage_(damage) {}

void Enemy::TakeDamage(Damage damage) {
  health_ -= damage;
  if (health_ < 0) health_ = 0;
}

Damage Enemy::GetAttackDamage() const { return base_damage_ + (rand() % 10); }

Mage::Mage()
    : Enemy(TextManager::GetInstance().Get("battle_mage_title"), 120, 10),
      invisible_(false),
      invisible_position_(0) {}

void Mage::SpecialAbility() {
  invisible_ = true;
  invisible_position_ = 1 + (rand() % 4);
}

string Mage::GetAbilityWarning() const {
  if (!invisible_) return "";
  return TextManager::GetInstance().Get("battle_mage_warning") + " " +
         TextManager::GetInstance().Get("combat_direction_" +
                                        to_string(invisible_position_));
}

Shadow::Shadow()
    : Enemy(TextManager::GetInstance().Get("battle_shadow_title"), 140, 12),
      enraged_(false),
      preparing_(false),
      copy_count_(0),
      turn_counter_(0) {}

void Shadow::SpecialAbility() {
  if (enraged_) {
    AddCopy();
  }
  enraged_ = false;
  preparing_ = false;
}

void Shadow::PrepareAbility() {
  turn_counter_++;
  if (turn_counter_ >= 2) {
    preparing_ = true;
    enraged_ = true;
    turn_counter_ = 0;
  }
}

string Shadow::GetAbilityWarning() const {
  if (preparing_) {
    return TextManager::GetInstance().Get("battle_shadow_warning");
  }
  if (enraged_ && !preparing_) {
    return TextManager::GetInstance().Get("battle_shadow_ability_warning");
  }
  return "";
}

Ghost::Ghost()
    : Enemy(TextManager::GetInstance().Get("battle_ghost_title"), 110, 10),
      ethereal_(false),
      staff_vulnerable_(false),
      turn_counter_(0) {}

void Ghost::SpecialAbility() {
  turn_counter_++;
  if (turn_counter_ % 2 == 0) {
    ethereal_ = true;
    staff_vulnerable_ = true;
  } else {
    ethereal_ = false;
    staff_vulnerable_ = false;
  }
}

string Ghost::GetAbilityWarning() const {
  if (!ethereal_) return "";
  return TextManager::GetInstance().Get("battle_ghost_vulnerable");
}

Azazel::Azazel()
    : Enemy(TextManager::GetInstance().Get("battle_azazel_title"), 250, 20),
      empowered_(false) {}

void Azazel::SpecialAbility() { empowered_ = (rand() % 100) < 35; }

string Azazel::GetAbilityWarning() const {
  if (!empowered_) return "";
  return TextManager::GetInstance().Get("battle_azazel_ability");
}

Damage Azazel::GetModifiedDamage(Damage damage) const {
  return empowered_ ? damage * 2 : damage;
}

static int ReadChoice() {
  int choice = 0;
  cin >> choice;
  return choice;
}

bool Combat::StartFight(Enemy& enemy, const string& weapon_name,
                        Damage weapon_damage, bool enchanted) {
  srand(static_cast<unsigned int>(time(nullptr)));
  auto& player = Player::GetInstance();
  player.Reset();

  enemy.ResetAbility();
  if (auto* shadow = dynamic_cast<Shadow*>(&enemy)) {
    shadow->ResetCopies();
    shadow->ResetTurnCounter();
    shadow->ResetPreparing();
  }
  if (auto* ghost = dynamic_cast<Ghost*>(&enemy)) {
    ghost->ResetTurnCounter();
  }

  while (player.IsAlive() && enemy.IsAlive()) {
    enemy.PrepareAbility();

    Screen::Clear();
    Screen::DrawBorder(enemy.GetName());

    cout << "  " << TextManager::GetInstance().Get("combat_player") << ": "
         << player.GetHealth() << "/" << player.GetMaxHealth() << "\n";
    cout << "  " << enemy.GetName() << ": " << enemy.GetHealth() << "/"
         << enemy.GetMaxHealth() << "\n";

    string warning = enemy.GetAbilityWarning();
    if (!warning.empty()) cout << "\n  " << warning << "\n";

    if (auto* shadow = dynamic_cast<Shadow*>(&enemy)) {
      if (shadow->GetCopyCount() > 0) {
        cout << "\n  " << TextManager::GetInstance().Get("shadow_copies_count")
             << ": " << shadow->GetCopyCount() << "/4\n";
        if (shadow->GetCopyCount() >= 4) {
          cout << "\n  "
               << TextManager::GetInstance().Get("shadow_copies_death") << "\n";
          Screen::WaitForEnter();
          Screen::Clear();
          Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
          cout << "  " << TextManager::GetInstance().Get("shadow_copies_defeat")
               << "\n";
          Screen::WaitForEnter();
          return false;
        }
      }
    }

    cout << "\n  " << TextManager::GetInstance().Get("combat_action_title")
         << "\n";
    cout << "    1. " << TextManager::GetInstance().Get("combat_action_1")
         << " (" << weapon_name << ")\n";
    cout << "    2. " << TextManager::GetInstance().Get("combat_action_2")
         << "\n";
    cout << "    3. " << TextManager::GetInstance().Get("combat_action_3")
         << "\n";
    cout << "    4. " << TextManager::GetInstance().Get("combat_action_4")
         << "\n";
    cout << "\n  " << TextManager::GetInstance().Get("main_menu_press_enter")
         << " ";

    int choice = ReadChoice();
    Damage player_damage = 0;
    bool defended = false;

    if (choice == 1) {
      player_damage = weapon_damage + (rand() % 10);
      cout << "\n  " << TextManager::GetInstance().Get("combat_you_attack")
           << " " << weapon_name << "!\n";
    } else if (choice == 2) {
      if (dynamic_cast<Mage*>(&enemy)) {
        auto* mage = dynamic_cast<Mage*>(&enemy);
        cout << "\n  " << TextManager::GetInstance().Get("combat_throw")
             << "\n";
        cout << "  1. " << TextManager::GetInstance().Get("combat_direction_1")
             << "\n";
        cout << "  2. " << TextManager::GetInstance().Get("combat_direction_2")
             << "\n";
        cout << "  3. " << TextManager::GetInstance().Get("combat_direction_3")
             << "\n";
        cout << "  4. " << TextManager::GetInstance().Get("combat_direction_4")
             << "\n";
        cout << "  " << TextManager::GetInstance().Get("main_menu_press_enter")
             << " ";
        int dir = ReadChoice();
        if (dir == mage->GetInvisiblePosition()) {
          player_damage = weapon_damage + 15;
          cout << "\n  " << TextManager::GetInstance().Get("combat_hit")
               << "\n";
        } else {
          cout << "\n  " << TextManager::GetInstance().Get("combat_miss")
               << "\n";
        }
      } else {
        player_damage = weapon_damage + (rand() % 5);
        cout << "\n  " << TextManager::GetInstance().Get("combat_you_attack")
             << "\n";
      }
    } else if (choice == 3) {
      defended = true;
      cout << "\n  " << TextManager::GetInstance().Get("combat_defend") << "\n";
    } else if (choice == 4) {
      player.Heal(20);
      cout << "\n  " << TextManager::GetInstance().Get("combat_heal") << " "
           << player.GetHealth() << " HP.\n";
    } else {
      cout << "\n  " << TextManager::GetInstance().Get("combat_invalid")
           << "\n";
    }

    auto* shadow = dynamic_cast<Shadow*>(&enemy);
    bool shadow_created_copy = false;

    if (shadow && shadow->IsAbilityActive()) {
      if (player_damage > 0 && !defended) {
        shadow->AddCopy();
        shadow_created_copy = true;
        cout << "\n  " << TextManager::GetInstance().Get("shadow_copy_created")
             << " " << shadow->GetCopyCount() << "!\n";
      }
      shadow->ResetAbility();
      shadow->ResetPreparing();
    }

    if (player_damage > 0) {
      auto* ghost = dynamic_cast<Ghost*>(&enemy);
      if (ghost && ghost->IsAbilityActive()) {
        if (!ghost->IsStaffVulnerable()) {
          cout << "\n  " << TextManager::GetInstance().Get("ghost_immune")
               << "\n";
          player_damage = 0;
        }
      }

      if (player_damage > 0) {
        enemy.TakeDamage(player_damage);
        cout << "\n  " << TextManager::GetInstance().Get("combat_you_damage")
             << " " << player_damage << " "
             << TextManager::GetInstance().Get("combat_damage") << "\n";
      }
    }

    if (!enemy.IsAlive()) break;

    if (shadow && shadow->GetCopyCount() >= 4) {
      Screen::Clear();
      Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
      cout << "\n  " << TextManager::GetInstance().Get("shadow_copies_death")
           << "\n";
      cout << "  " << TextManager::GetInstance().Get("shadow_copies_defeat")
           << "\n";
      Screen::WaitForEnter();
      return false;
    }

    enemy.SpecialAbility();

    Damage enemy_damage = enemy.GetAttackDamage();

    if (dynamic_cast<Ghost*>(&enemy)) {
      if (enemy.IsAbilityActive()) {
        cout << "\n  " << TextManager::GetInstance().Get("battle_ghost_ability")
             << "\n";
      }
    } else if (dynamic_cast<Mage*>(&enemy)) {
      if (enemy.IsAbilityActive()) {
        cout << "\n  " << enemy.GetAbilityWarning() << "\n";
      }
    } else if (dynamic_cast<Azazel*>(&enemy)) {
    }

    if (defended) {
      enemy_damage /= 2;
      cout << "\n  " << TextManager::GetInstance().Get("combat_defend_half")
           << "\n";
    }

    player.TakeDamage(enemy_damage);
    cout << "\n  " << TextManager::GetInstance().Get("combat_enemy_attack")
         << " " << enemy_damage << " "
         << TextManager::GetInstance().Get("combat_damage") << "\n";

    if (!player.IsAlive()) break;
    Screen::WaitForEnter();
  }

  Screen::Clear();
  if (player.IsAlive()) {
    Screen::DrawBorder(TextManager::GetInstance().Get("victory_title"));
    cout << "  " << TextManager::GetInstance().Get("battle_victory") << "\n";
    Screen::WaitForEnter();
    return true;
  }

  Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
  cout << "  " << TextManager::GetInstance().Get("battle_defeat") << "\n";
  Screen::WaitForEnter();
  return false;
}

bool Combat::StartAzazelFight(Azazel& enemy, bool has_all_enchanted) {
  if (!has_all_enchanted) {
    Screen::Clear();
    Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
    cout << "  " << TextManager::GetInstance().Get("azazel_weapons_required")
         << "\n";
    Screen::WaitForEnter();
    return false;
  }

  srand(static_cast<unsigned int>(time(nullptr)));
  auto& player = Player::GetInstance();
  player.Reset();

  while (player.IsAlive() && enemy.IsAlive()) {
    Screen::Clear();
    Screen::DrawBorder(enemy.GetName());

    const auto& inv = player.GetInventory();
    cout << "  " << TextManager::GetInstance().Get("combat_player") << ": "
         << player.GetHealth() << "/" << player.GetMaxHealth() << "\n";
    cout << "  " << enemy.GetName() << ": " << enemy.GetHealth() << "/"
         << enemy.GetMaxHealth() << "\n";

    string warning = enemy.GetAbilityWarning();
    if (!warning.empty()) cout << "\n  " << warning << "\n";

    cout << "\n  " << TextManager::GetInstance().Get("combat_action_title")
         << "\n";
    cout << "    1. "
         << TextManager::GetInstance().Get("battle_azazel_attack_knives")
         << " (" << inv.knives.name << ")\n";
    cout << "    2. "
         << TextManager::GetInstance().Get("battle_azazel_attack_bow") << " ("
         << inv.bow.name << ")\n";
    cout << "    3. "
         << TextManager::GetInstance().Get("battle_azazel_attack_staff") << " ("
         << inv.staff.name << ")\n";
    cout << "    4. "
         << TextManager::GetInstance().Get("battle_azazel_attack_combo")
         << "\n";
    cout << "    5. " << TextManager::GetInstance().Get("battle_azazel_defend")
         << "\n";
    cout << "    6. " << TextManager::GetInstance().Get("battle_azazel_heal")
         << "\n";
    cout << "\n  " << TextManager::GetInstance().Get("main_menu_press_enter")
         << " ";

    int choice = ReadChoice();
    Damage player_damage = 0;
    bool defended = false;

    if (choice == 1) {
      player_damage = inv.knives.base_damage + (rand() % 10);
      cout << "\n  " << TextManager::GetInstance().Get("combat_you_attack")
           << " " << inv.knives.name << "\n";
    } else if (choice == 2) {
      player_damage = inv.bow.base_damage + (rand() % 10);
      cout << "\n  " << TextManager::GetInstance().Get("combat_you_attack")
           << " " << inv.bow.name << "\n";
    } else if (choice == 3) {
      player_damage = inv.staff.base_damage + (rand() % 10);
      cout << "\n  " << TextManager::GetInstance().Get("combat_you_attack")
           << " " << inv.staff.name << "\n";
    } else if (choice == 4) {
      player_damage = inv.EnchantedCount() * 25 + (rand() % 20);
      cout << "\n  " << TextManager::GetInstance().Get("combat_combo") << "\n";
    } else if (choice == 5) {
      defended = true;
      cout << "\n  " << TextManager::GetInstance().Get("combat_defend") << "\n";
    } else if (choice == 6) {
      player.Heal(20);
      cout << "\n  " << TextManager::GetInstance().Get("combat_heal") << " "
           << player.GetHealth() << " HP.\n";
    } else {
      cout << "\n  " << TextManager::GetInstance().Get("combat_invalid")
           << "\n";
    }

    if (player_damage > 0) {
      if (inv.EnchantedCount() >= 3 && choice == 4) player_damage += 20;
      enemy.TakeDamage(enemy.GetModifiedDamage(player_damage));
      cout << "\n  " << TextManager::GetInstance().Get("combat_you_damage")
           << " " << player_damage << " "
           << TextManager::GetInstance().Get("combat_damage") << "\n";
    }

    if (!enemy.IsAlive()) break;

    enemy.SpecialAbility();
    Damage enemy_damage = enemy.GetAttackDamage();
    if (enemy.IsAbilityActive()) {
      enemy_damage = enemy.GetModifiedDamage(enemy_damage);
      cout << "\n  " << TextManager::GetInstance().Get("battle_azazel_ability")
           << "\n";
      enemy.ResetAbility();
    }

    if (defended) enemy_damage /= 2;
    player.TakeDamage(enemy_damage);

    cout << "\n  " << TextManager::GetInstance().Get("combat_enemy_attack")
         << " " << enemy_damage << " "
         << TextManager::GetInstance().Get("combat_damage") << "\n";

    if (!player.IsAlive()) break;
    Screen::WaitForEnter();
  }

  Screen::Clear();
  if (player.IsAlive()) {
    Screen::DrawBorder(TextManager::GetInstance().Get("victory_title"));
    cout << "  " << TextManager::GetInstance().Get("battle_azazel_victory")
         << "\n";
    Screen::WaitForEnter();
    return true;
  }

  Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
  cout << "  " << TextManager::GetInstance().Get("battle_azazel_defeat")
       << "\n";
  Screen::WaitForEnter();
  return false;
}
