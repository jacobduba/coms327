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
};

void print(char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH]) {
  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      printf("%c", terrain[y][x]);
    }
    printf("\n");
  }
}

void place_seed(char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH], int *head,
                struct cord generation_queue[], int x, int y, char type) {

  struct cord tall_grass_seed_1 = {rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT};

  terrain[tall_grass_seed_1.y][tall_grass_seed_1.x] = type;

  generation_queue[*head] = (struct cord){
      tall_grass_seed_1.x,
      tall_grass_seed_1.y,
  };

  *head += 1;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH];

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      terrain[y][x] = EMPTY;
    }
  }

  struct cord generation_queue[MAP_X_WIDTH * MAP_Y_HEIGHT];

  int head, tail = 0;

  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, TALL_GRASS);
  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, TALL_GRASS);
  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, SHORT_GRASS);
  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, SHORT_GRASS);
  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, WATER);
  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, BOULDER);
  place_seed(terrain, &head, generation_queue, rand() % MAP_X_WIDTH,
             rand() % MAP_Y_HEIGHT, TREE);

  while (head - tail > 0) {
    struct cord cur = generation_queue[tail++];

    if (cur.x > 0 && terrain[cur.y][cur.x - 1] == EMPTY) {
      terrain[cur.y][cur.x - 1] = terrain[cur.y][cur.x];
      generation_queue[head++] = (struct cord){cur.x - 1, cur.y};
    }
    if (cur.x < MAP_X_WIDTH - 1 && terrain[cur.y][cur.x + 1] == EMPTY) {
      terrain[cur.y][cur.x + 1] = terrain[cur.y][cur.x];
      generation_queue[head++] = (struct cord){cur.x + 1, cur.y};
    }
    if (cur.y > 0 && terrain[cur.y - 1][cur.x] == EMPTY) {
      terrain[cur.y - 1][cur.x] = terrain[cur.y][cur.x];
      generation_queue[head++] = (struct cord){cur.x, cur.y - 1};
    }
    if (cur.y < MAP_Y_HEIGHT - 1 && terrain[cur.y + 1][cur.x] == EMPTY) {
      terrain[cur.y + 1][cur.x] = terrain[cur.y][cur.x];
      generation_queue[head++] = (struct cord){cur.x, cur.y + 1};
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
