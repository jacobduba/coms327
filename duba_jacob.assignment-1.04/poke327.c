#include "sc_heap.h"
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WORLD_SIZE 401

#define CHUNK_X_WIDTH 80
#define CHUNK_Y_HEIGHT 21

// Out of 100
#define TERRAIN_NOISE 70

#define MAX_ATTEMPTS_TO_PLACE_BUILDING 1000

typedef struct cord {
        int x;
        int y;
} cord_t;

typedef enum terrain {
        EMPTY,
        BOULDER,
        FOREST,
        MOUNTAIN,
        TALL_GRASS,
        SHORT_GRASS,
        WATER,
        ROAD,
        GATE,
        POKEMON_CENTER,
        POKEMART
} land_t;

typedef enum direction { NORTH, SOUTH, EAST, WEST } dir_t;

typedef struct chunk {
        land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];
        int n_gate_x, s_gate_x, e_gate_y, w_gate_y;
} chunk_t;

char get_land_char(land_t land) {
        switch (land) {
        case EMPTY:
                return 'E';
        case BOULDER:
                return '%';
        case FOREST:
                return '^';
        case MOUNTAIN:
                return '%';
        case TALL_GRASS:
                return ':';
        case SHORT_GRASS:
                return '.';
        case WATER:
                return '~';
        case ROAD:
                return '#';
        case GATE:
                return '#';
        case POKEMON_CENTER:
                return 'C';
        case POKEMART:
                return 'M';
        }
}

void print_terrain(land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT], cord_t pc) {
        for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                        if (pc.x == x && pc.y == y) {
                                printf("@");
                        } else {
                                printf("%c", get_land_char(terrain[x][y]));
                        }
                }
                printf("\n");
        }
}

void place_seed_in_gen_queue(
    land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT], int *head,
    cord_t generation_queue[CHUNK_X_WIDTH * CHUNK_Y_HEIGHT], int x, int y,
    char type) {

        cord_t tall_grass_seed_1 = {rand() % CHUNK_X_WIDTH,
                                    rand() % CHUNK_Y_HEIGHT};

        terrain[tall_grass_seed_1.x][tall_grass_seed_1.y] = type;

        generation_queue[*head] = tall_grass_seed_1;

        (*head)++;
}

/*
 * Returns 1 if the pokemart has not been placed
 */
int try_place_building(land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                       char building) {
        cord_t c = {rand() % (CHUNK_X_WIDTH - 3) + 1,
                    rand() % (CHUNK_Y_HEIGHT - 3) + 1};

        // Buildings have 4 (b)uilding tiles b1, b2, b3, b4 layed out like
        // b1  b2
        // b3  b4

        land_t *b1 = &terrain[c.x][c.y];
        land_t *b2 = &terrain[c.x + 1][c.y];
        land_t *b3 = &terrain[c.x][c.y + 1];
        land_t *b4 = &terrain[c.x + 1][c.y + 1];

        if (*b1 != FOREST && *b1 != BOULDER && *b1 != TALL_GRASS &&
            *b1 != SHORT_GRASS) {
                return 1;
        }

        if (*b2 != FOREST && *b2 != BOULDER && *b2 != TALL_GRASS &&
            *b2 != SHORT_GRASS) {
                return 1;
        }

        if (*b3 != FOREST && *b3 != BOULDER && *b3 != TALL_GRASS &&
            *b3 != SHORT_GRASS) {
                return 1;
        }

        if (*b4 != FOREST && *b4 != BOULDER && *b4 != TALL_GRASS &&
            *b4 != SHORT_GRASS) {
                return 1;
        }

        if (terrain[c.x - 1][c.y] != ROAD &&
            terrain[c.x - 1][c.y + 1] != ROAD &&
            terrain[c.x][c.y - 1] != ROAD && terrain[c.x][c.y + 2] != ROAD &&
            terrain[c.x + 1][c.y - 1] != ROAD &&
            terrain[c.x + 2][c.y] != ROAD &&
            terrain[c.x + 2][c.y + 1] != ROAD) {
                return 1;
        }

        *b1 = building;
        *b2 = building;
        *b3 = building;
        *b4 = building;

        return 0;
}

