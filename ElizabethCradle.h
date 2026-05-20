#ifndef ELIZABETH_CRADLE_H_
#define ELIZABETH_CRADLE_H_

#include <string>
#include <vector>

#include "Location.h"

class ElizabethCradle : public Location {
 public:
  ElizabethCradle();
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
  void BattleWithGhost();
  std::string GetSequenceString() const;

  std::vector<Item> items_;
  std::vector<int> collected_order_;
  bool completed_;
  bool enchanted_;
  std::string weapon_name_;
  int next_order_;
};

#endif
