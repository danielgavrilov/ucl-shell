#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "args.h"
#include "utils.h"
#include "command.h"
#include "linked_list.h"

// Prints the prompt to the user and reads a line from stdin.
// Blocking until a line is read.
ssize_t prompt(char **line, size_t *size) {
  char *cwd = get_cwd();
  printf("%s > ", cwd);
  free(cwd);
  return getline(line, size, stdin);
}

// Implements cd. Returns 0 if sucessful.
int cd(char *path) {
  int result = chdir(path);
  if (result != 0) {
    fprintf(stderr, "cd: no such file or directory: %s\n", path);
  }
  return result;
}

int main() {

  struct Env *env = get_env("./profile");

  if (env->HOME == NULL) {
    fprintf(stderr, "HOME variable not set in profile. Exiting.\n");
    exit(1);
  }

  if (env->PATH == NULL) {
    fprintf(stderr, "PATH variable not set in profile. Exiting.\n");
    exit(1);
  }

  // if cd to HOME fails, cd to "/"
  if (cd(env->HOME) != 0) {
    cd("/");
  }

  // store the input line from the terminal
  char *line = NULL;
  size_t size;

  while (prompt(&line, &size) != -1) {

    char *input = remove_trailing_newline(line);

    // Handle "quit" and "exit" commands.
    if (streq(input, "exit") ||
        streq(input, "quit")) {
      exit(0);
    }

    // Handle variable assignment, e.g. $HOME=/var
    // Only supports "HOME" and "PATH".
    if (starts_with("$", input)) {

      char *variable = input+1;

      if (is_var_assignment("PATH", variable)) {
        free(env->PATH);
        env->PATH = get_after('=', variable);
      } else if (is_var_assignment("HOME", variable)) {
        free(env->HOME);
        env->HOME = get_after('=', variable);
      } else {
        printf("Trying to set unknown variable: '%s'. Only 'HOME' and 'PATH' may be used.\n", variable);
      }

    // If line does not start with "$", then treat is as a command
    } else {

      char *cmd = get_before(' ', input);

      if (strlen(cmd) == 0) {
        // do nothing
      } else if (streq(cmd, "cd")) {

        char *rest = get_after(' ', input);
        if (streq(rest, "~") || strlen(rest) == 0) {
          cd(env->HOME);
        } else {
          cd(rest);
        }
        free(rest);

      } else if (streq(cmd, "printenv")) {

        printf("PATH=%s\nHOME=%s\n", env->PATH, env->HOME);

      } else {

        char *path = find_executable(env->PATH, cmd);
        char **args = convert_to_args(input);

        // if executable is not found in PATH, check current directory
        if (path == NULL) {
          char *cwd = get_cwd();
          path = find_executable(cwd, cmd);
          free(cwd);
        }

        // if a directory containing the executable was found, then execute it.
        if (path != NULL) {
          execute(path, cmd, args);
          free(path);
        } else {
          fprintf(stderr, "command not found: %s\n", cmd);
        }

        free(args);
      }
      free(cmd);
    }
    free(input);
  }

  free(line);

  return 0;
}
