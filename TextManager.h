#ifndef TEXT_MANAGER_H_
#define TEXT_MANAGER_H_

#include <string>
#include <unordered_map>

class TextManager {
 public:
  static TextManager& GetInstance();
  bool Load(const std::string& filepath);
  std::string Get(const std::string& key) const;

 private:
  TextManager() = default;
  std::unordered_map<std::string, std::string> texts_;
};

#endif
