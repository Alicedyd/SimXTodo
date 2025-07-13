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

  /* 显示光标 */
  curs_set(1);

  /* 设置光标位置 */
  int cursor_x = 4;
  wmove(popup, 4, cursor_x);

  /* 输入处理 */
  int pos = 0; /* buffer中的字节位置 */
  int ch;

  /* 清空buffer */
  /* memset(buffer, 0, buffer_size); */

  if (strlen(buffer) > 0) {
    /* store the original information */

    mvwprintw(popup, 4, cursor_x, buffer);

    int initial_char_bytes = strlen(buffer);
    int initial_display_width = display_width(buffer, initial_char_bytes);

    pos = initial_char_bytes;
    cursor_x = initial_display_width + 4;

    wmove(popup, 4, cursor_x);
  }

  while ((ch = wgetch(popup)) != '\n' && ch != '\r') {
    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      /* 处理退格键 */
      if (pos > 0) {
        /* 找到上一个字符的起始位置 */
        int last_char_start = find_last_char_start(buffer, pos);
        int char_bytes = pos - last_char_start;

        /* 计算要删除的字符的显示宽度 */
        char temp_char[5] = {0};
        memcpy(temp_char, buffer + last_char_start, char_bytes);
        int char_width = display_width(temp_char, char_bytes);

        /* 清除屏幕上的字符 */
        for (int i = 0; i < char_width; i++) {
          mvwprintw(popup, 4, cursor_x - char_width + i, " ");
        }

        /* 更新buffer和光标位置 */
        memset(buffer + last_char_start, 0, char_bytes);
        pos = last_char_start;
        cursor_x -= char_width;

        /* 移动光标 */
        wmove(popup, 4, cursor_x);
      }
    } else if (ch == 27) {
      /* ESC键 - 清空输入 */
      /* 清除所有已输入的内容 */
      int input_width = display_width(buffer, pos);
      for (int i = 0; i < input_width; i++) {
        mvwprintw(popup, 4, 4 + i, " ");
      }

      memset(buffer, 0, buffer_size);
      pos = 0;
      cursor_x = 4;
      wmove(popup, 4, cursor_x);

      break;
    } else if (ch >= 32 && ch <= 126) {
      /* ASCII字符 */
      if (pos < buffer_size - 1) {
        buffer[pos++] = ch;
        mvwprintw(popup, 4, cursor_x, "%c", ch);
        cursor_x++;
        wmove(popup, 4, cursor_x);
      }
    } else if (ch > 127) {
      /* 多字节字符处理 */
      unsigned char first_byte = (unsigned char)ch;
      int bytes_needed = utf8_char_bytes(first_byte);

      /* 检查缓冲区空间 */
      if (pos + bytes_needed < buffer_size) {
        char mb_char[5] = {0};
        mb_char[0] = ch;

        /* 读取剩余字节 */
        for (int i = 1; i < bytes_needed; i++) {
          int next_byte = wgetch(popup);
          if (next_byte == ERR)
            break;
          mb_char[i] = next_byte;
        }

        /* 添加到buffer */
        memcpy(buffer + pos, mb_char, bytes_needed);
        pos += bytes_needed;

        /* 显示字符 */
        mvwprintw(popup, 4, cursor_x, "%s", mb_char);

        /* 更新光标位置（中文字符占2个位置） */
        cursor_x += (bytes_needed > 1) ? 2 : 1;
        wmove(popup, 4, cursor_x);
      }
    }

    wrefresh(popup);
  }

  /* 确保字符串正确结束 */
  buffer[pos] = '\0';

  /* 隐藏光标 */
  curs_set(0);

  delwin(popup);
  clear();
  refresh();
}

void help_popup(void) {
  int width = 30, height = 12;

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  int start_y, start_x;
  start_y = (max_y - height) / 2;
  start_x = (max_x - width) / 2;

  WINDOW *help = newwin(height, width, start_y, start_x);
  box(help, 0, 0);

  /* add the title */
  const char *title = "HELP";
  mvwprintw(help, 0, (width - strlen(title)) / 2, title);

  /* add the help items */
  const int help_count = 8;
  const char *helps[] = {
      "j/k: cursor move down/up",
      "J/K: item move down/up",
      "a: add todo item",
      "d: delete the item",
      "c: change item status",
      "m: modify the item",
      "h: show help",
      "q: quit the programm",
  };
  for (int i = 0; i < help_count; i++) {
    mvwprintw(help, i + 2, 2, helps[i]);
  }

  wrefresh(help);
  wgetch(help);
  delwin(help);

  /* clean the window */
  clear();
  refresh();
}

/* display the help bar and the todo items according to status and
 * current_selected */
void display_screen(struct todo_list *list, int screen_offset) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  /* display the title */
  int contents_len, status_len, date_len;

  /* calculate the length of different part */
  date_len = 15;
  status_len = 8;
  contents_len = max_x - date_len - status_len;

  /**
  mvwprintw(stdscr, 0, contents_len / 2, "TODO");
  mvwprintw(stdscr, 0, contents_len, "STATUS");
  mvwprintw(stdscr, 0, contents_len + status_len, "START DATE");
  **/

  mvwprintw(stdscr, 0, 2, "START DATE");
  mvwprintw(stdscr, 0, date_len, "STATUS");
  mvwprintw(stdscr, 0, date_len + status_len + contents_len / 2, "TODO");

  int end_index = screen_offset + max_y - 2;
  if (end_index > list->count) {
    end_index = list->count;
  }

  for (int i = screen_offset; i < end_index; i++) {
    if (i == list->current_selected) {
      attron(A_REVERSE);
    }

    attron(COLOR_PAIR(list->items[i].status + 1));

    /**
    mvwprintw(stdscr, i + 1 - screen_offset, 1, list->items[i].contents);

    if (list->items[i].status == STATUS_TODO) {
      mvwprintw(stdscr, i + 1 - screen_offset, contents_len, "[TODO]");
    } else if (list->items[i].status == STATUS_DOING) {
      mvwprintw(stdscr, i + 1 - screen_offset, contents_len, "[DOING]");
    } else if (list->items[i].status == STATUS_DONE) {
      mvwprintw(stdscr, i + 1 - screen_offset, contents_len, "[DONE]");
    }

    mvwprintw(stdscr, i + 1 - screen_offset, contents_len + status_len,
              format_time(list->items[i].create_t));
    **/

    mvwprintw(stdscr, i + 1 - screen_offset, 2,
              format_time(list->items[i].create_t));

    if (list->items[i].status == STATUS_TODO) {
      mvwprintw(stdscr, i + 1 - screen_offset, date_len, "[TODO]");
    } else if (list->items[i].status == STATUS_DOING) {
      mvwprintw(stdscr, i + 1 - screen_offset, date_len, "[DOING]");
    } else if (list->items[i].status == STATUS_DONE) {
      mvwprintw(stdscr, i + 1 - screen_offset, date_len, "[DONE]");
    }

    mvwprintw(stdscr, i + 1 - screen_offset, date_len + status_len + 1,
              list->items[i].contents);

    attroff(COLOR_PAIR(list->items[i].status + 1));

    if (i == list->current_selected) {
      attroff(A_REVERSE);
    }
  }

  attron(A_BOLD);
  mvwprintw(stdscr, max_y - 1, 1, "Press h for help");
  attroff(A_BOLD);

  refresh();
}