int bfs_explore_generation(
    land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
    cord_t generation_queue[CHUNK_X_WIDTH * CHUNK_Y_HEIGHT * 2], int *head,
    int *tail) {

        while (*head - *tail > 0) {
                cord_t cur = generation_queue[(*tail)++];

                // TODO to reduce indents
                if (cur.x > 0 && terrain[cur.x - 1][cur.y] == EMPTY) {
                        if (rand() % 100 < TERRAIN_NOISE) {
                                terrain[cur.x - 1][cur.y] =
                                    terrain[cur.x][cur.y];
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x - 1, cur.y};
                        } else {
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x, cur.y};
                        }
                }

                if (cur.x < CHUNK_X_WIDTH - 1 &&
                    terrain[cur.x + 1][cur.y] == EMPTY) {
                        if (rand() % 100 < TERRAIN_NOISE) {
                                terrain[cur.x + 1][cur.y] =
                                    terrain[cur.x][cur.y];
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x + 1, cur.y};
                        } else {
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x, cur.y};
                        }
                }

                if (cur.y > 0 && terrain[cur.x][cur.y - 1] == EMPTY) {
                        if (rand() % 100 < TERRAIN_NOISE) {
                                terrain[cur.x][cur.y - 1] =
                                    terrain[cur.x][cur.y];
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x, cur.y - 1};
                        } else {
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x, cur.y};
                        }
                }

                if (cur.y < CHUNK_Y_HEIGHT - 1 &&
                    terrain[cur.x][cur.y + 1] == EMPTY) {
                        if (rand() % 100 < TERRAIN_NOISE) {
                                terrain[cur.x][cur.y + 1] =
                                    terrain[cur.x][cur.y];
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x, cur.y + 1};
                        } else {
                                generation_queue[(*head)++] =
                                    (cord_t){cur.x, cur.y};
                        }
                }
        }

        return 0;
}

int place_boulders(land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT]) {
        for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                        if (x == 0 || y == 0 || x == CHUNK_X_WIDTH - 1 ||
                            y == CHUNK_Y_HEIGHT - 1) {
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
                return 70;
        case WATER:
                return 100;
        case FOREST:
                return 25;
        case ROAD:
                return 5;
        default:
                return 100;
        }
}

void generate_path_explore_neighbor(
    land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
    int dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
    bool visited[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
    cord_t prev[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT], cord_t new_c, cord_t old_c,
    int d, struct sc_heap *heap) {
        if (1 > new_c.x || new_c.x >= CHUNK_X_WIDTH - 1 || 1 > new_c.y ||
            new_c.y >= CHUNK_Y_HEIGHT - 1) {
                return;
        }

        if (visited[new_c.x][new_c.y]) {
                return;
        }

        int terrain_cost = get_terrain_cost(terrain[new_c.x][new_c.y]);

        int new_d = d + terrain_cost;

        cord_t *new_c_heap;
        new_c_heap = malloc(sizeof(cord_t));
        *new_c_heap = (cord_t){new_c.x, new_c.y};

        if (new_d < dist[new_c.x][new_c.y]) {
                dist[new_c.x][new_c.y] = new_d;
                prev[new_c.x][new_c.y] = old_c;
                sc_heap_add(heap, new_d, new_c_heap);
        }
}

int gen_path_from_cords(land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                        cord_t start, cord_t end) {
        int dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];

        for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                        dist[x][y] = INT_MAX;
                }
        }

        cord_t prev[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];

        bool visited[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];
        for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                        visited[x][y] = false;
                }
        }

        struct sc_heap heap;

        sc_heap_init(&heap, CHUNK_X_WIDTH * CHUNK_Y_HEIGHT);

        sc_heap_add(&heap, 0, &start);

        dist[start.x][start.y] = 0;

        struct sc_heap_data *elem;
        while (sc_heap_size(&heap)) {
                elem = sc_heap_pop(&heap);
                cord_t c = *(cord_t *)elem->data;
                int d = elem->key;
                // free(elem->data);

                if (visited[c.x][c.y]) {
                        continue;
                }

                visited[c.x][c.y] = true;

                cord_t south = (cord_t){c.x, c.y + 1};
                generate_path_explore_neighbor(terrain, dist, visited, prev,
                                               south, c, d, &heap);
                cord_t north = (cord_t){c.x, c.y - 1};
                generate_path_explore_neighbor(terrain, dist, visited, prev,
                                               north, c, d, &heap);
                cord_t west = (cord_t){c.x - 1, c.y};
                generate_path_explore_neighbor(terrain, dist, visited, prev,
                                               west, c, d, &heap);
                cord_t east = (cord_t){c.x + 1, c.y};
                generate_path_explore_neighbor(terrain, dist, visited, prev,
                                               east, c, d, &heap);
        }

        cord_t cur = end;
        while (cur.x != start.x || cur.y != start.y) {
                terrain[cur.x][cur.y] = ROAD;
                cur = prev[cur.x][cur.y];
        }
        terrain[cur.x][cur.y] = ROAD;

        return 0;
}

