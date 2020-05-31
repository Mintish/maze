all:
	gcc -o maze maze.c

debug:
	gcc -o maze maze.c -D VERBOSE

tiles:
	gcc -o tiles tiles.c
