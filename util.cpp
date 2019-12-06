#include "./deps.h"

typedef unsigned int uint;

namespace Hint {

enum Key {
  NEW_GAME,
  HISTORY,
  HELP,
  QUIT,
};

static std::string list[] = {"> New game", "> History", "> Help", "> Quit"};
static int selected = 0;
static uint listSize = sizeof(list) / sizeof(list[0]);
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
2H	=	1H	+	1H
3He	=	2H	+	1H
4He	=	3He	+	3He
7Be	=	4He	+	3He
8Be	=	4He	+	4He
12C	=	8Be	+	4He
16O	=	12C	+	4He
20Ne	=	16O	+	4He
24Mg	=	20Ne	+	4He
32S	=	28Si	+	4He
36Ar	=	32S	+	4He
40Ca	=	36Ar	+	4He
44Ti	=	40Ca	+	4He
48Cr	=	44Ti	+	4He
52Fe	=	48Cr	+	4He
56Ni	=	52Fe	+	4He
28Si	=	16O	+	16O
20Ne	=	12C	+	12C

Decay Guide
7Be	decays to	4He
8Be	decays to	4He
20Ne	decays to	16O
52Fe	decays to	48Cr
56Ni	decays to	56Fe

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

static void pre() {
  srand(time(NULL));
}
