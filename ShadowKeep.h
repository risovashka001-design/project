#ifndef SHADOW_KEEP_H_
#define SHADOW_KEEP_H_

#include "Combat.h"
#include "Location.h"

class ShadowKeep : public Location {
 public:
  ShadowKeep();
  void Play() override;
  bool IsCompleted() const override { return completed_; }

 private:
  void ShowWeaponsStatus();
  void BattleWithAzazel();
  void ShowVictory();
  void ShowDefeat();
  void ShowEndGameMenu();  

  bool completed_;
};

#endif