int find_random_road(land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                     cord_t *cord) {
        int random_road_not_found;
        int x, y;

        random_road_not_found = 1;
        while (random_road_not_found) {
                x = rand() % (CHUNK_X_WIDTH - 2) + 1;
                y = rand() % (CHUNK_Y_HEIGHT - 2) + 1;

                if (terrain[x][y] == ROAD) {
                        cord->x = x;
                        cord->y = y;
                        random_road_not_found = 0;
                }
        }

        return 0;
}

/*
 * Draws paths between gates.
 *
 * n_gate_x, s_gate_x, w_gate_y, s_gate_y must be in range [1, CHUNK_X_WIDTH
 * - 2]. If not, paths will not be drawn to that exist
 */
int generate_paths(chunk_t *chunk, int n_gate_x, int s_gate_x, int w_gate_y,
                   int e_gate_y) {
        cord_t random_road_tile;
        int n_gate_exists, s_gate_exists, w_gate_exists, e_gate_exists;

        n_gate_exists = 0 < n_gate_x && n_gate_x < CHUNK_X_WIDTH - 1;
        s_gate_exists = 0 < s_gate_x && s_gate_x < CHUNK_X_WIDTH - 1;
        w_gate_exists = 0 < w_gate_y && w_gate_y < CHUNK_Y_HEIGHT - 1;
        e_gate_exists = 0 < e_gate_y && e_gate_y < CHUNK_Y_HEIGHT - 1;

        // If the pathfinder could explore the edges of the chunk, it could draw
        // a path through there. Thus, gates cannot start in the corners since
        // in order to be connected the path would have to go through the edges.

        if (n_gate_exists && s_gate_exists && w_gate_exists && e_gate_exists) {
                chunk->terrain[n_gate_x][0] = GATE;
                chunk->terrain[s_gate_x][CHUNK_Y_HEIGHT - 1] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){n_gate_x, 1},
                                    (cord_t){s_gate_x, CHUNK_Y_HEIGHT - 2});

                chunk->terrain[0][w_gate_y] = GATE;
                chunk->terrain[CHUNK_X_WIDTH - 1][e_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){1, w_gate_y},
                                    (cord_t){CHUNK_X_WIDTH - 2, e_gate_y});
        } else if (!n_gate_exists && s_gate_exists && w_gate_exists &&
                   e_gate_exists) {
                chunk->terrain[0][w_gate_y] = GATE;
                chunk->terrain[CHUNK_X_WIDTH - 1][e_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){1, w_gate_y},
                                    (cord_t){CHUNK_X_WIDTH - 2, e_gate_y});

                find_random_road(chunk->terrain, &random_road_tile);

                chunk->terrain[s_gate_x][CHUNK_Y_HEIGHT - 1] = GATE;
                gen_path_from_cords(chunk->terrain, random_road_tile,
                                    (cord_t){s_gate_x, CHUNK_Y_HEIGHT - 2});
        } else if (n_gate_exists && !s_gate_exists && w_gate_exists &&
                   e_gate_exists) {
                chunk->terrain[0][w_gate_y] = GATE;
                chunk->terrain[CHUNK_X_WIDTH - 1][e_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){1, w_gate_y},
                                    (cord_t){CHUNK_X_WIDTH - 2, e_gate_y});

                find_random_road(chunk->terrain, &random_road_tile);

                chunk->terrain[n_gate_x][0] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){n_gate_x, 1},
                                    random_road_tile);
        } else if (n_gate_exists && s_gate_exists && !w_gate_exists &&
                   e_gate_exists) {
                chunk->terrain[n_gate_x][0] = GATE;
                chunk->terrain[s_gate_x][CHUNK_Y_HEIGHT - 1] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){n_gate_x, 1},
                                    (cord_t){s_gate_x, CHUNK_Y_HEIGHT - 2});

                find_random_road(chunk->terrain, &random_road_tile);

                chunk->terrain[CHUNK_X_WIDTH - 1][e_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain, random_road_tile,
                                    (cord_t){CHUNK_X_WIDTH - 2, e_gate_y});
        } else if (n_gate_exists && s_gate_exists && w_gate_exists &&
                   !e_gate_exists) {
                chunk->terrain[n_gate_x][0] = GATE;
                chunk->terrain[s_gate_x][CHUNK_Y_HEIGHT - 1] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){n_gate_x, 1},
                                    (cord_t){s_gate_x, CHUNK_Y_HEIGHT - 2});

                find_random_road(chunk->terrain, &random_road_tile);

                chunk->terrain[0][w_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){1, w_gate_y},
                                    random_road_tile);

        } else if (n_gate_exists && !s_gate_exists && w_gate_exists &&
                   !e_gate_exists) {
                chunk->terrain[n_gate_x][0] = GATE;
                chunk->terrain[0][w_gate_y] = GATE;

                gen_path_from_cords(chunk->terrain, (cord_t){n_gate_x, 1},
                                    (cord_t){1, w_gate_y});
        } else if (n_gate_exists && !s_gate_exists && !w_gate_exists &&
                   e_gate_exists) {
                chunk->terrain[n_gate_x][0] = GATE;
                chunk->terrain[CHUNK_X_WIDTH - 1][e_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain, (cord_t){n_gate_x, 1},
                                    (cord_t){CHUNK_X_WIDTH - 2, e_gate_y});

        } else if (!n_gate_exists && s_gate_exists && w_gate_exists &&
                   !e_gate_exists) {
                chunk->terrain[s_gate_x][CHUNK_Y_HEIGHT - 1] = GATE;
                chunk->terrain[0][w_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain,
                                    (cord_t){s_gate_x, CHUNK_Y_HEIGHT - 2},
                                    (cord_t){1, w_gate_y});
        } else if (!n_gate_exists && s_gate_exists && !w_gate_exists &&
                   e_gate_exists) {
                chunk->terrain[s_gate_x][CHUNK_Y_HEIGHT - 1] = GATE;
                chunk->terrain[CHUNK_X_WIDTH - 1][e_gate_y] = GATE;
                gen_path_from_cords(chunk->terrain,
                                    (cord_t){s_gate_x, CHUNK_Y_HEIGHT - 2},
                                    (cord_t){CHUNK_X_WIDTH - 2, e_gate_y});
        }

        return 0;
}

