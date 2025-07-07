#ifndef __RESULT_H__
#define __RESULT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32

#include <direct.h>
#include <shlobj.h>
#include <windows.h>
#define PATH_SEPERATOR "\\"
#define mkdir(path, mode) _mkdir(path)

#else

#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#define PATH_SEPERATOR "/"

#endif

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

char *get_home_dirctory(void);
char *get_save_file_path(void);
int file_exits(const char *path);
int create_dirctory_recursive(const char *path);
Result ensure_save_file_exists(const char *file_path);

#endif
