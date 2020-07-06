all:
	gcc -std=c11 -pedantic-errors -O3 -o maze dfs_maze_generator.c grid_maze_topology.c grid_maze_rooms_topology.c grid_maze_ppm_print.c main.c 

debug:
	gcc -std=c11 -pedantic-errors -g -O0 -o maze dfs_maze_generator.c grid_maze_topology.c grid_maze_rooms_topology.c grid_maze_ppm_print.c main.c 
