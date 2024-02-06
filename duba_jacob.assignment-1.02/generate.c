#include "sc_heap.h"
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_X_WIDTH 80
#define MAP_Y_HEIGHT 21

#define EMPTY 'E'
#define BOULDER '%'
#define TREE '^'
#define TALL_GRASS ':'
#define SHORT_GRASS '.'
#define WATER '~'
#define ROAD '#'
#define POKEMON_CENTER 'C'
#define POKEMART 'M'

struct cord {
  int x;
  int y;
};

void print_terrain(char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT]) {
  for (int y = 0; y < MAP_Y_HEIGHT; y++) {
    for (int x = 0; x < MAP_X_WIDTH; x++) {
      printf("%c", terrain[x][y]);
    }
    printf("\n");
  }
}

void place_seed_in_gen_queue(
    char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT], int *head,
    struct cord generation_queue[MAP_X_WIDTH * MAP_Y_HEIGHT], int x, int y,
    char type) {

  struct cord tall_grass_seed_1 = {rand() % MAP_X_WIDTH, rand() % MAP_Y_HEIGHT};

  terrain[tall_grass_seed_1.x][tall_grass_seed_1.y] = type;

  generation_queue[*head] = tall_grass_seed_1;

  (*head)++;
}

/*
 * Returns 1 if the pokemart has not been placed
 */
int try_place_building(char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT], char building) {
  struct cord c = {rand() % (MAP_X_WIDTH - 3) + 1,
                   rand() % (MAP_Y_HEIGHT - 3) + 1};

  if (terrain[c.x][c.y] == ROAD || terrain[c.x][c.y] == WATER ||
      terrain[c.x + 1][c.y] == ROAD || terrain[c.x][c.y] == WATER ||
      terrain[c.x][c.y + 1] == ROAD || terrain[c.x][c.y + 1] == WATER ||
      terrain[c.x + 1][c.y + 1] == ROAD || terrain[c.x + 1][c.y + 1] == WATER) {
    return 1;
  }

  if (terrain[c.x - 1][c.y] != ROAD && terrain[c.x - 1][c.y + 1] != ROAD &&
      terrain[c.x][c.y - 1] != ROAD && terrain[c.x][c.y + 2] != ROAD &&
      terrain[c.x + 1][c.y - 1] != ROAD && terrain[c.x + 2][c.y] != ROAD &&
      terrain[c.x + 2][c.y + 1] != ROAD) {
    return 1;
  }

  terrain[c.x][c.y] = building;
  terrain[c.x + 1][c.y] = building;
  terrain[c.x][c.y + 1] = building;
  terrain[c.x + 1][c.y + 1] = building;

  return 0;
}

int bfs_explore_generation(
    char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT],
    struct cord generation_queue[MAP_X_WIDTH * MAP_Y_HEIGHT], int *head,
    int *tail) {

  while (*head - *tail > 0) {
    struct cord cur = generation_queue[(*tail)++];

    if (cur.x > 0 && terrain[cur.x - 1][cur.y] == EMPTY) {
      terrain[cur.x - 1][cur.y] = terrain[cur.x][cur.y];
      generation_queue[(*head)++] = (struct cord){cur.x - 1, cur.y};
    }

    if (cur.x < MAP_X_WIDTH - 1 && terrain[cur.x + 1][cur.y] == EMPTY) {
      terrain[cur.x + 1][cur.y] = terrain[cur.x][cur.y];
      generation_queue[(*head)++] = (struct cord){cur.x + 1, cur.y};
    }

    if (cur.y > 0 && terrain[cur.x][cur.y - 1] == EMPTY) {
      terrain[cur.x][cur.y - 1] = terrain[cur.x][cur.y];
      generation_queue[(*head)++] = (struct cord){cur.x, cur.y - 1};
    }

    if (cur.y < MAP_Y_HEIGHT - 1 && terrain[cur.x][cur.y + 1] == EMPTY) {
      terrain[cur.x][cur.y + 1] = terrain[cur.x][cur.y];
      generation_queue[(*head)++] = (struct cord){cur.x, cur.y + 1};
    }
  }

  return 0;
}

