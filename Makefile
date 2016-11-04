INCLUDES=./includes/*.c ./includes/*.h
FLAGS=./includes/*.c -Wall -lm -I ./includes/

main.o: main.c $(INCLUDES)
	gcc -o $@ main.c $(FLAGS)
