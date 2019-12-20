#include "./random_set.cc"

namespace Game {
enum Status { Failed, Proceed, Win };

class Game {
  int scale;
  std::vector<std::vector<std::string>> src;
  RandomSet<std::pair<int, int>> freePos;
  void init() {
    for (int i = 0; i < scale; ++i) {
      for (int j = 0; j < scale; ++j) {
        freePos.insert({i, j});
      }
    }
    generateElts(2);
  }
  void generateElts(int n) {
    for (int i = 0; i < n; ++i) {
      if (freePos.empty()) {
        status = Failed;
      }
      auto pos = freePos.pop();
      src[pos.first][pos.second] =
          Util::random(0, 10) == 9 ? Element::H_2 : Element::H_1;
    }
  }
  bool canFusion(std::string s1, std::string s2) {
    return !Element::fusion[s1][s2].empty();
  }
  std::string fusion(std::string s1, std::string s2) {
    return Element::fusion[s1][s2];
  }
  void decay() {
    for (int i = 0; i < scale; ++i) {
      for (int j = 0; j < scale; ++j) {
        if (!Element::decay[src[i][j]].empty()) {
          int random = Util::random(0, 11);
          if (random == 10) {
            src[i][j] = Element::decay[src[i][j]];

            // decay to (56)Fe
            if (src[i][j] == Element::Fe_56) {
              status = Win;
            }
          }
        }
      }
    }
  }
  bool moveToLeft() {
    bool reallyMove = false;
    for (int i = 0; i < scale; ++i) {
      int index = 0;
      for (int j = 0; j < scale; ++j) {
        if (src[i][j].empty()) {
          continue;
        }
        if (j == scale - 1) {
          if (index != j) reallyMove = true;
          src[i][index] = src[i][j];
          ++index;
          break;
        }
        for (int k = j + 1; k < scale; ++k) {
          if (src[i][k].empty()) {
            if (k == scale - 1) {
              if (index != j) reallyMove = true;
              src[i][index] = src[i][j];
              ++index;
              j = k;
            }
            continue;
          }
          if (canFusion(src[i][j], src[i][k])) {
            reallyMove = true;
            src[i][index] = fusion(src[i][j], src[i][k]);
            ++index;

            // ignore the element which index is `k`.
            j = k;
          } else {
            if (index != j) reallyMove = true;
            src[i][index] = src[i][j];
            ++index;

            // next step is to find an element which can fusion with
            // src[i][k].
            j = k - 1;
          }

          // This loop will continue when src[i][k] is empty.
          break;
        }
      }
      for (; index < scale; ++index) {
        src[i][index] = "";
        freePos.insert({i, index});
      }
    }
    return reallyMove;
  }
  bool moveToRight() {
    bool reallyMove = false;
    for (int i = 0; i < scale; ++i) {
      int index = scale - 1;
      for (int j = scale - 1; j >= 0; --j) {
        if (src[i][j].empty()) {
          continue;
        }
        if (j == 0) {
          if (index != j) reallyMove = true;
          src[i][index] = src[i][j];
          --index;
          break;
        }
        for (int k = j - 1; k >= 0; --k) {
          if (src[i][k].empty()) {
            if (k == 0) {
              if (index != j) reallyMove = true;
              src[i][index] = src[i][j];
              --index;
              j = k;
            }
            continue;
          }
          if (canFusion(src[i][j], src[i][k])) {
            reallyMove = true;
            src[i][index] = fusion(src[i][j], src[i][k]);
            --index;

            // ignore the element which index is `k`.
            j = k;
          } else {
            if (index != j) reallyMove = true;
            src[i][index] = src[i][j];
            --index;

            // next step is to find an element which can fusion with
            // src[i][k].
            j = k + 1;
          }

          // This loop will continue when src[i][k] is empty.
          break;
        }
      }
      for (; index >= 0; --index) {
        src[i][index] = "";
        freePos.insert({i, index});
      }
    }
    return reallyMove;
  }
  bool moveUp() {
    bool reallyMove = false;
    for (int i = 0; i < scale; ++i) {
      int index = 0;
      for (int j = 0; j < scale; ++j) {
        if (src[j][i].empty()) {
          continue;
        }
        if (j == scale - 1) {
          if (index != j) reallyMove = true;
          src[index][i] = src[j][i];
          ++index;
          break;
        }
        for (int k = j + 1; k < scale; ++k) {
          if (src[k][i].empty()) {
            if (k == scale - 1) {
              if (index != j) reallyMove = true;
              src[index][i] = src[j][i];
              ++index;
              j = k;
            }
            continue;
          }
          if (canFusion(src[j][i], src[k][i])) {
            reallyMove = true;
            src[index][i] = fusion(src[j][i], src[k][i]);
            ++index;

            // ignore the element which index is `k`.
            j = k;
          } else {
            if (index != j) reallyMove = true;
            src[index][i] = src[j][i];
            ++index;

            // next step is to find an element which can fusion with
            // src[k][i].
            j = k - 1;
          }

          // This loop will continue when src[k][i] is empty.
          break;
        }
      }
      for (; index < scale; ++index) {
        src[index][i] = "";
        freePos.insert({index, i});
      }
    }
    return reallyMove;
  }
  bool moveDown() {
    bool reallyMove = false;
    for (int i = 0; i < scale; ++i) {
      int index = scale - 1;
      for (int j = scale - 1; j >= 0; --j) {
        if (src[j][i].empty()) {
          continue;
        }
        if (j == 0) {
          if (index != j) reallyMove = true;
          src[index][i] = src[j][i];
          --index;
          break;
        }
        for (int k = j - 1; k >= 0; --k) {
          if (src[k][i].empty()) {
            if (k == 0) {
              if (index != j) reallyMove = true;
              src[index][i] = src[j][i];
              --index;
              j = k;
            }
            continue;
          }
          if (canFusion(src[j][i], src[k][i])) {
            reallyMove = true;
            src[index][i] = fusion(src[j][i], src[k][i]);
            --index;

            // ignore the element which index is `k`.
            j = k;
          } else {
            if (index != j) reallyMove = true;
            src[index][i] = src[j][i];
            --index;

            // next step is to find an element which can fusion with
            // src[k][i].
            j = k + 1;
          }

          // This loop will continue when src[k][i] is empty.
          break;
        }
      }
      for (; index >= 0; --index) {
        src[index][i] = "";
        freePos.insert({index, i});
      }
    }
    return reallyMove;
  }

