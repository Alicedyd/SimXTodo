#ifndef __RESULT_H__
#define __RESULT_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RESULT_OK 0
#define RESULT_ERROR 1

typedef struct result {
  int status;
  char *msg;
} Result;

char *format_time(time_t);

int utf8_char_bytes(unsigned char first_byte);
int find_last_char_start(const char *str, int pos);
int display_width(const char *str, int byte_len);

#endif
