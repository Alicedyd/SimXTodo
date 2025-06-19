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
