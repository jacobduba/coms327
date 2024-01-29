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
#define TALL_GRASS ':'
#define SHORT_GRASS '.'
#define WATER '~'

struct cord {
  int x;
  int y;
  char type;
};

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

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      terrain[y][x] = EMPTY;
    }
  }

  struct cord generation_queue[MAP_X_WIDTH * MAP_Y_HEIGHT * 4];

  int head, tail = 0;

  int tall_grass_seed_1_x = rand() % MAP_X_WIDTH;
  int tall_grass_seed_1_y = rand() % MAP_Y_HEIGHT;
  generation_queue[head++] =
      (struct cord){tall_grass_seed_1_x, tall_grass_seed_1_y, TALL_GRASS};

  int tall_grass_seed_2_x = rand() % MAP_X_WIDTH;
  int tall_grass_seed_2_y = rand() % MAP_Y_HEIGHT;
  generation_queue[head++] =
      (struct cord){tall_grass_seed_2_x, tall_grass_seed_2_y, TALL_GRASS};

  int water_seed_x = rand() % MAP_X_WIDTH;
  int water_seed_y = rand() % MAP_Y_HEIGHT;
  generation_queue[head++] = (struct cord){water_seed_x, water_seed_y, WATER};

  int clearing_seed_1_x = rand() % MAP_X_WIDTH;
  int clearing_seed_1_y = rand() % MAP_Y_HEIGHT;
  generation_queue[head++] =
      (struct cord){clearing_seed_1_x, clearing_seed_1_y, SHORT_GRASS};

  int clearing_seed_2_x = rand() % MAP_X_WIDTH;
  int clearing_seed_2_y = rand() % MAP_Y_HEIGHT;
  generation_queue[head++] =
      (struct cord){clearing_seed_2_x, clearing_seed_2_y, SHORT_GRASS};

  int mountain_seed_x = rand() % MAP_X_WIDTH;
  int mountain_seed_y = rand() % MAP_Y_HEIGHT;
  generation_queue[head++] =
      (struct cord){mountain_seed_x, mountain_seed_y, BOULDER};

  while (head - tail > 0) {
    struct cord cur = generation_queue[tail++];

    // printf("y: %d, x: %d, type: %c\n", cur.y, cur.x, cur.type);

    if (terrain[cur.y][cur.x] != EMPTY) {
      continue;
    }

    terrain[cur.y][cur.x] = cur.type;

    if (cur.x > 0 && terrain[cur.y][cur.x - 1] == EMPTY) {
      generation_queue[head++] = (struct cord){cur.x - 1, cur.y, cur.type};
    }
    if (cur.x < MAP_X_WIDTH - 1 && terrain[cur.y][cur.x + 1] == EMPTY) {
      generation_queue[head++] = (struct cord){cur.x + 1, cur.y, cur.type};
    }
    if (cur.y > 0 && terrain[cur.y - 1][cur.x] == EMPTY) {
      generation_queue[head++] = (struct cord){cur.x, cur.y - 1, cur.type};
    }
    if (cur.y < MAP_Y_HEIGHT - 1 && terrain[cur.y + 1][cur.x] == EMPTY) {
      generation_queue[head++] = (struct cord){cur.x, cur.y + 1, cur.type};
    }
    // print(terrain);
  }

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      if (x == 0 || y == 0 || x == MAP_X_WIDTH - 1 || y == MAP_Y_HEIGHT - 1) {
        terrain[y][x] = BOULDER;
      }
    }
  }

  print(terrain);
}
