#ifndef COMBAT_H_
#define COMBAT_H_

#include <string>

#include "Types.h"

class Enemy {
 public:
  Enemy(const std::string& name, Health health, Damage damage);
  virtual ~Enemy() = default;

  const std::string& GetName() const { return name_; }
  Health GetHealth() const { return health_; }
  Health GetMaxHealth() const { return max_health_; }
  bool IsAlive() const { return health_ > 0; }

  void TakeDamage(Damage damage);
  virtual Damage GetAttackDamage() const;
  virtual void SpecialAbility() = 0;
  virtual bool IsAbilityActive() const { return false; }
  virtual void ResetAbility() {}
  virtual std::string GetAbilityWarning() const { return ""; }
  virtual void PrepareAbility() {}  

 protected:
  std::string name_;
  Health health_;
  Health max_health_;
  Damage base_damage_;
};

class Mage : public Enemy {
 public:
  Mage();
  void SpecialAbility() override;
  bool IsAbilityActive() const override { return invisible_; }
  void ResetAbility() override { invisible_ = false; }
  std::string GetAbilityWarning() const override;
  int GetInvisiblePosition() const { return invisible_position_; }

 private:
  bool invisible_;
  int invisible_position_;
};

class Shadow : public Enemy {
 public:
  Shadow();
  void SpecialAbility() override;
  void PrepareAbility() override;
  bool IsAbilityActive() const override { return enraged_; }
  void ResetAbility() override { enraged_ = false; }
  std::string GetAbilityWarning() const override;
  int GetCopyCount() const { return copy_count_; }
  void AddCopy() { copy_count_++; }
  void ResetCopies() { copy_count_ = 0; }
  void IncrementTurnCounter() { turn_counter_++; }
  void ResetTurnCounter() { turn_counter_ = 0; }
  bool IsPreparing() const { return preparing_; }
  void ResetPreparing() { preparing_ = false; }

 private:
  bool enraged_;
  bool preparing_;
  int copy_count_;
  int turn_counter_;
};

class Ghost : public Enemy {
 public:
  Ghost();
  void SpecialAbility() override;
  bool IsAbilityActive() const override { return ethereal_; }
  void ResetAbility() override {
    ethereal_ = false;
    staff_vulnerable_ = false;
  }
  std::string GetAbilityWarning() const override;
  bool IsStaffVulnerable() const { return ethereal_ && staff_vulnerable_; }
  void DeactivateAbility() {
    ethereal_ = false;
    staff_vulnerable_ = false;
  }
  void IncrementTurnCounter() { turn_counter_++; }
  void ResetTurnCounter() { turn_counter_ = 0; }
  int GetTurnCounter() const { return turn_counter_; }

 private:
  bool ethereal_;
  bool staff_vulnerable_;
  int turn_counter_;
};

class Azazel : public Enemy {
 public:
  Azazel();
  void SpecialAbility() override;
  bool IsAbilityActive() const override { return empowered_; }
  void ResetAbility() override { empowered_ = false; }
  std::string GetAbilityWarning() const override;
  Damage GetModifiedDamage(Damage damage) const;

 private:
  bool empowered_;
};

class Combat {
 public:
  static bool StartFight(Enemy& enemy, const std::string& weapon_name,
                         Damage weapon_damage, bool enchanted);
  static bool StartAzazelFight(Azazel& enemy, bool has_all_enchanted);
};

#endif
