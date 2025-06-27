#include "../include/utils.h"
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
