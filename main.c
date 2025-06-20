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
  // init_window();

  /* load the todo list */

  const char *file_name = "todo";

  struct todo_list *list = init_todo_list();

  Result result = load_todo_list(list, file_name);
  printf("Status: %d, MSG: %s", result.status, result.msg);

  struct todo_item item = init_todo_item("test");
  result = add_todo_item(list, item);

  save_todo_list(list, file_name);

  free_todo_list(list);

  // endwin();
  return 0;
}
