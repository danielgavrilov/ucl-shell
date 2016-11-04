#ifndef UTILS_H
#define UTILS_H

#include <string.h>

int streq(char*, char*);
int starts_with(char *start, char *str);
char* extract_chars(char *str, size_t len);
char* get_before(char c, char *str);
char* get_after(char c, char *str);
char* remove_trailing_newline(char *str);
int count_delimiter(char c, char *str);

#endif
