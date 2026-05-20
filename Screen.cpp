#include "Screen.h"

#include <iostream>
#include <limits>

#include "TextManager.h"

void Screen::Clear() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void Screen::WaitForEnter() {
  std::cout << "\n  " << TextManager::GetInstance().Get("press_enter");
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
}

void Screen::WaitForEnter(const std::string& message) {
  std::cout << "\n  " << message;
  WaitForEnter();
}

void Screen::DrawBorder(const std::string& title) {
  std::cout << "+----------------------------------------------------------+\n";
  if (!title.empty()) {
    std::cout << "| " << title << "\n";
    std::cout
        << "+----------------------------------------------------------+\n";
  }
}
