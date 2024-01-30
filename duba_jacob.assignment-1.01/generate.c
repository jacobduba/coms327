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

void print_terrain(char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH]) {
  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      printf("%c", terrain[y][x]);
    }
    printf("\n");
  }
}

void place_seed_in_gen_queue(char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH], int *head,
                             struct cord generation_queue[], int x, int y,
                             char type) {

  struct cord tall_grass_seed_1 = {rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT};

  terrain[tall_grass_seed_1.y][tall_grass_seed_1.x] = type;

  generation_queue[*head] = (struct cord){
      tall_grass_seed_1.x,
      tall_grass_seed_1.y,
  };

  *head += 1;
}

/*
 * Returns 1 if the pokemart has not been placed
 */
int try_place_pokemart_ret_1_if_fail(char terrain[MAP_Y_HEIGHT][MAP_X_WIDTH]) {
  struct cord c = {rand() % (MAP_X_WIDTH - 3) + 1,
                   rand() % (MAP_Y_HEIGHT - 3) + 1};

  if (terrain[c.y][c.x] == ROAD || terrain[c.y][c.x] == WATER ||
      terrain[c.y + 1][c.x] == ROAD || terrain[c.y][c.x] == WATER ||
      terrain[c.y][c.x + 1] == ROAD || terrain[c.y][c.x + 1] == WATER ||
      terrain[c.y + 1][c.x + 1] == ROAD || terrain[c.y + 1][c.x + 1] == WATER) {
    return 1;
  }

  // if (terrain[c.y - 1][c.x] != ROAD || terrain[c.y - 1][c.x - 1] != 1) {
  // }

  terrain[c.y][c.x] = POKEMON_CENTER;
  return 0;
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

  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT,
                          TALL_GRASS);
  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT,
                          TALL_GRASS);
  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT,
                          SHORT_GRASS);
  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT,
                          SHORT_GRASS);
  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT, WATER);
  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT, BOULDER);
  place_seed_in_gen_queue(terrain, &head, generation_queue,
                          rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT, TREE);

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
  }

  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      if (x == 0 || y == 0 || x == MAP_X_WIDTH - 1 || y == MAP_Y_HEIGHT - 1) {
        terrain[y][x] = BOULDER;
      }
    }
  }

  int west_path_y = rand() % (MAP_Y_HEIGHT - 2) + 1;
  int east_path_y = rand() % (MAP_Y_HEIGHT - 2) + 1;
  int vertical_travel_x = rand() % (MAP_X_WIDTH - 2) + 1;

  int cur_x = 0;
  int cur_y = west_path_y;

  while (cur_x != vertical_travel_x) {
    terrain[cur_y][cur_x] = ROAD;
    cur_x++;
  }

  while (cur_y != east_path_y) {
    terrain[cur_y][cur_x] = ROAD;
    cur_y += (east_path_y - west_path_y) / abs(east_path_y - west_path_y);
  }

  while (cur_x != MAP_X_WIDTH) {
    terrain[cur_y][cur_x] = ROAD;
    cur_x++;
  }

  int north_path_x = rand() % (MAP_X_WIDTH - 2) + 1;
  int south_path_x = rand() % (MAP_X_WIDTH - 2) + 1;
  int horizontal_travel_y = rand() % (MAP_Y_HEIGHT - 2) + 1;

  cur_x = north_path_x;
  cur_y = 0;

  while (cur_y != horizontal_travel_y) {
    terrain[cur_y][cur_x] = ROAD;
    cur_y++;
  }

  while (cur_x != south_path_x) {
    terrain[cur_y][cur_x] = ROAD;
    cur_x += (south_path_x - north_path_x) / abs(south_path_x - north_path_x);
  }

  while (cur_y != MAP_Y_HEIGHT) {
    terrain[cur_y][cur_x] = ROAD;
    cur_y++;
  }

  int pokemon_center_not_placed = 1;
  do {
    pokemon_center_not_placed = try_place_pokemart_ret_1_if_fail(terrain);
  } while (pokemon_center_not_placed);

  print_terrain(terrain);
}
