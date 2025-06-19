#include "window_control.h"

#include <locale.h>
#include <ncurses.h>

void init_window(void) {
  setlocale(LC_ALL, ""); // support chinese

  initscr();

  /* initialize the color */
  start_color();
  init_pair(0, COLOR_RED, COLOR_BLACK);
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLUE);

  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
}
