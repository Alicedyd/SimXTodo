#include "include/utils.h"
#define _GNU_SOURCE
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#include "include/todo_list.h"
#include "include/utils.h"
#include "include/window_control.h"

int main(void) {

  /* init the programm */
  init_window();

  /* load the todo list */
  const char *file_name = "./todo";
  struct todo_list *list = init_todo_list();
  int screen_offset = 0;
  Result result = load_todo_list(list, file_name);

  if (result.status == RESULT_ERROR) {
    /* print the error msg and clean the todo file (delte and touch) */
    msg_popup(strlen(result.msg) + 10, "Notice", result.msg);

    /* TODO: add the function to delete and create todo file */
  }

  /* clean the main window */
  clear();

  /* display the main window */
  display_screen(list, screen_offset);

  /* the main loop */
  int ch;
  Result main_result;
  while ((ch = getch()) != 'q') {
    if (ch == 'a') {
      struct todo_item new_item = init_todo_item(NULL);
      input_popup(MAX_TODO_CONTENTS_LEN + 4, "Please input the TODO",
                  new_item.contents, MAX_TODO_CONTENTS_LEN);

      main_result = add_todo_item(list, new_item);
      if (main_result.status == RESULT_ERROR) {
        msg_popup(strlen(main_result.msg) + 10, "ERROR", main_result.msg);
      }
    } else if (ch == 'd') {
      bool selection = confirm_popup(30, "Delete selected item?");
      if (selection) {
        main_result = delete_todo_item(list, list->current_selected);

        if (main_result.status == RESULT_ERROR) {
          msg_popup(strlen(main_result.msg) + 10, "ERROR", main_result.msg);
        } else if (main_result.status == RESULT_OK) {
          list->current_selected--;
          if (list->current_selected < 0) {
            list->current_selected = list->count - 1;
          }
        }
      }
    } else if (ch == 'c') {
      main_result = change_item_status(list, list->current_selected);
      if (main_result.status == RESULT_ERROR) {
        msg_popup(strlen(main_result.msg) + 10, "ERROR", main_result.msg);
      }
    } else if (ch == 'm') {
      input_popup(MAX_TODO_CONTENTS_LEN + 4, "Enter the new TODO",
                  list->items[list->current_selected].contents,
                  MAX_TODO_CONTENTS_LEN);
    } else if (ch == 'j') {
      list->current_selected++;
      if (list->current_selected >= list->count) {
        list->current_selected = 0;
        screen_offset = 0;
      } else {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        if (list->current_selected >= screen_offset + max_y - 2) {
          screen_offset++;
        }
      }
    } else if (ch == 'k') {
      list->current_selected--;
      if (list->current_selected < 0) {
        list->current_selected = list->count - 1;
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        screen_offset =
            (list->count > max_y - 2) ? list->count - (max_y - 2) : 0;
      } else {
        if (list->current_selected < screen_offset) {
          screen_offset--;
        }
      }
    } else if (ch == 'h') {
      help_popup();
    }

    /* refresh the main window */
    clear();
    display_screen(list, screen_offset);
  }

  save_todo_list(list, file_name);

  free_todo_list(list);

  endwin();
  return 0;
}
