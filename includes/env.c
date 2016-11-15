#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "utils.h"
#include "linked_list.h"

#define MAX_SIZE 255

// reads a single line from a file
char* read_line(FILE *file) {
  char *temp = malloc(MAX_SIZE * sizeof(char));
  return fgets(temp, MAX_SIZE, file);
}

// Like starts_with but handles spaces
int is_var(char *var, char *str) {
  return starts_with(var, str) &&
         str[strlen(var)] == '=';
}

int is_terminal(char c) {
  return c == '\0' || c == '\n' || c == EOF;
}

// returns pointer to after "=", creates new string
char* get_value(char *str) {
  char *temp = get_after('=', str);
  char *result = remove_trailing_newline(temp);
  free(temp);
  return result;
}

struct Node* parse_path(char *value) {

  char *copy = strdup(value);
  struct Node *path = NULL;
  char *delimiter = ":";

  // TODO: use strtok_r

  char *token;

  for (
    token = strtok(copy, delimiter);
    token != NULL;
    token = strtok(NULL, delimiter)
  ) {
    List_add(&path, strdup(token));
  }

  free(copy);

  return path;
}

void set_defaults(struct Env *env) {
  env->PATH = NULL;
  env->HOME = NULL;
}

struct Env* get_env(char *profile_path) {

  struct Env *env = malloc(sizeof(struct Env));
  FILE *file = fopen(profile_path, "r");

  set_defaults(env);

  if (file) {

    char *line;

    while ((line = read_line(file)) != NULL) {
      if (strchr(line, '=') != NULL) {
        if (is_var("PATH", line)) {
          env->PATH = get_value(line);
        } else if (is_var("HOME", line)) {
          env->HOME = get_value(line);
        } else {
          printf("UNKNOWN VAR: %s", line);
        }
      } else {
        // line didn't contain assignment.
        // ignore
      }
      free(line);
    }

  } else {
    fprintf(stderr, "Couldn't find profile in %s\n", profile_path);
  }

  return env;
}

char* get_cwd() {
  return getwd(NULL);
}
