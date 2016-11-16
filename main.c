#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "args.h"
#include "utils.h"
#include "command.h"
#include "linked_list.h"

ssize_t prompt(char **line, size_t *size) {
  char *cwd = get_cwd();
  printf("%s > ", cwd);
  free(cwd);
  return getline(line, size, stdin);
}

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

  char *line = NULL;
  size_t size;

  while (prompt(&line, &size) != -1) {

    char *input = remove_trailing_newline(line);

    if (streq(input, "exit")) exit(0);

    if (starts_with("$", input)) {

      if (is_var_assignment("PATH", input+1)) {
        free(env->PATH);
        env->PATH = get_after('=', input+1);
      } else if (is_var_assignment("HOME", input+1)) {
        free(env->HOME);
        env->HOME = get_after('=', input+1);
      } else {
        printf("Unknown variable setting: %s\n", input);
      }

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

        if (path != NULL) {
          execute(path, cmd, args);
          free(path);
        } else {
          fprintf(stderr, "Couldn't locate: \"%s\". Check your PATH.\n", cmd);
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
