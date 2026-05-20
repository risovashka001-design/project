#include "Player.h"

#include "TextManager.h"

Player::Player() : health_(100), max_health_(100) {
  inventory_.knives = {TextManager::GetInstance().Get("inventory_knives"),
                       false, 10};
  inventory_.bow = {TextManager::GetInstance().Get("inventory_bow"), false, 10};
  inventory_.staff = {TextManager::GetInstance().Get("inventory_staff"), false,
                      10};
}

Player& Player::GetInstance() {
  static Player instance;
  return instance;
}

void Player::Reset() { health_ = max_health_; }

void Player::ResetAllWeapons() {
  health_ = max_health_;
  inventory_.knives = {TextManager::GetInstance().Get("inventory_knives"),
                       false, 10};
  inventory_.bow = {TextManager::GetInstance().Get("inventory_bow"), false, 10};
  inventory_.staff = {TextManager::GetInstance().Get("inventory_staff"), false,
                      10};
}

void Player::TakeDamage(Health damage) {
  health_ -= damage;
  if (health_ < 0) health_ = 0;
}

void Player::Heal(Health amount) {
  health_ += amount;
  if (health_ > max_health_) health_ = max_health_;
}

void Player::SetKnivesEnchanted(bool enchanted) {
  inventory_.knives.enchanted = enchanted;
  inventory_.knives.base_damage = enchanted ? 30 : 10;
}

void Player::SetBowEnchanted(bool enchanted) {
  inventory_.bow.enchanted = enchanted;
  inventory_.bow.base_damage = enchanted ? 30 : 10;
}

void Player::SetStaffEnchanted(bool enchanted) {
  inventory_.staff.enchanted = enchanted;
  inventory_.staff.base_damage = enchanted ? 30 : 10;
}
