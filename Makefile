all:
	gcc -O3 -o maze dfs_maze_generator.c grid_maze_topology.c main.c 

debug:
	gcc -g -O0 -o maze dfs_maze_generator.c grid_maze_topology.c main.c 
