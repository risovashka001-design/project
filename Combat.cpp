#include "Combat.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

#include "Player.h"
#include "Screen.h"
#include "TextManager.h"

using namespace std;

Enemy::Enemy()
    : stats_{"", 0, 0, 0},
      invisible_(false),
      invisible_position_(0),
      enraged_(false),
      preparing_(false),
      copy_count_(0),
      turn_counter_shadow_(0),
      ethereal_(false),
      staff_vulnerable_(false),
      turn_counter_ghost_(0),
      empowered_(false) {}

Enemy::Enemy(const string& type_id, const string& name, Health health,
             Damage damage)
    : type_id_(type_id),
      stats_{name, health, health, damage},
      invisible_(false),
      invisible_position_(0),
      enraged_(false),
      preparing_(false),
      copy_count_(0),
      turn_counter_shadow_(0),
      ethereal_(false),
      staff_vulnerable_(false),
      turn_counter_ghost_(0),
      empowered_(false) {}

void Enemy::LoadFromAsset(const EnemyAsset& asset) {
  type_id_ = asset.type_id;
  stats_.name = TextManager::GetInstance().Get(asset.name_key);
  stats_.health = asset.health;
  stats_.max_health = asset.health;
  stats_.base_damage = asset.base_damage;
  ability_type_ = asset.ability_type;
  ability_params_ = asset.ability_params.params;
}

int Enemy::GetIntParam(const string& key, int default_val) const {
  auto it = ability_params_.find(key);
  return (it != ability_params_.end()) ? stoi(it->second) : default_val;
}

bool Enemy::GetBoolParam(const string& key, bool default_val) const {
  auto it = ability_params_.find(key);
  return (it != ability_params_.end()) ? (it->second == "true") : default_val;
}

string Enemy::GetStringParam(const string& key,
                             const string& default_val) const {
  auto it = ability_params_.find(key);
  return (it != ability_params_.end()) ? it->second : default_val;
}

vector<int> Enemy::GetPositions() const {
  vector<int> result;
  auto it = ability_params_.find("positions");
  if (it != ability_params_.end() && !it->second.empty()) {
    string s = it->second;
    size_t start = 0, end = 0;
    while ((end = s.find(',', start)) != string::npos) {
      result.push_back(stoi(s.substr(start, end - start)));
      start = end + 1;
    }
    if (start < s.length()) result.push_back(stoi(s.substr(start)));
  }
  return result;
}

void Enemy::TakeDamage(Damage damage) {
  stats_.health -= damage;
  if (stats_.health < 0) stats_.health = 0;
}

Damage Enemy::GetAttackDamage() const {
  return stats_.base_damage + (rand() % 10);
}

void Enemy::UpdateAbility() {
  if (ability_type_ == "ethereal") {
    turn_counter_ghost_++;
    int cycle = GetIntParam("cycle_turns", 2);
    if (turn_counter_ghost_ % cycle == 0) {
      ethereal_ = true;
      staff_vulnerable_ = GetBoolParam("staff_vulnerable", true);
    } else {
      ethereal_ = false;
      staff_vulnerable_ = false;
    }
  } else if (ability_type_ == "empower") {
    int chance = GetIntParam("chance", 35);
    empowered_ = (rand() % 100) < chance;
  }
}

void Enemy::PrepareAbility() {
  if (ability_type_ == "copy_on_hit") {
    turn_counter_shadow_++;
    int prep_turns = GetIntParam("prep_turns", 2);
    if (turn_counter_shadow_ >= prep_turns) {
      preparing_ = true;
      enraged_ = true;
      turn_counter_shadow_ = 0;
    }
  }
}

void Enemy::ResetAbility() {
  if (ability_type_ == "invisibility") {
    invisible_ = false;
  } else if (ability_type_ == "copy_on_hit") {
    enraged_ = false;
    preparing_ = false;
  } else if (ability_type_ == "ethereal") {
    ethereal_ = false;
    staff_vulnerable_ = false;
  } else if (ability_type_ == "empower") {
    empowered_ = false;
  }
}

string Enemy::GetAbilityWarning() const {
  if (ability_type_ == "invisibility" && invisible_) {
    string warning_key = GetStringParam("warning_key", "battle_mage_warning");
    return TextManager::GetInstance().Get(warning_key) + " " +
           TextManager::GetInstance().Get("combat_direction_" +
                                          to_string(invisible_position_));
  } else if (ability_type_ == "copy_on_hit") {
    if (preparing_) {
      string key =
          GetStringParam("warning_prepare_key", "battle_shadow_warning");
      return TextManager::GetInstance().Get(key);
    }
    if (enraged_ && !preparing_) {
      string key =
          GetStringParam("warning_active_key", "battle_shadow_ability_warning");
      return TextManager::GetInstance().Get(key);
    }
  } else if (ability_type_ == "ethereal" && ethereal_) {
    string key = GetStringParam("vulnerable_key", "battle_ghost_vulnerable");
    return TextManager::GetInstance().Get(key);
  } else if (ability_type_ == "empower" && empowered_) {
    string key = GetStringParam("warning_key", "battle_azazel_ability");
    return TextManager::GetInstance().Get(key);
  }
  return "";
}

