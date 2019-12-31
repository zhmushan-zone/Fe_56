#include "./deps.h"
#include "./util.cc"
#include "./game.cc"
#include "./fxxk.cc"

int main() {
  Util::pre();
  auto game = Game::New();

entry : {
  Display::clearWindow();
  Display::entry();

_tmp:
  char direct = getch();
  if (Fxxk::handler(direct)) goto _tmp;
  if (Behavior::isEnter(direct)) goto direct;

  auto arrow = Behavior::getArrow(direct);
  if (arrow == Behavior::UP)
    Hint::prev();
  else if (arrow == Behavior::DOWN)
    Hint::next();

  goto entry;
}

game : {
  for (; game.status == Game::Proceed;) {
    Display::clearWindow();
    game.recover();
    char direct = getch();
    if (Behavior::isQuit(direct)) goto entry;
    game.move(Behavior::getArrow(direct));
  }

  Display::clearWindow();
  game.recover();

  if (game.status == Game::Win) {
    Color::red("YOU WIN!");
  } else {
    Color::red("YOU FAILED!");
  }

  for (;;) {
    char direct = getch();
    if (Behavior::isQuit(direct)) goto entry;
  }

  goto quit;
}

help : {
  Display::clearWindow();
  Display::help();

  char direct;
  for (;;) {
    direct = getch();
    if (Behavior::isQuit(direct)) goto entry;
  }

  goto quit;
}

undefined : {
  Color::red("YOUR BEHAVIOR IS UNDEFINED!");
  goto quit;
}

direct : {
  switch (Hint::selected) {
    case Hint::NEW_GAME:
      game = Game::New();
      goto game;
    case Hint::HISTORY:
      goto game;
    case Hint::HELP:
      goto help;
    case Hint::QUIT:
      goto quit;
  }

  goto undefined;
}

quit:

  return 0;
}
