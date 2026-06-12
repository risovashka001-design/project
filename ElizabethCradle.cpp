#include "ElizabethCradle.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Combat.h"
#include "Player.h"
#include "Screen.h"
#include "TextManager.h"

using namespace std;

ElizabethCradle::ElizabethCradle()
    : completed_(false), enchanted_(false), next_order_(1) {}

void ElizabethCradle::Play() {
  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get("elizabeth_title"));
  cout << "  " << TextManager::GetInstance().Get("elizabeth_entry") << "\n";
  cout << "  " << TextManager::GetInstance().Get("elizabeth_entry2") << "\n\n";

  items_ = {
      {0, TextManager::GetInstance().Get("elizabeth_items_tear"),
       TextManager::GetInstance().Get("elizabeth_items_tear_desc"), false, 1},
      {0, TextManager::GetInstance().Get("elizabeth_items_lullaby"),
       TextManager::GetInstance().Get("elizabeth_items_lullaby_desc"), false,
       2},
      {0, TextManager::GetInstance().Get("elizabeth_items_mirror"),
       TextManager::GetInstance().Get("elizabeth_items_mirror_desc"), false, 3},
      {0, TextManager::GetInstance().Get("elizabeth_items_flower"),
       TextManager::GetInstance().Get("elizabeth_items_flower_desc"), false, 4},
  };

  vector<int> numbers = {1, 2, 3, 4};
  srand(static_cast<unsigned int>(time(nullptr)));
  for (int i = 3; i > 0; --i) {
    int j = rand() % (i + 1);
    swap(numbers[i], numbers[j]);
  }
  for (int i = 0; i < 4; ++i) items_[i].id = numbers[i];

  cout << "\n+----------------------------------------------------------+\n";
  cout << "| " << TextManager::GetInstance().Get("elizabeth_items_title")
       << "\n";
  for (const auto& item : items_) {
    cout << "|   " << item.id << " - " << item.name << "\n";
  }
  cout << "+----------------------------------------------------------+\n";

  Screen::WaitForEnter(TextManager::GetInstance().Get("elizabeth_press_start"));

  bool room_completed = false;
  while (!room_completed) {
    ShowRoomStatus();
    cout << "\n  " << TextManager::GetInstance().Get("chilling_action_title")
         << "\n";
    cout << "  1. " << TextManager::GetInstance().Get("chilling_action_1")
         << "\n";
    cout << "  2. " << TextManager::GetInstance().Get("chilling_action_2")
         << "\n";
    cout << "  3. " << TextManager::GetInstance().Get("chilling_action_3")
         << "\n";
    cout << "  4. " << TextManager::GetInstance().Get("chilling_action_4")
         << "\n";
    cout << "  5. " << TextManager::GetInstance().Get("chilling_action_5")
         << "\n";
    cout << "  " << TextManager::GetInstance().Get("main_menu_press_enter")
         << " ";

    int choice;
    cin >> choice;

    if (choice >= 1 && choice <= 4) {
      TakeItem(choice - 1);
    } else if (choice == 5) {
      if (next_order_ > 4) {
        room_completed = true;
        Screen::Clear();
        Screen::DrawBorder(TextManager::GetInstance().Get("room_complete"));
        cout << "  " << TextManager::GetInstance().Get("items_collected")
             << "\n";
        Screen::WaitForEnter();
        MeetBlacksmith();
      } else {
        cout << "  " << TextManager::GetInstance().Get("progress_not_completed")
             << "\n";
        Screen::WaitForEnter();
      }
    } else {
      cout << "  " << TextManager::GetInstance().Get("error_invalid") << "\n";
      Screen::WaitForEnter();
    }
  }

  completed_ = true;
}