/*
 * Populates chunk_t->terrain with random generation
 *
 * n_gate_x, s_gate_x, w_gate_y, s_gate_y must be in range [1, CHUNK_X_WIDTH
 * - 2]. If not, paths will not be drawn to that exist
 */
int generate_terrain(chunk_t *chunk, int place_pokemon_center,
                     int place_pokemart) {
        cord_t generation_queue[CHUNK_X_WIDTH * CHUNK_Y_HEIGHT * 2];
        int head, tail;
        int pokemon_center_not_placed;
        int pokemart_not_placed;
        int attempts_placed;

        for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                        chunk->terrain[x][y] = EMPTY;
                }
        }

        head = 0, tail = 0;

        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                TALL_GRASS);
        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                TALL_GRASS);
        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                SHORT_GRASS);
        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                SHORT_GRASS);
        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                WATER);
        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                MOUNTAIN);
        place_seed_in_gen_queue(chunk->terrain, &head, generation_queue,
                                rand() % CHUNK_X_WIDTH, rand() % CHUNK_Y_HEIGHT,
                                FOREST);

        bfs_explore_generation(chunk->terrain, generation_queue, &head, &tail);

        place_boulders(chunk->terrain);

        generate_paths(chunk, chunk->n_gate_x, chunk->s_gate_x, chunk->w_gate_y,
                       chunk->e_gate_y);

        if (place_pokemon_center) {
                attempts_placed = 0;
                pokemon_center_not_placed = 1;
                do {
                        pokemon_center_not_placed =
                            try_place_building(chunk->terrain, POKEMON_CENTER);
                } while (pokemon_center_not_placed &&
                         attempts_placed++ < MAX_ATTEMPTS_TO_PLACE_BUILDING);
        }

        if (place_pokemart) {
                attempts_placed = 0;
                pokemart_not_placed = 1;
                do {
                        pokemart_not_placed =
                            try_place_building(chunk->terrain, POKEMART);
                } while (pokemart_not_placed &&
                         attempts_placed++ < MAX_ATTEMPTS_TO_PLACE_BUILDING);
        }

        return 0;
}

