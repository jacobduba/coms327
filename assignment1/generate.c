#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_X_WIDTH 80
#define MAP_Y_HEIGHT 21

#define NUM_TERRAIN_TYPES 7
#define MOUNTAIN '%'
#define TREE '^'
#define ROAD '#'
#define POKEMON_CENTER 'C'
#define POKEMART 'M'
#define LONG_GRASS ':'
#define CLEARING '.'

int main(int argc, char *argv[]) {
  srand(time(NULL));

  char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH];

  char temp_num_to_terrain_type[] = {
      MOUNTAIN, TREE, ROAD, POKEMON_CENTER, POKEMART, LONG_GRASS, CLEARING};

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      int random_number = rand() % NUM_TERRAIN_TYPES;
      terrain[y][x] = temp_num_to_terrain_type[rand() % NUM_TERRAIN_TYPES];
    }
  }

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      printf("%c", terrain[y][x]);
    }
    printf("\n");
  }
}
