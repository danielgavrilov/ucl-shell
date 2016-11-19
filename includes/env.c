#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "utils.h"
#include "linked_list.h"

#define MAX_SIZE 1024
#define MAX_CWD_SIZE 10240

// Reads a single line from a file. The returned string needs to be freed.
char* read_line(FILE *file) {
  char *temp = malloc(MAX_SIZE * sizeof(char));
  return fgets(temp, MAX_SIZE, file);
}

// Given a variable name and a string, it checks if the string is an assignment
// of the variable.
int is_var_assignment(char *var, char *str) {
  return starts_with(var, str) &&
         str[strlen(var)] == '=';
}

// Given an "assignment string", it returns the value (as a string) that it assigns.
char* get_value(char *str) {
  char *temp = get_after('=', str);
  char *result = remove_trailing_newline(temp);
  free(temp);
  return result;
}

// Given a "path string" of directory paths separated by colons ':', it returns
// a linked list of the individual directory paths.
struct Node* parse_path(char *path_string) {

  char *copy = strdup(path_string);
  struct Node *path = NULL;
  char *delimiter = ":";

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

// Given a path to the "profile" file, it returns an Env struct with the parsed
// variable assignments.
struct Env* get_env(char *profile_path) {

  struct Env *env = malloc(sizeof(struct Env));
  FILE *file = fopen(profile_path, "r");

  env->PATH = NULL;
  env->HOME = NULL;

  if (file) {

    char *line;

    while ((line = read_line(file)) != NULL) {
      if (strchr(line, '=') != NULL) {
        if (is_var_assignment("PATH", line)) {
          env->PATH = get_value(line);
        } else if (is_var_assignment("HOME", line)) {
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

// Returns the current working directory.
char* get_cwd() {
  char *cwd = malloc(MAX_CWD_SIZE * sizeof(char));
  return getcwd(cwd, MAX_CWD_SIZE);
}
