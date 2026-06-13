#ifndef ASSET_MANAGER_H_
#define ASSET_MANAGER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "EnemyData.h"

class AssetManager {
 public:
  static AssetManager& GetInstance();

  bool LoadAll(const std::string& assets_dir = "assets/");

  const EnemyAsset& GetEnemy(const std::string& type_id) const;
  const WeaponAsset& GetWeapon(const std::string& id) const;
  const LocationAsset& GetLocation(const std::string& id) const;

  bool HasEnemy(const std::string& type_id) const;
  bool HasWeapon(const std::string& id) const;
  bool HasLocation(const std::string& id) const;

  std::vector<std::string> GetAllEnemyIds() const;
  std::vector<std::string> GetAllLocationIds() const;

 private:
  AssetManager() = default;

  bool LoadEnemies(const std::string& filename);
  bool LoadWeapons(const std::string& filename);
  bool LoadLocations(const std::string& filename);

  std::unordered_map<std::string, EnemyAsset> enemies_;
  std::unordered_map<std::string, WeaponAsset> weapons_;
  std::unordered_map<std::string, LocationAsset> locations_;
};

#endif
