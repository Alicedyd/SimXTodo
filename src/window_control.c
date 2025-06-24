#include "../include/window_control.h"
#include <ncurses.h>

void init_window(void) {
  setlocale(LC_ALL, ""); // support chinese

  initscr();

  /* initialize the color */
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLUE);

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

bool confirm_popup(int width, const char *msg) {
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

  return (ch == 'y' || ch == 'Y');
}

void input_popup(int width, const char *prompt, char *buffer, int buffer_size) {
  WINDOW *popup = create_popup(POPUP_HEIGHT, width, "Input", NULL);

  /* add the prompt */
  mvwprintw(popup, 2, 2, "%s", prompt);

  /* add the input bar */
  mvwprintw(popup, 4, 2, "> ");

  wrefresh(popup);

  /* get the input */
  echo();
  mvwgetnstr(popup, 4, 4, buffer, buffer_size - 1);
  noecho();

  delwin(popup);
  clear();
  refresh();
}

/* display the help bar and the todo items according to status and
 * current_selected */
void display_screen(struct todo_list *list) {
  /* display the help bar */
  mvwprintw(stdscr, 0, 1,
            "j: move up; k: move down; a: add item; d: delete item; c: change "
            "the status");

  /* display the title */
  mvwprintw(stdscr, 1, (MAX_TODO_CONTENTS_LEN - 4) / 2, "TODO");
  mvwprintw(stdscr, 1, MAX_TODO_CONTENTS_LEN + 1, "STATUS");
  mvwprintw(stdscr, 1, MAX_TODO_CONTENTS_LEN + 16, "START DATE");

  for (int i = 0; i < list->count; i++) {
    if (i == list->current_selected) {
      attron(COLOR_PAIR(4) | A_REVERSE);
    }

    attron(COLOR_PAIR(list->items[i].status + 1));

    mvwprintw(stdscr, i + 2, 0, list->items[i].contents);

    if (list->items[i].status == STATUS_TODO) {
      mvwprintw(stdscr, i + 2, MAX_TODO_CONTENTS_LEN + 1, "[TODO]");
    } else if (list->items[i].status == STATUS_DOING) {
      mvwprintw(stdscr, i + 2, MAX_TODO_CONTENTS_LEN + 1, "[DOING]");
    } else if (list->items[i].status == STATUS_DONE) {
      mvwprintw(stdscr, i + 2, MAX_TODO_CONTENTS_LEN + 1, "[DONE]");
    }

    mvwprintw(stdscr, i + 2, MAX_TODO_CONTENTS_LEN + 16,
              format_time(list->items[i].create_t));

    attroff(COLOR_PAIR(list->items[i].status + 1));

    if (i == list->current_selected) {
      attroff(COLOR_PAIR(4) | A_REVERSE);
    }
  }

  refresh();
}
