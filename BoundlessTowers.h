#ifndef BOUNDLESS_TOWERS_H_
#define BOUNDLESS_TOWERS_H_

#include <string>
#include <vector>

#include "Combat.h"  
#include "Location.h"

class BoundlessTowers : public Location {
 public:
  BoundlessTowers();
  void Play() override;
  bool IsCompleted() const override { return completed_; }

 private:
  struct Item {
    int id;
    std::string name;
    std::string description;
    bool taken;
    int correct_position;
  };

  void ShowRoomStatus();
  void ShowCornerInfo(int corner_index);
  bool TakeItem(int corner_index);
  void MeetBlacksmith();
  void BattleWithShadow();
  std::string GetSequenceString() const;

  std::vector<Item> items_;
  std::vector<int> collected_order_;
  bool completed_;
  bool enchanted_;
  std::string weapon_name_;
  int next_order_;
};

#endif
