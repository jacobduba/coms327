#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_X_WIDTH 80
#define MAP_Y_HEIGHT 21

#define EMPTY 'E'
#define BOULDER '%'
#define TREE '^'
#define ROAD '#'
#define POKEMON_CENTER 'C'
#define POKEMART 'M'
#define LONG_GRASS ':'
#define SHORT_GRASS '.'
#define WATER '~'

void print(char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH]) {
  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      printf("%c", terrain[y][x]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH];

  int empty_tiles = 0;

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      terrain[y][x] = EMPTY;
      empty_tiles++;
    }
  }

  // TODO Refactor into function

  int tall_grass_seed_1_x = rand() % (MAP_X_WIDTH - 2);
  int tall_grass_seed_1_y = rand() % (MAP_Y_HEIGHT - 2);
  terrain[tall_grass_seed_1_y + 1][tall_grass_seed_1_x + 1] = LONG_GRASS;
  empty_tiles--;

  int tall_grass_seed_2_x = rand() % (MAP_X_WIDTH - 2);
  int tall_grass_seed_2_y = rand() % (MAP_Y_HEIGHT - 2);
  terrain[tall_grass_seed_2_y + 1][tall_grass_seed_2_x + 1] = LONG_GRASS;
  empty_tiles--;

  int water_seed_x = rand() % (MAP_X_WIDTH - 2);
  int water_seed_y = rand() % (MAP_Y_HEIGHT - 2);
  terrain[water_seed_y + 1][water_seed_x + 1] = WATER;
  empty_tiles--;

  int clearing_seed_1_x = rand() % (MAP_X_WIDTH - 2);
  int clearing_seed_1_y = rand() % (MAP_Y_HEIGHT - 2);
  terrain[clearing_seed_1_y + 1][clearing_seed_1_x + 1] = SHORT_GRASS;
  empty_tiles--;

  int clearing_seed_2_x = rand() % (MAP_X_WIDTH - 2);
  int clearing_seed_2_y = rand() % (MAP_Y_HEIGHT - 2);
  terrain[clearing_seed_2_y + 1][clearing_seed_2_x + 1] = SHORT_GRASS;
  empty_tiles--;

  // end

  print(terrain);

  // while (empty_tiles != 0) {
  //   for (int y = 0; y < MAP_Y_HEIGHT; y++) {
  //     for (int x = 0; x < MAP_X_WIDTH; x++) {
  //       if (terrain[y][x] == EMPTY) {
  //         if (y - 1 >= 0 && terrain[y - 1][x] != EMPTY) {
  //           terrain[y][x] = terrain[y - 1][x];
  //           empty_tiles--;
  //         } else if (y + 1 < MAP_Y_HEIGHT && terrain[y + 1][x] != EMPTY) {
  //           terrain[y][x] = terrain[y + 1][x];
  //           empty_tiles--;
  //         } else if (x - 1 >= 0 && terrain[y][x - 1] != EMPTY) {
  //           terrain[y][x] = terrain[y][x - 1];
  //           empty_tiles--;
  //         } else if (x + 1 < MAP_X_WIDTH && terrain[y][x + 1] != EMPTY) {
  //           terrain[y][x] = terrain[y][x + 1];
  //           empty_tiles--;
  //         }
  //       }
  //     }
  //   }
  // }

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      if (x == 0 || y == 0 || x == MAP_X_WIDTH - 1 || y == MAP_Y_HEIGHT - 1) {
        terrain[y][x] = BOULDER;
      }
    }
  }

  print(terrain);
}
