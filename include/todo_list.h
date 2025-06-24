#ifndef __TODO_LIST_H__
#define __TODO_LIST_H__

#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* some constants */
#define STATUS_TODO 0
#define STATUS_DOING 1
#define STATUS_DONE 2

#define MAX_TODO_CONTENTS_LEN 100

#define NOT_SELECTED -1

/* Single todo item */
struct todo_item {
  char *contents;
  int status;
  time_t create_t;
};

/* Todo list */
struct todo_list {
  int count; /* current count of items */
  int limit; /* max limits of items */
  struct todo_item *items;

  /* index of current selected item, don't save to the file */
  int current_selected;
};

/* functions */
struct todo_item init_todo_item(const char *contents);
struct todo_list *init_todo_list(void);
void free_todo_list(struct todo_list *list);

Result load_todo_list(struct todo_list *list, const char *file_name);
Result save_todo_list(struct todo_list *list, const char *file_name);

Result add_todo_item(struct todo_list *list, struct todo_item item);
Result delete_todo_item(struct todo_list *list, int item_index);

Result change_item_status(struct todo_list *list, int item_index);
Result change_item_contents(struct todo_list *list, int item_index,
                            char *new_contents);

#endif
