#include "./deps.h"

class Game {
  int scale;
  std::vector<std::vector<std::string>> src;
  Game(int scale, std::vector<std::vector<std::string>> &src)
      : scale(scale), src(src) {}

 public:
  static Game New(int scale = 4) {
    std::vector<std::vector<std::string>> src(scale);
    for (auto &i : src) {
      i.resize(scale);
    }
    return Game(scale, src);
  }
};
