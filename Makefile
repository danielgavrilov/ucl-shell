INCLUDES=./includes/*.c ./includes/*.h
FLAGS=./includes/*.c -Wall -lm -I ./includes/

shell: main.c $(INCLUDES)
	gcc -o $@ main.c $(FLAGS)