int place_boulders(char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT]) {
  for (int x = 0; x < MAP_X_WIDTH; x++) {
    for (int y = 0; y < MAP_Y_HEIGHT; y++) {
      if (x == 0 || y == 0 || x == MAP_X_WIDTH - 1 || y == MAP_Y_HEIGHT - 1) {
        terrain[x][y] = BOULDER;
      }
    }
  }

  return 0;
}

int get_terrain_cost(char type) {
  switch (type) {
  case SHORT_GRASS:
    return 10;
  case TALL_GRASS:
    return 15;
  case BOULDER:
    return 40;
  case WATER:
    return 80;
  case TREE:
    return 25;
  default:
    return 100;
  }
}

void generate_path_explore_neighbor(char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT],
                                    int dist[MAP_X_WIDTH][MAP_Y_HEIGHT],
                                    bool visited[MAP_X_WIDTH][MAP_Y_HEIGHT],
                                    struct cord prev[MAP_X_WIDTH][MAP_Y_HEIGHT],
                                    struct cord new_c, struct cord old_c, int d,
                                    struct sc_heap *heap) {
  if (1 > new_c.x || new_c.x >= MAP_X_WIDTH - 1 || 1 > new_c.y ||
      new_c.y >= MAP_Y_HEIGHT - 1) {
    return;
  }

  if (visited[new_c.x][new_c.y]) {
    return;
  }

  int terrain_cost = get_terrain_cost(terrain[new_c.x][new_c.y]);

  int new_d = d + terrain_cost;

  // TODO possible memory leak?
  struct cord *new_c_heap;
  new_c_heap = malloc(sizeof(struct cord));
  *new_c_heap = (struct cord){new_c.x, new_c.y};
  // end

  if (new_d < dist[new_c.x][new_c.y]) {
    dist[new_c.x][new_c.y] = new_d;
    prev[new_c.x][new_c.y] = old_c;
    sc_heap_add(heap, new_d, new_c_heap);
  }
}

