#ifndef COMBAT_H_
#define COMBAT_H_

#include <functional>
#include <string>

#include "Types.h"

struct EnemyStats {
  std::string name;
  Health health;
  Health max_health;
  Damage base_damage;
};

enum class EnemyType { Mage, Shadow, Ghost, Azazel };

class Enemy {
 public:
  Enemy(EnemyType type, const std::string& name, Health health, Damage damage);

  const std::string& GetName() const { return stats_.name; }
  Health GetHealth() const { return stats_.health; }
  Health GetMaxHealth() const { return stats_.max_health; }
  bool IsAlive() const { return stats_.health > 0; }
  EnemyType GetType() const { return type_; }

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
  EnemyType type_;
  EnemyStats stats_;

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
};

class Combat {
 public:
  static bool StartFight(Enemy& enemy, const std::string& weapon_name,
                         Damage weapon_damage, bool enchanted);
  static bool StartAzazelFight(Enemy& enemy, bool has_all_enchanted);
};

#endif
