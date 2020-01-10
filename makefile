
all: boids boidsomp

boids: boids.c
	gcc boids.c -o boids -lncurses -lm -fopenmp

boidsomp: boids.c
	gcc boids.c -o boidsomp -lm -DNOGRAPHICS -fopenmp

