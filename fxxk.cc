/**
 * fxxk 是法拉利 xx 计划的终极车，性能只能用逆天形容。
 * 
 * 这个模块用法拉利的型号命名
 * 强调了它在整个游戏中无可取代的地位
 * 是整个游戏最为核心的部分
 */
namespace Fxxk {

static std::string base = "abc";
static std::string str = "";

class Fxxk {
 private:
  bool canMove;

 public:
  friend class Body;
  friend class Animal;
  friend class Plant;
  virtual void say() = 0;
};

class Animal : virtual public Fxxk {
 public:
  Animal() { canMove = true; }
  void say() { std::cout << "Animal: Fxxk!" << std::endl; }
};
class Plant : virtual public Fxxk {
 public:
  Plant() { canMove = false; }
  void say() { std::cout << "Plant: Fxxk!" << std::endl; }
};

class Body : public Animal, public Plant {
 public:
  Body(bool cm) { canMove = cm; }
  Body(Fxxk& fxxk) { canMove = fxxk.canMove; }
  void say() {
    if (canMove)
      Animal::say();
    else
      Plant::say();
  }
};

static bool str2bool(std::string s) {
  if (s == "true") {
    return true;
  } else if (s == "false") {
    return false;
  }
  throw std::exception();
}

static void exec() {
  std::cout << "Hi, can you move? (true or false)" << std::endl;
  std::string flag;
  std::cin >> flag;
  try {
    auto body = Body(str2bool(flag));
    body.say();
  } catch (const std::exception& e) {
    std::cout << "YOU MISSED FXXK" << std::endl;
  }
}

static bool handler(char ch) {
  str += ch;
  int sz = str.size();
  if (str[sz - 1] != base[sz - 1]) {
    str = "";
  } else if (str == base) {
    str = "";
    exec();
    return true;
  }
  return false;
}
}  // namespace Fxxk