int generate_path(char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT], struct cord start,
                  struct cord end) {
  int dist[MAP_X_WIDTH][MAP_Y_HEIGHT];

  for (int x = 0; x < MAP_X_WIDTH; x++) {
    for (int y = 0; y < MAP_Y_HEIGHT; y++) {
      dist[x][y] = INT_MAX;
    }
  }

  struct cord prev[MAP_X_WIDTH][MAP_Y_HEIGHT];

  bool visited[MAP_X_WIDTH][MAP_Y_HEIGHT];
  for (int x = 0; x < MAP_X_WIDTH; x++) {
    for (int y = 0; y < MAP_Y_HEIGHT; y++) {
      visited[x][y] = false;
    }
  }

  struct sc_heap heap;

  sc_heap_init(&heap, MAP_X_WIDTH * MAP_Y_HEIGHT);

  sc_heap_add(&heap, 0, &start);

  dist[start.x][start.y] = 0;

  struct sc_heap_data *elem;
  while (sc_heap_size(&heap)) {
    elem = sc_heap_pop(&heap);
    struct cord c = *(struct cord *)elem->data;
    int d = elem->key;
    // free(elem->data);

    if (visited[c.x][c.y]) {
      continue;
    }

    visited[c.x][c.y] = true;

    struct cord south = (struct cord){c.x, c.y + 1};
    generate_path_explore_neighbor(terrain, dist, visited, prev, south, c, d,
                                   &heap);
    struct cord north = (struct cord){c.x, c.y - 1};
    generate_path_explore_neighbor(terrain, dist, visited, prev, north, c, d,
                                   &heap);
    struct cord west = (struct cord){c.x - 1, c.y};
    generate_path_explore_neighbor(terrain, dist, visited, prev, west, c, d,
                                   &heap);
    struct cord east = (struct cord){c.x + 1, c.y};
    generate_path_explore_neighbor(terrain, dist, visited, prev, east, c, d,
                                   &heap);
  }

  struct cord cur = end;
  while (cur.x != start.x || cur.y != start.y) {
    terrain[cur.x][cur.y] = ROAD;
    cur = prev[cur.x][cur.y];
  }
  terrain[cur.x][cur.y] = ROAD;

  return 0;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  char terrain[MAP_X_WIDTH][MAP_Y_HEIGHT];

  for (int x = 0; x < MAP_X_WIDTH; x++) {
    for (int y = 0; y < MAP_Y_HEIGHT; y++) {
      terrain[x][y] = EMPTY;
    }
  }

  struct cord generation_queue[MAP_X_WIDTH * MAP_Y_HEIGHT];
  int head = 0, tail = 0;

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

  bfs_explore_generation(terrain, generation_queue, &head, &tail);

  place_boulders(terrain);

  struct cord west_exit = (struct cord){0, rand() % (MAP_Y_HEIGHT - 2) + 1};
  struct cord east_exit =
      (struct cord){MAP_X_WIDTH - 1, rand() % (MAP_Y_HEIGHT - 2) + 1};
  terrain[west_exit.x][west_exit.y] = ROAD;
  terrain[east_exit.x][east_exit.y] = ROAD;
  // Roads are not allowed on the sides, except for the exits. Thus those are
  // manually placed.
  generate_path(terrain, (struct cord){west_exit.x + 1, west_exit.y},
                (struct cord){east_exit.x - 1, east_exit.y});

  struct cord north_exit = (struct cord){rand() % (MAP_X_WIDTH - 2) + 1, 0};
  struct cord south_exit =
      (struct cord){rand() % (MAP_X_WIDTH - 2) + 1, MAP_Y_HEIGHT - 1};
  terrain[north_exit.x][north_exit.y] = ROAD;
  terrain[south_exit.x][south_exit.y] = ROAD;
  // Roads are not allowed on the sides, except for the exits. Thus those are
  // manually placed.
  generate_path(terrain, (struct cord){north_exit.x, north_exit.y + 1},
                (struct cord){south_exit.x, south_exit.y - 1});

  // int west_path_y = rand() % (MAP_Y_HEIGHT - 2) + 1;
  // int east_path_y = rand() % (MAP_Y_HEIGHT - 2) + 1;
  // int vertical_travel_x = rand() % (MAP_X_WIDTH - 2) + 1;

  // int cur_x = 0;
  // int cur_y = west_path_y;

  // while (cur_x != vertical_travel_x) {
  //   terrain[cur_y][cur_x] = ROAD;
  //   cur_x++;
  // }

  // while (cur_y != east_path_y) {
  //   terrain[cur_y][cur_x] = ROAD;
  //   cur_y += (east_path_y - west_path_y) / abs(east_path_y - west_path_y);
  // }

  // while (cur_x != MAP_X_WIDTH) {
  //   terrain[cur_y][cur_x] = ROAD;
  //   cur_x++;
  // }

  // int north_path_x = rand() % (MAP_X_WIDTH - 2) + 1;
  // int south_path_x = rand() % (MAP_X_WIDTH - 2) + 1;
  // int horizontal_travel_y = rand() % (MAP_Y_HEIGHT - 2) + 1;

  // cur_x = north_path_x;
  // cur_y = 0;

  // while (cur_y != horizontal_travel_y) {
  //   terrain[cur_y][cur_x] = ROAD;
  //   cur_y++;
  // }

  // while (cur_x != south_path_x) {
  //   terrain[cur_y][cur_x] = ROAD;
  //   cur_x += (south_path_x - north_path_x) / abs(south_path_x -
  //   north_path_x);
  // }

  // while (cur_y != MAP_Y_HEIGHT) {
  //   terrain[cur_y][cur_x] = ROAD;
  //   cur_y++;
  // }

  int pokemon_center_not_placed = 1;
  do {
    pokemon_center_not_placed = try_place_building(terrain, POKEMON_CENTER);
  } while (pokemon_center_not_placed);

  int pokemart_not_placed = 1;
  do {
    pokemart_not_placed = try_place_building(terrain, POKEMART);
  } while (pokemart_not_placed);

  print_terrain(terrain);
}
