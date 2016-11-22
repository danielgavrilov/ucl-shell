File structure
==============

main.c is the entry point. Utility functions are in the `includes` directory,
their responsibilities are below:

- args.c: Converting input strings to arguments to be passed to executables.

- command.c: Searching for executables in directories and executing them.

- env.c: Reading 'profile' and generating the struct storing the environment variables.

- linked_list.c: Linked list creation and manipulation.

- utils.c: Utility functions, mostly for manipulating strings.
