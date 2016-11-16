#include <string.h>
#include <stdlib.h>

#include "utils.h"

// String equality, helper function
int streq(char *a, char *b) {
  return strcmp(a,b) == 0;
}

// Checks if the 2nd string starts with the 1st.
int starts_with(char *start, char *str) {
  return strncmp(start, str, strlen(start)) == 0;
}

// Extracts `len` number of chars from string. The returned string is always
// null-terminated.
char* extract_chars(char *str, size_t len) {
  char *result = malloc(sizeof(char) * (len + 1));
  strncpy(result, str, len);
  result[len] = '\0';
  return result;
}

// Given a character and a string, it returns the substring up to the first
// occurrence of character, and not including the character.
// If the character `c` never occurrs, it returns a copy of the string.
char* get_before(char c, char *str) {
  char *p = strchr(str, c);
  if (p != NULL) {
    return extract_chars(str, p - str);
  } else {
    return extract_chars(str, strlen(str));
  }
}

// Like get_before, but returns the substring after the first occurrence of
// the character, and not including the character.
// If the character `c` never occurrs, it returns an empty string.
char* get_after(char c, char *str) {
  char *p = strchr(str, c);
  if (p != NULL) {
    return extract_chars(p+1, strlen(p));
  } else {
    return extract_chars(p, 0);
  }
}

// Given a string, it returns a copy of it excluding a trailing newline.
// If there is no trailing newline, a copy of the string is returned.
char* remove_trailing_newline(char *str) {
  size_t len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    return extract_chars(str, len-1);
  } else {
    return extract_chars(str, len);
  }
}

// Given a character and a string, it counts the number of continuous
// occurrences of the character.
// E.g. if c = ':' and str = "a:b::c:::d", then the result is 3.
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
