#include "../include/todo_list.h"

/* initilize an empty todo item */
struct todo_item init_todo_item(const char *contents) {
  struct todo_item item;

  item.status = STATUS_TODO;
  item.create_t = time(NULL);
  item.contents = (char *)malloc(sizeof(char) * MAX_TODO_CONTENTS_LEN);

  strcpy(item.contents, contents);

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

/* free the pointers when closing todo list */
void free_todo_list(struct todo_list *list) {
  /* free the contents pointers in todo items */
  int i;
  for (i = 0; i < list->count; i++) {
    free(list->items[i].contents);
  }

  /* free the items pointer */
  free(list->items);

  /* free the todo list pointer */
  free(list);
}

/* load a todo list from a file name */
Result load_todo_list(struct todo_list *list, const char *file_name) {
  Result result;
  FILE *fp = fopen(file_name, "rb");

  /* check the file pointer */
  if (!fp) {
    result.status = RESULT_ERROR;
    result.msg = "Invalid file pointer";

    return result;
  }

  /* load the item count */
  int saved_count;
  if (fread(&saved_count, sizeof(int), 1, fp) != 1) {
    if (feof(fp)) {
      /* empty file, meaning no todo item has been saved */
      result.status = RESULT_OK;
      result.msg = "OK";

      return result;
    }
    result.status = RESULT_ERROR;
    result.msg = "failed to read item count";

    return result;
  }

  if (saved_count < 0) {
    result.status = RESULT_ERROR;
    result.msg = "Invalid item count in file";

    return result;
  }

  /* read the items */
  int i;
  for (i = 0; i < saved_count; i++) {
    struct todo_item item;

    /* read the status */
    if (fread(&item.status, sizeof(int), 1, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "failed to read item status";

      return result;
    }

    /* read create_t */
    if (fread(&item.create_t, sizeof(time_t), 1, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "failed to read item create time";

      return result;
    }

    /* read the length of contents */
    int contents_len;
    if (fread(&contents_len, sizeof(int), 1, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "failed to read item contents length";

      return result;
    }

    if (contents_len < 0 || contents_len >= MAX_TODO_CONTENTS_LEN) {
      result.status = RESULT_ERROR;
      result.msg = "Invalid item contents length";

      return result;
    }

    /* malloc the memory and read the contents */
    item.contents = (char *)malloc(sizeof(char) * MAX_TODO_CONTENTS_LEN);

    if (fread(item.contents, sizeof(char), contents_len, fp) != 1) {
      free(item.contents);

      result.status = RESULT_ERROR;
      result.msg = "failed to read item contents";

      return result;
    }

    item.contents[contents_len] = '\0';

    Result add_result = add_todo_item(list, item);

    if (add_result.status == RESULT_ERROR) {
      return add_result;
    }
  }

  /* all done */
  result.status = RESULT_OK;
  result.msg = "Ok";

  return result;
}

/* save a todo list to a file name */
Result save_todo_list(struct todo_list *list, const char *file_name) {
  Result result;
  FILE *fp = fopen(file_name, "wb");

  if (!fp) {
    result.status = RESULT_ERROR;
    result.msg = "Invalid file pointer";

    return result;
  }

  /* write the count of items */
  if (fwrite(&list->count, sizeof(int), 1, fp) != 1) {
    result.status = RESULT_ERROR;
    result.msg = "Failed to write item count";

    return result;
  }

  /* write all the items */
  int i;
  for (i = 0; i < list->count; i++) {
    struct todo_item *item = &list->items[i];

    /* write the status */
    if (fwrite(&item->status, sizeof(int), 1, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "Failed to write item status";

      return result;
    }

    /* write create_t */
    if (fwrite(&item->create_t, sizeof(time_t), 1, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "Failed to write crearte time";

      return result;
    }

    /* write contents length */
    int contents_len = strlen(item->contents);
    if (fwrite(&contents_len, sizeof(int), 1, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "Failed to write contents length";

      return result;
    }

    /* write contents */
    if (fwrite(item->contents, sizeof(char), contents_len, fp) != 1) {
      result.status = RESULT_ERROR;
      result.msg = "Failed to write contents";

      return result;
    }
  }

  /* flush */
  if (fflush(fp) != 0) {
    result.status = RESULT_ERROR;
    result.msg = "Failed to flush file buffer";

    return result;
  }

  result.status = RESULT_OK;
  result.msg = "Ok";

  return result;
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

      /* free the contents */
      free(list->items[index].contents);

      /* move the items after the deleted item forward */
      int j;
      for (j = index; j < list->count - 1; j++) {
        list->items[j] = list->items[j + 1];
      }

      list->count -= 1;

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

/* Change the status of a todo item */
Result change_item_status(struct todo_list *list, int item_index) {
  Result result;
  int index;

  for (index = 0; index < list->count; index++) {
    if (index == item_index) {
      list->items[index].status = (list->items[index].status + 1) % 3;

      result.status = RESULT_OK;
      result.msg = "OK";

      return result;
    }
  }

  /* can't find the item */
  result.status = RESULT_ERROR;
  result.msg = "Can't find the item";

  return result;
}

Result change_item_contents(struct todo_list *list, int item_index,
                            char *new_contents) {
  Result result;
  int index;

  for (index = 0; index < list->count; index++) {
    if (index == item_index) {
      strcpy(list->items[index].contents, new_contents);

      result.status = RESULT_OK;
      result.msg = "OK";

      return result;
    }
  }

  /* can't find the item */
  result.status = RESULT_ERROR;
  result.msg = "Can't find the item";

  return result;
}
