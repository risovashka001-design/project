#include "AssetManager.h"

#include <fstream>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

AssetManager& AssetManager::GetInstance() {
  static AssetManager instance;
  return instance;
}

bool AssetManager::LoadAll(const std::string& assets_dir) {
  bool success = true;
  success &= LoadEnemies(assets_dir + "enemies.json");
  success &= LoadWeapons(assets_dir + "weapons.json");
  success &= LoadLocations(assets_dir + "locations.json");

  if (success) {
    std::cout << "AssetManager: Loaded " << enemies_.size() << " enemies, "
              << weapons_.size() << " weapons, " << locations_.size()
              << " locations" << std::endl;
  }
  return success;
}

bool AssetManager::LoadEnemies(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open: " << filename << std::endl;
    return false;
  }

  try {
    json data = json::parse(file);

    for (auto& [key, value] : data["enemies"].items()) {
      EnemyAsset enemy;
      enemy.type_id = key;
      enemy.name_key = value.value("name_key", "");
      enemy.health = value.value("health", 100);
      enemy.base_damage = value.value("base_damage", 10);
      enemy.ability_type = value.value("ability_type", "");
      enemy.weapon_vulnerability = value.value("weapon_vulnerability", "");
      enemy.special_requirement = value.value("special_requirement", "");
      enemy.special_requirement_key =
          value.value("special_requirement_key", "");

      if (value.contains("ability_params")) {
        for (auto& [pkey, pval] : value["ability_params"].items()) {
          if (pval.is_string()) {
            enemy.ability_params.params[pkey] = pval.get<std::string>();
          } else if (pval.is_number()) {
            enemy.ability_params.params[pkey] = std::to_string(pval.get<int>());
          } else if (pval.is_boolean()) {
            enemy.ability_params.params[pkey] =
                pval.get<bool>() ? "true" : "false";
          } else if (pval.is_array()) {
            std::string arr;
            for (size_t i = 0; i < pval.size(); ++i) {
              if (i > 0) arr += ",";
              arr += std::to_string(pval[i].get<int>());
            }
            enemy.ability_params.params[pkey] = arr;
          }
        }
      }

      enemies_[enemy.type_id] = enemy;
    }
    return true;
  } catch (const json::exception& e) {
    std::cerr << "JSON error in " << filename << ": " << e.what() << std::endl;
    return false;
  }
}

bool AssetManager::LoadWeapons(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open: " << filename << std::endl;
    return false;
  }

  try {
    json data = json::parse(file);

    for (auto& [key, value] : data["weapons"].items()) {
      WeaponAsset weapon;
      weapon.id = key;
      weapon.name_key = value.value("name_key", "");
      weapon.base_damage = value.value("base_damage", 10);
      weapon.enchanted_damage = value.value("enchanted_damage", 30);
      weapons_[weapon.id] = weapon;
    }
    return true;
  } catch (const json::exception& e) {
    std::cerr << "JSON error in " << filename << ": " << e.what() << std::endl;
    return false;
  }
}

bool AssetManager::LoadLocations(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open: " << filename << std::endl;
    return false;
  }

  try {
    json data = json::parse(file);

    for (auto& [key, value] : data["locations"].items()) {
      LocationAsset loc;
      loc.id = key;
      loc.title_key = value.value("title_key", "");
      loc.entry_key = value.value("entry_key", "");
      loc.entry2_key = value.value("entry2_key", "");
      loc.items_title_key = value.value("items_title_key", "");
      loc.press_start_key = value.value("press_start_key", "");
      loc.weapon_id = value.value("weapon_id", "");
      loc.enemy_id = value.value("enemy_id", "");

      if (value.contains("blacksmith")) {
        auto& bs = value["blacksmith"];
        loc.blacksmith.title_key = bs.value("title_key", "");
        loc.blacksmith.intro_key = bs.value("intro_key", "");
        loc.blacksmith.dialogue_key = bs.value("dialogue_key", "");
        loc.blacksmith.prompt_key =
            bs.value("prompt_key", "blacksmith_chilling_prompt");
        loc.blacksmith.your_sequence_key =
            bs.value("your_sequence_key", "blacksmith_chilling_your_sequence");
        loc.blacksmith.success_key = bs.value("success_key", "");
        loc.blacksmith.fail_key = bs.value("fail_key", "");
      }

      if (value.contains("items")) {
        for (auto& item : value["items"]) {
          LocationItemAsset it;
          it.id = item.value("id", 0);
          it.name_key = item.value("name_key", "");
          it.description_key = item.value("description_key", "");
          it.correct_order = item.value("correct_order", 0);
          it.hint_key = item.value("hint_key", "");
          loc.items.push_back(it);
        }
      }

      locations_[loc.id] = loc;
    }
    return true;
  } catch (const json::exception& e) {
    std::cerr << "JSON error in " << filename << ": " << e.what() << std::endl;
    return false;
  }
}

const EnemyAsset& AssetManager::GetEnemy(const std::string& type_id) const {
  auto it = enemies_.find(type_id);
  if (it == enemies_.end()) {
    throw std::runtime_error("Enemy not found: " + type_id);
  }
  return it->second;
}

const WeaponAsset& AssetManager::GetWeapon(const std::string& id) const {
  auto it = weapons_.find(id);
  if (it == weapons_.end()) {
    throw std::runtime_error("Weapon not found: " + id);
  }
  return it->second;
}

const LocationAsset& AssetManager::GetLocation(const std::string& id) const {
  auto it = locations_.find(id);
  if (it == locations_.end()) {
    throw std::runtime_error("Location not found: " + id);
  }
  return it->second;
}

bool AssetManager::HasEnemy(const std::string& type_id) const {
  return enemies_.find(type_id) != enemies_.end();
}

bool AssetManager::HasWeapon(const std::string& id) const {
  return weapons_.find(id) != weapons_.end();
}

bool AssetManager::HasLocation(const std::string& id) const {
  return locations_.find(id) != locations_.end();
}

std::vector<std::string> AssetManager::GetAllEnemyIds() const {
  std::vector<std::string> ids;
  for (const auto& pair : enemies_) ids.push_back(pair.first);
  return ids;
}

std::vector<std::string> AssetManager::GetAllLocationIds() const {
  std::vector<std::string> ids;
  for (const auto& pair : locations_) ids.push_back(pair.first);
  return ids;
}