/*
 * Gets gate coordinates of a chunk.
 * If chunk does not exist, return random value.
 * If cords are invalid, return -1.
 */
int get_gate_coordinates(chunk_t *world[WORLD_SIZE][WORLD_SIZE], cord_t chunk,
                         dir_t direction) {
        int range;

        if (chunk.x < 0 || WORLD_SIZE - 1 < chunk.x || chunk.y < 0 ||
            WORLD_SIZE - 1 < chunk.y) {
                return -1;
        }

        switch (direction) {
        case NORTH:
        case SOUTH:
                range = CHUNK_X_WIDTH;
                break;
        case WEST:
        case EAST:
                range = CHUNK_Y_HEIGHT;
                break;
        }

        if (!world[chunk.x][chunk.y]) {
                return rand() % (range - 2) + 1;
        }

        switch (direction) {
        case NORTH:
                return world[chunk.x][chunk.y]->n_gate_x;
        case SOUTH:
                return world[chunk.x][chunk.y]->s_gate_x;
        case WEST:
                return world[chunk.x][chunk.y]->w_gate_y;
        case EAST:
                return world[chunk.x][chunk.y]->e_gate_y;
        }
}

int create_chunk_if_not_exists(chunk_t *world[WORLD_SIZE][WORLD_SIZE],
                               cord_t cur_chunk) {
        chunk_t *chunk;
        int manhatten_dist;
        int n_gate_x, s_gate_x, w_gate_y, e_gate_y;
        int place_poke_center, place_pokemart;
        int prob_of_building;

        if (world[cur_chunk.x][cur_chunk.y]) {
                return 1;
        }

        chunk = malloc(sizeof(chunk_t));

        chunk->w_gate_y = get_gate_coordinates(
            world, (cord_t){cur_chunk.x - 1, cur_chunk.y}, EAST);
        chunk->e_gate_y = get_gate_coordinates(
            world, (cord_t){cur_chunk.x + 1, cur_chunk.y}, WEST);
        chunk->n_gate_x = get_gate_coordinates(
            world, (cord_t){cur_chunk.x, cur_chunk.y - 1}, SOUTH);
        chunk->s_gate_x = get_gate_coordinates(
            world, (cord_t){cur_chunk.x, cur_chunk.y + 1}, NORTH);

        manhatten_dist = abs(cur_chunk.x - WORLD_SIZE / 2) +
                         abs(cur_chunk.y - WORLD_SIZE / 2);
        if (manhatten_dist < 200) {
                prob_of_building = (float)-45 * manhatten_dist / 200 + 50;
        } else {
                prob_of_building = 5;
        }

        place_poke_center = rand() % 100 <= prob_of_building;
        place_pokemart = rand() % 100 <= prob_of_building;

        if (cur_chunk.x == 200 && cur_chunk.y == 200) {
                place_poke_center = 1;
                place_pokemart = 1;
        }

        world[cur_chunk.x][cur_chunk.y] = chunk;
        generate_terrain(chunk, place_poke_center, place_pokemart);

        return 0;
}

