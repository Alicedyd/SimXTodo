#ifndef __WINDOW_CONTROL_H__
#define __WINDOW_CONTROL_H__

#include <locale.h>
#include <ncurses.h>
#include <regex.h>
#include <string.h>

#include "todo_list.h"
#include "utils.h"

#define POPUP_HEIGHT 7

void init_window(void);

WINDOW *create_popup(int height, int width, const char *title, const char *msg);
void msg_popup(int width, const char *title, const char *msg);
int confirm_popup(int width, const char *msg);
void input_popup(int height, int width, const char *prompt, char *buffer,
                 int buffer_size);

void list_help(void);
void list_todo_list(struct todo_list *list);

#endif
