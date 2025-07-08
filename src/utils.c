#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
char *format_time(time_t time) {
  struct tm *time_info;
  char *data_string = (char *)malloc(sizeof(char) * 20);

  time_info = localtime(&time);

  /* format as a string */
  sprintf(data_string, "%04d-%02d-%02d", time_info->tm_year + 1900,
          time_info->tm_mon + 1, time_info->tm_mday);

  return data_string;
}

/* check the byte number of UTF-8 char */
int utf8_char_bytes(unsigned char first_byte) {
  if ((first_byte & 0x80) == 0)
    return 1; /* 0xxxxxxx*/
  if ((first_byte & 0xE0) == 0xC0)
    return 2; /* 110xxxxx*/
  if ((first_byte & 0xF0) == 0xE0)
    return 3; /* 1110xxxx*/
  if ((first_byte & 0xF8) == 0xF0)
    return 4; /* 11110xxx*/
  return 1;
}

/* get the start position of last char(utf8) */
int find_last_char_start(const char *str, int pos) {
  if (pos <= 0) {
    return 0;
  }

  int start = pos - 1;
  while (start > 0 && (str[start] & 0xC0) == 0x80) {
    start--;
  }

  return start;
}

/* calculate the display width of utf8 char */
int display_width(const char *str, int byte_len) {
  int width = 0;
  int i = 0;

  while (i < byte_len && str[i] != '\0') {
    unsigned char c = (unsigned char)str[i];
    int char_bytes = utf8_char_bytes(c);

    if (char_bytes == 1) {
      width += 1;
    } else {
      width += 2;
    }

    i += char_bytes;
  }

  return width;
}

/* get the HOME directory according to the system */
char *get_home_dirctory(void) {
  char *home_dir = NULL;

#ifdef _WIN32
  char *home_drive = getenv("HOMEDRIVE");
  char *home_path = getenv("HOMEPATH");

  if (home_drive && home_path) {
    home_dir = malloc(strlen(home_drive) + strlen(home_path) + 1);
    strcpy(home_dir, home_drive);
    strcpy(home_dir, home_path);
  } else {
    home_dir == getenv("USERPROFILE");
    if (home_dir) {
      char *temp = malloc(strlen(home_dir) + 1);
      strcpy(temp, home_dir);
      home_dir = temp;
    }
  }
#else
  home_dir = getenv("HOME");
  if (!home_dir) {
    /* if HOME does not exist, get from passwd */
    struct passwd *pw = getpwuid(getuid());
    if (pw) {
      home_dir = pw->pw_dir;
    }
  }

  if (home_dir) {
    char *temp = malloc(strlen(home_dir) + 1);
    strcpy(temp, home_dir);
    home_dir = temp;
  }
#endif

  return home_dir;
}

/* get the save file for todo list */
char *get_save_file_path(void) {
  char *home_dir = get_home_dirctory();
  if (!home_dir) {
    return NULL;
  }

  char *save_path = malloc(512);

#ifdef _WIN32
  /* Windows: %APPDATA%/SimxTodo/todo */
  char *appdata = getenv("APPDATA");
  if (appdata) {
    snprintf(save_path, 512, "%s%sSimxTodo%stodo", appdata, PATH_SEPERATOR,
             PATH_SEPERATOR);
  } else {
    snpring(save_path, 512, "%s%sSimxTodo%stodo", home_dir, PATH_SEPERATOR,
            PATH_SEPERATOR);
  }
#elif defined(__APPLE__)
  /* macos: ~/Library/Application Support/SimXTodo/todo */
  snprintf(save_path, 512, "%s%sLibrary%sApplication Support%sSimXTodo%stodo",
           home_dir, PATH_SEPERATOR, PATH_SEPERATOR, PATH_SEPERATOR,
           PATH_SEPERATOR);
#else
  /* Linux: ~/.config/simxtodo/todo */
  char *xdg_config = getenv("XDG_CONFIG_HOME");
  if (xdg_config) {
    snprintf(save_path, 512, "%s%ssimxtodo%stodo", xdg_config, PATH_SEPERATOR,
             PATH_SEPeRATOR);
  } else {
    snprintf(save_path, 512, "%s%s.config%ssimxtodo%stodo", home_dir,
             PATH_SEPERATOR, PATH_SEPERATOR, PATH_SEPERATOR);
  }
#endif

  free(home_dir);
  return save_path;
}

int file_exists(const char *path) {
  struct stat buffer;
  return (stat(path, &buffer) == 0);
}

int create_dirctory_recursive(const char *path) {
  char *path_copy = malloc(strlen(path) + 1);
  strcpy(path_copy, path);

  char *p = path_copy;

  /* skip the root dir */
#ifdef _WIN32
  if (strlne(p) > 3 && p[1] == ":") {
    p += 3; /* jump C:\ */
  }
#else
  if (p[0] == '/') {
    p++; /* jump "/" */
  }
#endif

  /* create the dirctories recursively */
  while (*p) {
    while (*p && *p != '/' && *p != '\\') {
      p++;
    }

    if (*p) {
      *p = '\0';

      if (!file_exists(path_copy)) {
        if (mkdir(path_copy, 0755) != 0 && errno != EEXIST) {
          free(path_copy);
          return -1;
        }
      }

#ifdef _WIN32
      *p = '\\';
#else
      *p = '/';
#endif
      p++;
    }
  }

  /* create the final directory */
  if (!file_exists(path_copy)) {
    if (mkdir(path_copy, 0755) != 0 && errno != EEXIST) {
      free(path_copy);
      return -1;
    }
  }

  free(path_copy);
  return 0;
}

/* ensure the save path exist */
Result ensure_save_file_exists(const char *file_path) {
  Result result;

  /* check is the file exist*/
  if (file_exists(file_path)) {
    result.status = RESULT_OK;
    result.msg = "OK";
    return result;
  }

  /* get the directory path */
  char *dir_path = malloc(strlen(file_path) + 1);
  strcpy(dir_path, file_path);

  char *last_sep = strrchr(dir_path, '/');
  if (!last_sep) {
    last_sep = strrchr(dir_path, '\\');
  }

  if (last_sep) {
    *last_sep = '\0';

    /* create the dirctory */
    if (create_dirctory_recursive(dir_path) != 0) {
      free(dir_path);
      result.status = RESULT_ERROR;
      result.msg = "Failed to create config dirctory";
      return result;
    }
  }

  free(dir_path);

  /* create an empty save file */
  FILE *fp = fopen(file_path, "wb");
  if (!fp) {
    result.status = RESULT_ERROR;
    result.msg = "Failed to create config file";
    return result;
  }

  /* write the initial data */
  int initial_count = 0;
  fwrite(&initial_count, sizeof(int), 1, fp);
  fclose(fp);

  result.status = RESULT_OK;
  result.msg = "OK";
  return result;
}
