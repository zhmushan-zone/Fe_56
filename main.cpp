#include "./game.cpp"
#include "./util.cpp"

int main() {
  pre();

  auto game = Game::New();

entry : {
  Display::clearWindow();
  Display::entry();

  char direct = getch();
  if (Behavior::isEnter(direct)) goto direct;

  auto arrow = Behavior::getArrow(direct);
  if (arrow == Behavior::UP)
    Hint::prev();
  else if (arrow == Behavior::DOWN)
    Hint::next();

  goto entry;
}

game : {
  Display::clearWindow();

  char direct;
  for (;;) {
    direct = getch();
    goto entry;
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
    default:
      goto undefined;
  }
}

quit:

  return 0;
}
