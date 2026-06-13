#ifndef COMBAT_H_
#define COMBAT_H_

#include <string>
#include <unordered_map>

#include "EnemyData.h"
#include "Types.h"

struct EnemyStats {
  std::string name;
  Health health;
  Health max_health;
  Damage base_damage;
};

class Enemy {
 public:
  Enemy();
  Enemy(const std::string& type_id, const std::string& name, Health health,
        Damage damage);

  void LoadFromAsset(const EnemyAsset& asset);

  const std::string& GetName() const { return stats_.name; }
  Health GetHealth() const { return stats_.health; }
  Health GetMaxHealth() const { return stats_.max_health; }
  bool IsAlive() const { return stats_.health > 0; }
  const std::string& GetTypeId() const { return type_id_; }

  void SetTypeId(const std::string& id) { type_id_ = id; }
  void SetAbilityType(const std::string& type) { ability_type_ = type; }
  void SetAbilityParams(
      const std::unordered_map<std::string, std::string>& params) {
    ability_params_ = params;
  }

  void TakeDamage(Damage damage);
  Damage GetAttackDamage() const;

  void UpdateAbility();
  void PrepareAbility();
  void ResetAbility();
  std::string GetAbilityWarning() const;
  bool IsAbilityActive() const;

  int GetCopyCount() const;
  void AddCopy();
  void ResetCopies();

  bool IsStaffVulnerable() const;
  Damage GetModifiedDamage(Damage damage) const;
  int GetInvisiblePosition() const;
  bool ProcessThrowAction(int& player_damage, Damage weapon_damage);

 private:
  std::string type_id_;
  EnemyStats stats_;

  std::string ability_type_;
  std::unordered_map<std::string, std::string> ability_params_;

  bool invisible_;
  int invisible_position_;

  bool enraged_;
  bool preparing_;
  int copy_count_;
  int turn_counter_shadow_;

  bool ethereal_;
  bool staff_vulnerable_;
  int turn_counter_ghost_;

  bool empowered_;

  int GetIntParam(const std::string& key, int default_val = 0) const;
  bool GetBoolParam(const std::string& key, bool default_val = false) const;
  std::string GetStringParam(const std::string& key,
                             const std::string& default_val = "") const;
  std::vector<int> GetPositions() const;
};

class Combat {
 public:
  static bool StartFight(Enemy& enemy, const std::string& weapon_name,
                         Damage weapon_damage, bool enchanted);
  static bool StartAzazelFight(Enemy& enemy, bool has_all_enchanted);
};

#endif
