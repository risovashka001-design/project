#ifndef TYPES_H_
#define TYPES_H_

#include <string>

using Health = int;
using Damage = int;

struct Weapon {
  std::string name;
  bool enchanted;
  Damage base_damage;
};

struct Inventory {
  Weapon knives;
  Weapon bow;
  Weapon staff;

  int EnchantedCount() const {
    int count = 0;
    if (knives.enchanted) ++count;
    if (bow.enchanted) ++count;
    if (staff.enchanted) ++count;
    return count;
  }
};

#endif
