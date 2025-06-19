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
  struct todo_item test = init_todo_item("test");
  printf("contents: %s, status: %d, time: %s/n", test.contents, test.status,
         format_time(test.create_t));

  struct todo_list *list = init_todo_list();
  Result result = add_todo_item(list, test);
  printf("limit: %d, count: %d, item.contents: %s/n", list->limit, list->count,
         list->items[0].contents);

  return 0;
}
