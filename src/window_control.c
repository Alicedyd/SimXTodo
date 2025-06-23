#include "../include/window_control.h"
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

/* popup functions */
WINDOW *create_popup(int height, int width, const char *title,
                     const char *msg) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  /* center positon */
  int start_y = (max_y - height) / 2;
  int start_x = (max_x - width) / 2;

  WINDOW *popup = newwin(height, width, start_y, start_x);

  /* draw the boundary */
  box(popup, 0, 0);

  /* add the title */
  if (title) {
    mvwprintw(popup, 0, (width - strlen(title)) / 2, "%s", title);
  }

  /* add the msg */
  if (msg) {
    mvwprintw(popup, height / 2, (width - strlen(msg)) / 2, "%s", msg);
  }

  /* refresh the window */
  wrefresh(popup);

  return popup;
}

void msg_popup(int width, const char *title, const char *msg) {
  WINDOW *popup = create_popup(POPUP_HEIGHT, width, title, msg);
  wgetch(popup);
  delwin(popup);

  /* clean the window */
  clear();
  refresh();
}

int confirm_popup(int width, const char *msg) {
  WINDOW *popup = create_popup(POPUP_HEIGHT, width, "Confirm", NULL);

  /* add the message */
  mvwprintw(popup, 2, (width - strlen(msg)) / 2, "%s", msg);

  /* add the options */
  mvwprintw(popup, 4, width / 4 - 2, "Y - Yes");
  mvwprintw(popup, 4, 3 * width / 4 - 4, "N - No");

  wrefresh(popup);

  int ch;
  while (true) {
    ch = wgetch(popup);

    if (ch == 'y' || ch == 'Y' || ch == 'n' || ch == 'N') {
      break;
    }
  }

  delwin(popup);
  clear();
  refresh();

  return (ch == 'y' || ch == 'Y') ? 1 : 0;
}

void input_popup(int height, int width, const char *prompt, char *buffer,
                 int buffer_size) {}

/* display the hint bar at the top of main window */
void list_help(void) {}

/* display the todo items according to status and current_selected */
void list_todo_list(struct todo_list *list) {}
