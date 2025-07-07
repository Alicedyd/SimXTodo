#include "include/utils.h"
#include <stddef.h>
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
  // const char *file_name = "/Users/alicedyd/.todo";
  char *file_name = get_save_file_path();
  if (!file_name) {
    msg_popup(40, "Error", "Failed to determine config file path");
    endwin();
    return 1;
  }

  /* ensure the save file exists */
  Result file_chcek_result = ensure_save_file_exists(file_name);
  if (file_chcek_result.status == RESULT_ERROR) {
    msg_popup(strlen(file_chcek_result.msg) + 10, "Error",
              file_chcek_result.msg);
    free(file_name);
    endwin();
    return 1;
  }

  /* show the save file path */
  /**
   * char info_msg[512];
   * snprintf(info_msg, sizeof(info_msg), "Config file: %s", file_name);
   * msg_popup(strlen(info_msg) + 4, "Info", info_msg);
   **/

  struct todo_list *list = init_todo_list();
  int screen_offset = 0;
  Result result = load_todo_list(list, file_name);

  if (result.status == RESULT_ERROR) {
    /* print the error msg and clean the todo file (delte and touch) */
    msg_popup(strlen(result.msg) + 10, "Notice", result.msg);

    FILE *fp = fopen(file_name, "wb");
    if (fp) {
      int initial_count = 0;
      fwrite(&initial_count, sizeof(int), 1, fp);
      fclose(fp);
    }

    /* reload */
    result = load_todo_list(list, file_name);
    if (result.status == RESULT_ERROR) {
      msg_popup(30, "Error", "Failed to initialize todo list");
      free_todo_list(list);
      free(file_name);
      endwin();
      return 1;
    }
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
      char *buffer = (char *)malloc(sizeof(char) * MAX_TODO_CONTENTS_LEN);
      input_popup(MAX_TODO_CONTENTS_LEN + 4, "Enter the new TODO", buffer,
                  MAX_TODO_CONTENTS_LEN);
      if (strlen(buffer) > 0) {
        strcpy(list->items[list->current_selected].contents, buffer);
      }
      free(buffer);
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
    } else if (ch == 'J') {
      int source_index, target_index;
      source_index = list->current_selected;
      target_index = list->current_selected + 1;
      if (target_index >= list->count) {
        msg_popup(30, "Notice", "Can't move down");
      } else {
        main_result = change_todo_item(list, source_index, target_index);
        if (main_result.status == RESULT_OK) {
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
        }
      }
    } else if (ch == 'K') {
      int source_index, target_index;
      source_index = list->current_selected;
      target_index = list->current_selected - 1;
      if (target_index < 0) {
        msg_popup(30, "Notice", "Can't move up");
      } else {
        main_result = change_todo_item(list, source_index, target_index);
        if (main_result.status == RESULT_OK) {
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
