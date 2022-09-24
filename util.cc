namespace Hint {

enum Key {
  NEW_GAME,
  HISTORY,
  HELP,
  QUIT,
};

static std::string list[] = {"> New game", "> History", "> Help", "> Quit"};
static int selected = 0;
static u32 listSize = sizeof(list) / sizeof(list[0]);
static void next() {
  ++selected;
  if (selected >= listSize) selected = listSize - 1;
}
static void prev() {
  --selected;
  if (selected < 0) selected = 0;
}
}  // namespace Hint

namespace Color {
enum Code {
  FG_RED = 31,
  FG_GREEN = 32,
  FG_BLUE = 34,
  FG_DEFAULT = 39,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_BLUE = 44,
  BG_DEFAULT = 49
};

class Color {
  Code code;

 public:
  Color(Code c) : code(c) {}
  friend std::ostream& operator<<(std::ostream& os, const Color& color) {
    return os << "\033[" << color.code << "m";
  }
};

static auto ored = Color(FG_RED);
static auto odef = Color(FG_DEFAULT);

static void red(std::string s) { std::cout << ored << s << odef << std::endl; }

static void def(std::string s) { std::cout << odef << s << odef << std::endl; }
}  // namespace Color

namespace Display {
static void entry() {
  for (int i = 0; i < Hint::listSize; ++i) {
    if (i == Hint::selected)
      Color::red(Hint::list[i]);
    else
      Color::def(Hint::list[i]);
  }
}

static void help() {
  std::cout <<
      R"(Fusion Guide
(2)H    =   (1)H    +   (1)H
(3)He   =   (2)H    +   (1)H
(4)He   =   (3)He   +   (3)He
(7)Be   =   (4)He   +   (3)He
(8)Be   =   (4)He   +   (4)He
(12)C   =   (8)Be   +   (4)He
(16)O   =   (12)C   +   (4)He
(20)Ne  =   (16)O   +   (4)He
(24)Mg  =   (20)Ne  +   (4)He
(32)S   =   (28)Si  +   (4)He
(36)Ar  =   (32)S   +   (4)He
(40)Ca  =   (36)Ar  +   (4)He
(44)Ti  =   (40)Ca  +   (4)He
(48)Cr  =   (44)Ti  +   (4)He
(52)Fe  =   (48)Cr  +   (4)He
(56)Ni  =   (52)Fe  +   (4)He
(28)Si  =   (16)O   +   (16)O
(20)Ne  =   (12)C   +   (12)C

Decay Guide
(7)Be  decays to (4)He
(8)Be  decays to (4)He
(20)Ne decays to (16)O
(52)Fe decays to (48)Cr
(56)Ni decays to (56)Fe

Press `q` to return the entrance.)";
}

static void clearWindow() { system("CLS"); }
}  // namespace Display

namespace Behavior {

enum Arrow { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75, UNKNOW = -1 };

static Arrow getArrow(char arrow) {
  if (arrow == 'w' || arrow == Arrow::UP) return Arrow::UP;
  if (arrow == 'a' || arrow == Arrow::LEFT) return Arrow::LEFT;
  if (arrow == 's' || arrow == Arrow::DOWN) return Arrow::DOWN;
  if (arrow == 'd' || arrow == Arrow::RIGHT) return Arrow::RIGHT;
  return Arrow::UNKNOW;
}

static bool isQuit(char direct) { return direct == 'q'; }

static bool isEnter(char direct) { return direct == 13; }
}  // namespace Behavior

namespace Element {
const static std::string H_1 = "(1)H", H_2 = "(2)H", He_3 = "(3)He",
                         He_4 = "(4)He", Be_7 = "(7)Be", Be_8 = "(8)Be",
                         C_12 = "(12)C", O_16 = "(16)O", Ne_20 = "(20)Ne",
                         Mg_24 = "(24)Mg", S_32 = "(32)S", Si_28 = "(28)Si",
                         Ar_36 = "(36)Ar", Ca_40 = "(40)Ca", Ti_44 = "(44)Ti",
                         Cr_48 = "(48)Cr", Fe_52 = "(52)Fe", Ni_56 = "(56)Ni",
                         Fe_56 = "(56)Fe";

static auto fusion =
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>();
static auto decay = std::unordered_map<std::string, std::string>();

static void addFusionEquation(std::string s1, std::string s2,
                              std::string target) {
  fusion[s1][s2] = target;
  fusion[s2][s1] = target;
}
static void addDecayEquation(std::string from, std::string to) {
  decay[from] = to;
}

static void init() {
  addFusionEquation(H_1, H_1, H_2);
  addFusionEquation(H_2, H_1, He_3);
  addFusionEquation(He_3, He_3, He_4);
  addFusionEquation(He_3, He_4, Be_7);
  addFusionEquation(He_4, He_4, Be_8);
  addFusionEquation(Be_8, He_4, C_12);
  addFusionEquation(C_12, He_4, O_16);
  addFusionEquation(O_16, He_4, Ne_20);
  addFusionEquation(Ne_20, He_4, Mg_24);
  addFusionEquation(Si_28, He_4, S_32);
  addFusionEquation(S_32, He_4, Ca_40);
  addFusionEquation(Ca_40, He_4, Ti_44);
  addFusionEquation(Ti_44, He_4, Cr_48);
  addFusionEquation(Cr_48, He_4, Fe_52);
  addFusionEquation(Fe_52, He_4, Ni_56);
  addFusionEquation(O_16, O_16, Si_28);
  addFusionEquation(C_12, C_12, Ne_20);

  addDecayEquation(Be_7, He_4);
  addDecayEquation(Be_8, He_4);
  addDecayEquation(Ne_20, O_16);
  addDecayEquation(Fe_52, Cr_48);
  addDecayEquation(Ni_56, Fe_56);
}
}  // namespace Element

namespace Util {
static void pre() {
  SetConsoleOutputCP(CP_UTF8);
  srand(time(NULL));
  Element::init();
}
// result ∈ [from, to)
static u32 random(int from, int to) { return (rand() % (to - from)) + from; }
}  // namespace Util