 public:
  Status status = Proceed;
  Game(int scale, std::vector<std::vector<std::string>> &src)
      : scale(scale), src(src) {
    init();
  }
  void recover() {
    for (int j = 0; j < scale; ++j) {
      std::cout << "───────";
    }
    std::cout << '\n';
    for (int i = 0; i < scale; ++i) {
      for (int j = 0; j < scale; ++j) {
        std::cout << std::setw(6) << src[i][j] << "│";
      }
      std::cout << '\n';
      for (int j = 0; j < scale; ++j) {
        std::cout << "───────";
      }
      std::cout << '\n';
    }
  }
  Status move(Behavior::Arrow arrow) {
    if (status != Proceed) return status;

    using namespace Behavior;
    bool reallyMove = false;
    freePos.clear();
    if (Arrow::LEFT == arrow) {
      reallyMove = moveToLeft();
    } else if (Arrow::RIGHT == arrow) {
      reallyMove = moveToRight();
    } else if (Arrow::UP == arrow) {
      reallyMove = moveUp();
    } else if (Arrow::DOWN == arrow) {
      reallyMove = moveDown();
    }

    if (reallyMove) {
      decay();
      generateElts(1);
    }

    return Proceed;
  }
};

static Game New(int scale = 4) {
  std::vector<std::vector<std::string>> src(scale);
  for (auto &i : src) {
    i.resize(scale);
  }
  return Game(scale, src);
}
}  // namespace Game
