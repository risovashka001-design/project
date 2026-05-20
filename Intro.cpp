#include "Intro.h"

#include <iostream>

#include "Screen.h"
#include "TextManager.h"

using namespace std;

void Intro::Show() {
  Screen::Clear();
  Screen::DrawBorder(TextManager::GetInstance().Get("intro_title"));
  cout << "  " << TextManager::GetInstance().Get("intro_line1") << "\n";
  cout << "  " << TextManager::GetInstance().Get("intro_line2") << "\n";
  cout << "  " << TextManager::GetInstance().Get("intro_line3") << "\n";
  cout << "  " << TextManager::GetInstance().Get("intro_line4") << "\n";
  cout << "  " << TextManager::GetInstance().Get("intro_line5") << "\n";
  cout << "  " << TextManager::GetInstance().Get("intro_line6") << "\n";
  Screen::WaitForEnter(TextManager::GetInstance().Get("intro_press_enter"));
}
