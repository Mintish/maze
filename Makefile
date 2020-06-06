all:
	gcc -o maze maze.c main.c 

debug:
	gcc -g -o maze maze.c main.c 
