#ifndef PLAYER_H_
#define PLAYER_H_

#include "Types.h"

class Player {
 public:
  static Player& GetInstance();

  void Reset();            
  void ResetAllWeapons();  
  void TakeDamage(Health damage);
  void Heal(Health amount);

  bool IsAlive() const { return health_ > 0; }
  Health GetHealth() const { return health_; }
  Health GetMaxHealth() const { return max_health_; }

  Inventory& GetInventory() { return inventory_; }
  const Inventory& GetInventory() const { return inventory_; }

  void SetKnivesEnchanted(bool enchanted);
  void SetBowEnchanted(bool enchanted);
  void SetStaffEnchanted(bool enchanted);

 private:
  Player();

  Health health_;
  Health max_health_;
  Inventory inventory_;
};

#endif
