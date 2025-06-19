#include "../include/todo_list.h"

/* initilize an empty todo item */
struct todo_item init_todo_item(char *contents) {
  struct todo_item item;

  item.status = TODO;
  item.create_t = time(NULL);
  item.contents = contents;

  return item;
}

/* initialize an empty todo list */
struct todo_list *init_todo_list(void) {
  struct todo_list *list = (struct todo_list *)malloc(sizeof(struct todo_list));

  list->count = 0;
  list->limit = 1;
  list->items =
      (struct todo_item *)malloc(sizeof(struct todo_item) * list->limit);

  return list;
}

/* add one todo item into the todo list */
Result add_todo_item(struct todo_list *list, struct todo_item item) {
  Result result;

  /* whether the list is full */
  if (list->count == list->limit) {
    size_t new_limit = 2 * list->limit; // double the limit
    struct todo_item *new_items =
        realloc(list->items, sizeof(struct todo_item) * new_limit);

    if (!new_items) {
      /* realloc failed */
      result.status = RESULT_ERROR;
      result.msg = "Failed to expand todo list";

      return result;
    } else {
      list->limit = new_limit;
      list->items = new_items;
    }
  }

  /* add the new item */
  list->items[list->count] = item;
  list->count++;

  result.status = RESULT_OK;
  result.msg = "OK";

  return result;
}

/* delete one todo item from the todo list according to an index */
Result delete_todo_item(struct todo_list *list, int item_index) {
  Result result;
  int index;

  for (index = 0; index < list->count; index++) {
    if (index == item_index) {
      /* find the item wanted to delete */
      int j;

      /* move the items after the deleted item forward */
      for (j = index; j < list->count - 1; j++) {
        list->items[j] = list->items[j + 1];
      }
      result.status = RESULT_OK;
      result.msg = "OK";

      return result;
    }
  }

  /* can't find the item wanted to delete */
  result.status = RESULT_ERROR;
  result.msg = "Can't find the item";

  return result;
}