void ElizabethCradle::ShowRoomStatus() {
  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get("elizabeth_title"));
  cout << "  " << TextManager::GetInstance().Get("elizabeth_entry2") << "\n\n";
  cout << "  " << TextManager::GetInstance().Get("elizabeth_items_title")
       << "\n";
  for (const auto& item : items_) {
    cout << "    " << item.id << ". " << item.name << " - "
         << (item.taken ? "ВЗЯТО" : item.description) << "\n";
  }
  if (next_order_ <= 4) {
    cout << "\n  "
         << TextManager::GetInstance().Get("elizabeth_hint_" +
                                           to_string(next_order_))
         << "\n";
  }
}

void ElizabethCradle::ShowCornerInfo(int corner_index) {
  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get(
      "chilling_corner_" + to_string(corner_index + 1)));
  const auto& item = items_[corner_index];
  if (item.taken) {
    cout << "  " << TextManager::GetInstance().Get("chilling_corner_empty")
         << " " << item.name << ".\n";
  } else if (item.correct_position == next_order_) {
    cout << "  " << TextManager::GetInstance().Get("chilling_found") << " "
         << item.name << " (" << item.id << ")\n";
    cout << "  " << item.description << "\n";
  } else {
    cout << "  " << TextManager::GetInstance().Get("chilling_wrong_item") << " "
         << item.name << " (" << item.id << ")\n";
  }
}

bool ElizabethCradle::TakeItem(int corner_index) {
  if (items_[corner_index].taken) {
    ShowCornerInfo(corner_index);
    cout << "\n  " << TextManager::GetInstance().Get("already_taken") << "\n";
    Screen::WaitForEnter();
    return false;
  }
  if (items_[corner_index].correct_position != next_order_) {
    ShowCornerInfo(corner_index);
    cout << "\n  " << TextManager::GetInstance().Get("elizabeth_dont_rush")
         << "\n";
    Screen::WaitForEnter();
    return false;
  }

  ShowCornerInfo(corner_index);
  cout << "\n  " << TextManager::GetInstance().Get("chilling_take") << " "
       << items_[corner_index].name << "\n";
  items_[corner_index].taken = true;
  collected_order_.push_back(items_[corner_index].id);
  ++next_order_;
  cout << "  " << TextManager::GetInstance().Get("item_added") << "\n";
  Screen::WaitForEnter();
  return true;
}

string ElizabethCradle::GetSequenceString() const {
  string result;
  for (size_t i = 0; i < collected_order_.size(); ++i) {
    result += to_string(collected_order_[i]);
    if (i + 1 < collected_order_.size()) result += "-";
  }
  return result;
}

void ElizabethCradle::MeetBlacksmith() {
  Screen::Clear();
  Screen::DrawBorder(
      TextManager::GetInstance().Get("blacksmith_elizabeth_title"));
  cout << "  " << TextManager::GetInstance().Get("blacksmith_elizabeth_intro")
       << "\n";
  cout << "  "
       << TextManager::GetInstance().Get("blacksmith_elizabeth_dialogue")
       << "\n";
  cout << "  " << TextManager::GetInstance().Get("blacksmith_chilling_prompt")
       << "\n";
  cout << "  "
       << TextManager::GetInstance().Get("blacksmith_chilling_your_sequence")
       << ": " << GetSequenceString() << "\n  ";

  string user_sequence;
  cin >> user_sequence;
  enchanted_ = (user_sequence == GetSequenceString());
  weapon_name_ = TextManager::GetInstance().Get("inventory_staff");

  cout << "\n  "
       << (enchanted_
               ? TextManager::GetInstance().Get("blacksmith_elizabeth_success")
               : TextManager::GetInstance().Get("blacksmith_elizabeth_fail"))
       << "\n";
  Screen::WaitForEnter();
  BattleWithGhost();
}

void ElizabethCradle::BattleWithGhost() {
  Enemy ghost(EnemyType::Ghost,
              TextManager::GetInstance().Get("battle_ghost_title"), 110, 10);

  bool victory =
      Combat::StartFight(ghost, weapon_name_, enchanted_ ? 30 : 10, enchanted_);
  if (victory) {
    Player::GetInstance().SetStaffEnchanted(enchanted_);
  }
}