bool Enemy::IsAbilityActive() const {
  if (ability_type_ == "invisibility") return invisible_;
  if (ability_type_ == "copy_on_hit") return enraged_;
  if (ability_type_ == "ethereal") return ethereal_;
  if (ability_type_ == "empower") return empowered_;
  return false;
}

int Enemy::GetCopyCount() const {
  return (ability_type_ == "copy_on_hit") ? copy_count_ : 0;
}

void Enemy::AddCopy() {
  if (ability_type_ == "copy_on_hit") copy_count_++;
}

void Enemy::ResetCopies() {
  if (ability_type_ == "copy_on_hit") copy_count_ = 0;
}

bool Enemy::IsStaffVulnerable() const {
  return (ability_type_ == "ethereal") && ethereal_ && staff_vulnerable_;
}

Damage Enemy::GetModifiedDamage(Damage damage) const {
  if (ability_type_ == "empower" && empowered_) {
    int multiplier = GetIntParam("damage_multiplier", 2);
    return damage * multiplier;
  }
  return damage;
}

int Enemy::GetInvisiblePosition() const {
  return (ability_type_ == "invisibility") ? invisible_position_ : 0;
}

bool Enemy::ProcessThrowAction(int& player_damage, Damage weapon_damage) {
  if (ability_type_ != "invisibility" || !invisible_) return false;

  cout << "\n  " << TextManager::GetInstance().Get("combat_throw") << "\n";
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

  int dir;
  cin >> dir;

  if (dir == invisible_position_) {
    player_damage = weapon_damage + 15;
    cout << "\n  " << TextManager::GetInstance().Get("combat_hit") << "\n";
  } else {
    player_damage = 0;
    cout << "\n  " << TextManager::GetInstance().Get("combat_miss") << "\n";
  }
  return true;
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
  if (enemy.GetTypeId() == "shadow") {
    enemy.ResetCopies();
  }

  while (player.IsAlive() && enemy.IsAlive()) {
    enemy.PrepareAbility();
    enemy.UpdateAbility();

    Screen::Clear();
    Screen::DrawBorder(enemy.GetName());

    cout << "  " << TextManager::GetInstance().Get("combat_player") << ": "
         << player.GetHealth() << "/" << player.GetMaxHealth() << "\n";
    cout << "  " << enemy.GetName() << ": " << enemy.GetHealth() << "/"
         << enemy.GetMaxHealth() << "\n";

    string warning = enemy.GetAbilityWarning();
    if (!warning.empty()) cout << "\n  " << warning << "\n";

    if (enemy.GetTypeId() == "shadow" && enemy.GetCopyCount() > 0) {
      cout << "\n  " << TextManager::GetInstance().Get("shadow_copies_count")
           << ": " << enemy.GetCopyCount() << "/4\n";
      if (enemy.GetCopyCount() >= 4) {
        cout << "\n  " << TextManager::GetInstance().Get("shadow_copies_death")
             << "\n";
        Screen::WaitForEnter();
        Screen::Clear();
        Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
        cout << "  " << TextManager::GetInstance().Get("shadow_copies_defeat")
             << "\n";
        Screen::WaitForEnter();
        return false;
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
      if (enemy.GetTypeId() == "mage") {
        enemy.ProcessThrowAction(player_damage, weapon_damage);
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

    if (enemy.GetTypeId() == "shadow" && enemy.IsAbilityActive()) {
      if (player_damage > 0 && !defended) {
        enemy.AddCopy();
        cout << "\n  " << TextManager::GetInstance().Get("shadow_copy_created")
             << " " << enemy.GetCopyCount() << "!\n";
      }
      enemy.ResetAbility();
    }

    if (player_damage > 0) {
      if (enemy.GetTypeId() == "ghost" && enemy.IsAbilityActive()) {
        if (!enemy.IsStaffVulnerable()) {
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

    if (enemy.GetTypeId() == "shadow" && enemy.GetCopyCount() >= 4) {
      Screen::Clear();
      Screen::DrawBorder(TextManager::GetInstance().Get("defeat_title"));
      cout << "\n  " << TextManager::GetInstance().Get("shadow_copies_death")
           << "\n";
      cout << "  " << TextManager::GetInstance().Get("shadow_copies_defeat")
           << "\n";
      Screen::WaitForEnter();
      return false;
    }

    enemy.UpdateAbility();

    if (enemy.GetTypeId() == "ghost" && enemy.IsAbilityActive()) {
      cout << "\n  " << TextManager::GetInstance().Get("battle_ghost_ability")
           << "\n";
    } else if (enemy.GetTypeId() == "mage" && enemy.IsAbilityActive()) {
      cout << "\n  " << enemy.GetAbilityWarning() << "\n";
    }

    Damage enemy_damage = enemy.GetAttackDamage();
    enemy_damage = enemy.GetModifiedDamage(enemy_damage);

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

bool Combat::StartAzazelFight(Enemy& enemy, bool has_all_enchanted) {
  if (enemy.GetTypeId() != "azazel") return false;

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

    enemy.UpdateAbility();
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
