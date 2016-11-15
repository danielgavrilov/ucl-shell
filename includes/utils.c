#include <string.h>
#include <stdlib.h>

#include "utils.h"

int streq(char *a, char *b) {
  return strcmp(a,b) == 0;
}

int starts_with(char *start, char *str) {
  return strncmp(start, str, strlen(start)) == 0;
}

// extract a null-terminated string.
char* extract_chars(char *str, size_t len) {
  char *result = malloc(sizeof(char) * (len + 1));
  strncpy(result, str, len);
  result[len] = '\0';
  return result;
}

char* get_before(char c, char *str) {
  char *p = strchr(str, c);
  if (p != NULL) {
    return extract_chars(str, (size_t)(p - str));
  } else {
    return extract_chars(str, strlen(str));
  }
}

char* get_after(char c, char *str) {
  char *p = strchr(str, c);
  if (p != NULL) {
    return extract_chars(p+1, strlen(p));
  } else {
    return extract_chars(p, 0);
  }
}

char* remove_trailing_newline(char *str) {
  size_t len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    return extract_chars(str, len-1);
  } else {
    return extract_chars(str, len);
  }
}

int count_delimiter(char c, char *str) {
  int count = 0;
  int state = 0; // 0 = string, 1 = delimiter
  int i = 0;
  for (i = 0; i < strlen(str); i++) {
    if (state == 0 && str[i] == c) {
      count++;
      state = 1;
    } else if (state == 1 && str[i] != c) {
      state = 0;
    }
  }
  return count;
}
