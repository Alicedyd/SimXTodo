#ifndef __TODO_LIST_H__
#define __TODO_LIST_H__

#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* some constants */
#define MAX_TODO 100

/* Status of single todo item */
typedef enum {
  TODO = 0,
  DOING = 1,
  DONE = 2,
} todo_status;

/* Single todo item */
struct todo_item {
  char *contents;
  todo_status status;
  time_t create_t;
};

/* Todo list */
struct todo_list {
  int count; /* current count of items */
  int limit; /* max limits of items */
  struct todo_item *items;
};

/* functions */
struct todo_item init_todo_item(char *contents);
struct todo_list *init_todo_list(void);

Result load_todo_list(struct todo_list *list, FILE *fp);
Result save_todo_list(struct todo_list *list, FILE *fp);

Result add_todo_item(struct todo_list *list, struct todo_item item);
Result delete_todo_item(struct todo_list *list, int item_index);

Result change_item_status(struct todo_item *list, int item_index);
Result change_item_contents(struct todo_list *list, int item_index,
                            char *new_contents);

#endif