int get_land_cost_hiker(land_t type) {
        switch (type) {
        case EMPTY:
                return -1;
        case BOULDER:
                return -1;
        case ROAD:
                return 10;
        case POKEMART:
                return 50;
        case POKEMON_CENTER:
                return 50;
        case TALL_GRASS:
                return 15;
        case SHORT_GRASS:
                return 10;
        case MOUNTAIN:
                return 15;
        case FOREST:
                return -1;
        case WATER:
                return -1;
        case GATE:
                return -1;
        }
}

int get_land_cost_rival(land_t type) {
        switch (type) {
        case EMPTY:
                return -1;
        case BOULDER:
                return -1;
        case ROAD:
                return 10;
        case POKEMART:
                return 50;
        case POKEMON_CENTER:
                return 50;
        case TALL_GRASS:
                return 20;
        case SHORT_GRASS:
                return 10;
        case MOUNTAIN:
                return -1;
        case FOREST:
                return -1;
        case WATER:
                return -1;
        case GATE:
                return -1;
        }
}

void explore(struct sc_heap *heap,
             land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
             int dist_map[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT], cord_t cord, int cost,
             int (*get_land_cost)(land_t type)) {
        int new_land_cost = get_land_cost(terrain[cord.x][cord.y]);

        if (new_land_cost < 0) {
                return;
        }

        int travel_cost = new_land_cost + cost;

        cord_t *heap_cord = malloc(sizeof(cord_t));
        heap_cord->x = cord.x;
        heap_cord->y = cord.y;

        if (travel_cost < dist_map[cord.x][cord.y]) {
                dist_map[cord.x][cord.y] = travel_cost;
                sc_heap_add(heap, travel_cost, heap_cord);
        }
}

void generate_distance_map(int dist_map[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                           land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                           cord_t pc, int (*get_land_cost)(land_t type)) {
        for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                        dist_map[x][y] = INT_MAX;
                }
        }

        // dijstrkas

        struct sc_heap heap;
        struct sc_heap_data *elem;

        sc_heap_init(&heap, CHUNK_X_WIDTH * CHUNK_Y_HEIGHT);

        sc_heap_add(&heap, 0, &pc);
        dist_map[pc.x][pc.y] = 0;

        bool visited[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];

        for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                        visited[x][y] = false;
                }
        }

        while ((elem = sc_heap_pop(&heap)) != NULL) {
                cord_t *pos = (cord_t *)elem->data;

                int x = pos->x, y = pos->y;

                if (visited[x][y]) {
                        continue;
                }

                visited[x][y] = true;

                int cur_cost = dist_map[x][y];

                explore(&heap, terrain, dist_map, (cord_t){x - 1, y - 1},
                        cur_cost, get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x, y - 1}, cur_cost,
                        get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x + 1, y - 1},
                        cur_cost, get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x - 1, y}, cur_cost,
                        get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x + 1, y}, cur_cost,
                        get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x - 1, y + 1},
                        cur_cost, get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x, y + 1}, cur_cost,
                        get_land_cost);
                explore(&heap, terrain, dist_map, (cord_t){x + 1, y + 1},
                        cur_cost, get_land_cost);
        }
}

