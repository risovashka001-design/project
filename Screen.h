#ifndef SCREEN_H_
#define SCREEN_H_

#include <string>

class Screen {
 public:
  static void Clear();
  static void DrawBorder(const std::string& title = "");
  static void WaitForEnter();
  static void WaitForEnter(const std::string& message);
};

#endif
