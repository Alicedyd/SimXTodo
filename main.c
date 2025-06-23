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
  const char *file_name = "todo";
  struct todo_list *list = init_todo_list();
  Result result = load_todo_list(list, file_name);

  const char *prompt = "This is a very long prompt for confirm popup to test "
                       "the auto length of the popup";
  int res = confirm_popup(strlen(prompt) + 10, prompt);

  clear();

  if (res == 1) {
    msg_popup(30, "Notice", "yes");
  } else {
    msg_popup(30, "Notice", "no");
  }

  if (result.status == RESULT_ERROR) {
    /* print the error msg and clean the todo file (delte and touch) */
    msg_popup(strlen(result.msg) + 10, "Notice", result.msg);

    /* TODO: add the function to delete and create todo file */
  }

  /* clean the main window */
  clear();

  /* display the hint bar */
  list_help();

  /* display the todo items */
  list_todo_list(list);

  /* the main loop */
  int ch;
  while ((ch = getch()) != 'q') {
    /* TODO write the main loop */
  }

  save_todo_list(list, file_name);

  free_todo_list(list);

  endwin();
  return 0;
}