int main(int argc, char *argv[]) {
        chunk_t *world[WORLD_SIZE][WORLD_SIZE];
        cord_t cur_chunk;

        cord_t pc;
        int hiker_dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];
        int rival_dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];

        // char *command;
        // size_t size_of_commands;
        // int fly_input_x, fly_input_y;

        // int seed = time(NULL);
        int seed = 1708573486;
        printf("Using seed: %d\n", seed);
        srand(seed);

        for (int x = 0; x < WORLD_SIZE; x++) {
                for (int y = 0; y < WORLD_SIZE; y++) {
                        world[x][y] = NULL;
                }
        }

        cur_chunk.x = 200;
        cur_chunk.y = 200;

        create_chunk_if_not_exists(world, cur_chunk);

        do {
                pc.x = rand() % (CHUNK_X_WIDTH - 2) + 1;
                pc.y = rand() % (CHUNK_Y_HEIGHT - 2) + 1;
        } while (world[cur_chunk.x][cur_chunk.y]->terrain[pc.x][pc.y] != ROAD);

        print_terrain(world[cur_chunk.x][cur_chunk.y]->terrain, pc);

        generate_distance_map(hiker_dist,
                              world[cur_chunk.x][cur_chunk.y]->terrain, pc,
                              get_land_cost_hiker);

        for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                printf(" ");
                for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                        if (hiker_dist[x][y] == INT_MAX) {
                                printf("   ");
                        } else {
                                printf("%02d ", hiker_dist[x][y] % 100);
                        }
                }
                printf("\n");
        }

        generate_distance_map(rival_dist,
                              world[cur_chunk.x][cur_chunk.y]->terrain, pc,
                              get_land_cost_rival);

        for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                printf(" ");
                for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                        if (rival_dist[x][y] == INT_MAX) {
                                printf("   ");
                        } else {
                                printf("%02d ", rival_dist[x][y] % 100);
                        }
                }
                printf("\n");
        }

        // for (;;) {
        //         create_chunk_if_not_exists(world, cur_chunk);

        //         print_terrain(world[cur_chunk.x][cur_chunk.y]->terrain);

        //         printf(
        //             "(%d,%d) | Enter command: ", cur_chunk.x - WORLD_SIZE /
        //             2, cur_chunk.y - WORLD_SIZE / 2);

        //         getline(&command, &size_of_commands, stdin);

        //         switch (command[0]) {
        //         case 'n':
        //                 if (cur_chunk.y == 0) {
        //                         break;
        //                 }
        //                 cur_chunk.y--;
        //                 break;
        //         case 's':
        //                 if (cur_chunk.y == WORLD_SIZE - 1) {
        //                         break;
        //                 }
        //                 cur_chunk.y++;
        //                 break;
        //         case 'w':
        //                 if (cur_chunk.x == 0) {
        //                         break;
        //                 }
        //                 cur_chunk.x--;
        //                 break;
        //         case 'e':
        //                 if (cur_chunk.x == WORLD_SIZE - 1) {
        //                         break;
        //                 }
        //                 cur_chunk.x++;
        //                 break;
        //         case 'q':
        //                 return 0;
        //         case 'f':
        //                 if (sscanf(command, "f %d %d", &fly_input_x,
        //                            &fly_input_y) != 2) {
        //                         printf("Error: Fly input should be "
        //                                "formated "
        //                                "like 'f x y'.\n");
        //                         break;
        //                 }

        //                 if (fly_input_x < WORLD_SIZE / -2 ||
        //                     WORLD_SIZE / 2 < fly_input_x) {
        //                         printf("Error: x input out of bounds.\n");
        //                         break;
        //                 }

        //                 if (fly_input_y < WORLD_SIZE / -2 ||
        //                     WORLD_SIZE / 2 < fly_input_y) {
        //                         printf("Error: y input out of bounds.\n");
        //                         break;
        //                 }

        //                 cur_chunk.x = fly_input_x + WORLD_SIZE / 2;
        //                 cur_chunk.y = fly_input_y + WORLD_SIZE / 2;
        //                 break;
        //         default:
        //                 printf("Error: Command '%c' not found.\n",
        //                 command[0]); break;
        //         }

        //         printf("\n");
        // }
}
