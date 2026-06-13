#ifndef ENEMY_DATA_H_
#define ENEMY_DATA_H_

#include <string>
#include <unordered_map>
#include <vector>

struct EnemyAbilityParams {
  std::unordered_map<std::string, std::string> params;

  int GetInt(const std::string& key, int default_val = 0) const {
    auto it = params.find(key);
    return (it != params.end()) ? std::stoi(it->second) : default_val;
  }

  bool GetBool(const std::string& key, bool default_val = false) const {
    auto it = params.find(key);
    return (it != params.end()) ? (it->second == "true") : default_val;
  }

  std::string GetString(const std::string& key,
                        const std::string& default_val = "") const {
    auto it = params.find(key);
    return (it != params.end()) ? it->second : default_val;
  }

  std::vector<int> GetPositions() const {
    std::vector<int> result;
    auto it = params.find("positions");
    if (it != params.end() && !it->second.empty()) {
      std::string s = it->second;
      size_t start = 0, end = 0;
      while ((end = s.find(',', start)) != std::string::npos) {
        result.push_back(std::stoi(s.substr(start, end - start)));
        start = end + 1;
      }
      if (start < s.length()) {
        result.push_back(std::stoi(s.substr(start)));
      }
    }
    return result;
  }
};

struct EnemyAsset {
  std::string type_id;
  std::string name_key;
  int health;
  int base_damage;
  std::string ability_type;
  EnemyAbilityParams ability_params;
  std::string weapon_vulnerability;
  std::string special_requirement;
  std::string special_requirement_key;
};

struct WeaponAsset {
  std::string id;
  std::string name_key;
  int base_damage;
  int enchanted_damage;
};

struct LocationItemAsset {
  int id;
  std::string name_key;
  std::string description_key;
  int correct_order;
  std::string hint_key;
};

struct LocationBlacksmithAsset {
  std::string title_key;
  std::string intro_key;
  std::string dialogue_key;
  std::string prompt_key;
  std::string your_sequence_key;
  std::string success_key;
  std::string fail_key;
};

struct LocationAsset {
  std::string id;
  std::string title_key;
  std::string entry_key;
  std::string entry2_key;
  std::string items_title_key;
  std::string press_start_key;
  std::string weapon_id;
  std::string enemy_id;
  LocationBlacksmithAsset blacksmith;
  std::vector<LocationItemAsset> items;
};

#endif
