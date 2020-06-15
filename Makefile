all:
	gcc -o maze dfs_maze_generator.c grid_maze_topology.c main.c 

debug:
	gcc -g -o maze dfs_maze_generator.c grid_maze_topology.c main.c 
