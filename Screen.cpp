#include "Screen.h"

#include <iostream>
#include <limits>
#include <string>

#include "TextManager.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void Screen::Clear() {
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD home = {0, 0};
  DWORD written;

  if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', size, home, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, size, home,
                               &written);
    SetConsoleCursorPosition(hConsole, home);
  }
#else
  cout << "\033[2J\033[1;1H";
  cout.flush();
#endif
}

void Screen::WaitForEnter() {
  cout << "\n  " << TextManager::GetInstance().Get("press_enter");
  cout.flush();

  cin.ignore(1000, '\n');
  cin.get();
}

void Screen::WaitForEnter(const string& message) {
  cout << "\n  " << message;
  WaitForEnter();
}

void Screen::DrawBorder(const string& title) {
  const string border =
      "+----------------------------------------------------------+";
  cout << border << "\n";

  if (!title.empty()) {
    int padding = 57 - static_cast<int>(title.length());
    if (padding < 0) padding = 0;

    cout << "| " << title << string(padding, ' ') << "|\n";
    cout << border << "\n";
  }
}
