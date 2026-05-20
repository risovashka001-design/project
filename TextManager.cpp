#include "TextManager.h"

#include <fstream>
#include <iostream>

TextManager& TextManager::GetInstance() {
  static TextManager instance;
  return instance;
}

bool TextManager::Load(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Cannot open file: " << filepath << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;
    size_t pos = line.find('=');
    if (pos != std::string::npos) {
      texts_[line.substr(0, pos)] = line.substr(pos + 1);
    }
  }

  return true;
}

std::string TextManager::Get(const std::string& key) const {
  auto it = texts_.find(key);
  if (it != texts_.end()) return it->second;
  return "[" + key + "]";
}
