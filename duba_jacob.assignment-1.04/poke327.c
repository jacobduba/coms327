#include "sc_heap.h"
#include <getopt.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Length and height for the world
#define WORLD_SIZE 401

#define CHUNK_X_WIDTH 80
#define CHUNK_Y_HEIGHT 21

// Out of 100
#define TERRAIN_NOISE 70

#define MAX_ATTEMPTS_TO_PLACE_BUILDING 1000

#define DEFAULT_NUMTRAINERS 10

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

typedef enum gate_dir {
        NORTH_GATE,
        WEST_GATE,
        EAST_GATE,
        SOUTH_GATE,
} gate_dir_t;

typedef enum dir {
        NORTH_EAST,
        NORTH,
        NORTH_WEST,
        WEST,
        EAST,
        SOUTH_EAST,
        SOUTH,
        SOUTH_WEST
} dir_t;

#define NUM_DIRECTIONS 8

typedef enum entity_type {
        NO_ENTITY,
        PC,
        HIKER,
        RIVAL,
        PACER,
        WANDERER,
        SENTRY,
        EXPLORER,
        // SWIMMER
} entity_type_t;

typedef struct entity {
        entity_type_t entity_type;
        void *data;
} entity_t;

#define NUM_TRAINER_TYPES 6

typedef struct event {
        // int seq_number;
        cord_t pos;
} event_t;

typedef struct chunk {
        land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];
        int n_gate_x, s_gate_x, e_gate_y,
            w_gate_y; // TODO move out. Will never be used
        entity_t entities[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];
        struct sc_heap *event_queue;
        cord_t pc_pos;
} chunk_t;

char land_t_to_char(land_t land) {
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

char entity_type_t_to_char(entity_type_t entity_type) {
        switch (entity_type) {
        case NO_ENTITY:
                return 'E';
        case PC:
                return '@';
        case HIKER:
                return 'h';
        case RIVAL:
                return 'r';
        case PACER:
                return 'p';
        case WANDERER:
                return 'w';
        case SENTRY:
                return 's';
        case EXPLORER:
                return 'e';
                // case SWIMMER:
                //         return 'm';
        }
}

int get_land_cost_pc(land_t type) {
        switch (type) {
        case EMPTY:
                return -1;
        case BOULDER:
                return -1;
        case ROAD:
                return 10;
        case POKEMART:
                return 10;
        case POKEMON_CENTER:
                return 10;
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
                return 10;
        }
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

// int get_land_cost_swimmer(land_t type) {
//         switch (type) {
//         case EMPTY:
//                 return -1;
//         case BOULDER:
//                 return -1;
//         case ROAD:
//                 return -1;
//         case POKEMART:
//                 return -1;
//         case POKEMON_CENTER:
//                 return -1;
//         case TALL_GRASS:
//                 return -1;
//         case SHORT_GRASS:
//                 return -1;
//         case MOUNTAIN:
//                 return -1;
//         case FOREST:
//                 return -1;
//         case WATER:
//                 return 7;
//         case GATE:
//                 return -1;
//         }
// }

int get_land_cost_other(land_t type) {
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

int get_land_cost_wanderer(land_t type) {
        if (type == ROAD) {
                return -1;
        }

        return get_land_cost_other(type);
}

/**
 * If an entity is at pos, return entity.
 * Else return land at the pos.
 */
char get_char_for_pos(cord_t pos, chunk_t *chunk) {
        if (chunk->entities[pos.x][pos.y].entity_type != NO_ENTITY) {
                return entity_type_t_to_char(
                    chunk->entities[pos.x][pos.y].entity_type);
        }

        return land_t_to_char(chunk->terrain[pos.x][pos.y]);
}

void print_chunk(chunk_t *chunk) {
        for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                        printf("%c", get_char_for_pos((cord_t){x, y}, chunk));
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

int pathfinding_terrain_cost(char type) {
        switch (type) {
        case SHORT_GRASS:
                return 10;
        case TALL_GRASS:
                return 15;
        case MOUNTAIN:
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

        int terrain_cost = pathfinding_terrain_cost(terrain[new_c.x][new_c.y]);

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
                         gate_dir_t direction) {
        int range;

        if (chunk.x < 0 || WORLD_SIZE - 1 < chunk.x || chunk.y < 0 ||
            WORLD_SIZE - 1 < chunk.y) {
                return -1;
        }

        switch (direction) {
        case NORTH_GATE:
        case SOUTH_GATE:
                range = CHUNK_X_WIDTH;
                break;
        case WEST_GATE:
        case EAST_GATE:
                range = CHUNK_Y_HEIGHT;
                break;
        }

        if (!world[chunk.x][chunk.y]) {
                return rand() % (range - 2) + 1;
        }

        switch (direction) {
        case NORTH_GATE:
                return world[chunk.x][chunk.y]->n_gate_x;
        case SOUTH_GATE:
                return world[chunk.x][chunk.y]->s_gate_x;
        case WEST_GATE:
                return world[chunk.x][chunk.y]->w_gate_y;
        case EAST_GATE:
                return world[chunk.x][chunk.y]->e_gate_y;
        }
}

/**
 * Picks a random tile without another entity and where get_land_cost(land_t) !=
 * -1.
 * Returns position entity was spawned at.
 */
cord_t spawn_entity(chunk_t *chunk, entity_type_t entity_type,
                    int (*get_land_cost)(land_t), int *seq_number) {
        cord_t cord;
        do {
                cord.x = rand() % (CHUNK_X_WIDTH - 2) + 1;
                cord.y = rand() % (CHUNK_Y_HEIGHT - 2) + 1;
        } while (chunk->entities[cord.x][cord.y].entity_type != NO_ENTITY ||
                 get_land_cost(chunk->terrain[cord.x][cord.y]) == -1);

        entity_t entity;
        entity.entity_type = entity_type;

        if (entity_type == WANDERER || entity_type == PACER ||
            entity_type == EXPLORER) {
                entity.data = malloc(sizeof(dir_t));
                dir_t random_dir = rand() % NUM_DIRECTIONS;
                *((dir_t *)entity.data) = random_dir;
        }
        chunk->entities[cord.x][cord.y] = entity;

        event_t *event = malloc(sizeof(event_t));
        event->pos = cord;
        // event->seq_number = *seq_number;

        sc_heap_add(chunk->event_queue, *seq_number, event);

        (*seq_number)++;

        return cord;
}

int return_negative_one_if_not_road(land_t land) {
        if (land != ROAD) {
                return -1;
        }

        return 0;
}

int spawn_entities(chunk_t *chunk, int num_trainers) {
        int entity_count = 0;

        for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                        chunk->entities[x][y] = (entity_t){NO_ENTITY, NULL};
                }
        }

        cord_t pc_pos = spawn_entity(chunk, PC, return_negative_one_if_not_road,
                                     &entity_count);
        chunk->pc_pos = pc_pos;

        if (num_trainers < 1)
                return 0;

        spawn_entity(chunk, HIKER, get_land_cost_hiker, &entity_count);

        if (num_trainers < 2)
                return 0;

        spawn_entity(chunk, RIVAL, get_land_cost_rival, &entity_count);

        int entity_type;
        for (int i = 2; i < num_trainers; i++) {
                entity_type = rand() % NUM_TRAINER_TYPES;
                switch (entity_type) {
                case 0:
                        spawn_entity(chunk, HIKER, get_land_cost_hiker,
                                     &entity_count);
                        break;
                case 1:
                        spawn_entity(chunk, RIVAL, get_land_cost_rival,
                                     &entity_count);
                        break;
                case 2:
                        spawn_entity(chunk, PACER, get_land_cost_other,
                                     &entity_count);
                        break;
                case 3:
                        spawn_entity(chunk, WANDERER, get_land_cost_wanderer,
                                     &entity_count);
                        break;
                case 4:
                        spawn_entity(chunk, SENTRY, get_land_cost_other,
                                     &entity_count);
                        break;
                case 5:
                        spawn_entity(chunk, EXPLORER, get_land_cost_other,
                                     &entity_count);
                        break;
                        // case 6:
                        //         spawn_entity(chunk, SWIMMER,
                        //         get_land_cost_swimmer,
                        //                      &entity_count);
                        //         break;
                }
        }

        return 0;
}

int create_chunk_if_not_exists(chunk_t *world[WORLD_SIZE][WORLD_SIZE],
                               cord_t cur_chunk, int num_trainers) {
        chunk_t *chunk;
        int manhatten_dist;
        int n_gate_x, s_gate_x, w_gate_y, e_gate_y;
        int place_poke_center, place_pokemart;
        int prob_of_building;
        const int num_entities = num_trainers + 1;

        if (world[cur_chunk.x][cur_chunk.y]) {
                return 1;
        }

        chunk = malloc(sizeof(chunk_t));

        chunk->w_gate_y = get_gate_coordinates(
            world, (cord_t){cur_chunk.x - 1, cur_chunk.y}, EAST_GATE);
        chunk->e_gate_y = get_gate_coordinates(
            world, (cord_t){cur_chunk.x + 1, cur_chunk.y}, WEST_GATE);
        chunk->n_gate_x = get_gate_coordinates(
            world, (cord_t){cur_chunk.x, cur_chunk.y - 1}, SOUTH_GATE);
        chunk->s_gate_x = get_gate_coordinates(
            world, (cord_t){cur_chunk.x, cur_chunk.y + 1}, NORTH_GATE);

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

        generate_terrain(chunk, place_poke_center, place_pokemart);

        chunk->event_queue = malloc(sizeof(struct sc_heap));
        sc_heap_init(chunk->event_queue, num_entities);

        spawn_entities(chunk, num_trainers);

        world[cur_chunk.x][cur_chunk.y] = chunk;

        return 0;
}

void generate_dist_map_explore(struct sc_heap *heap,
                               land_t terrain[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                               int dist_map[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                               cord_t cord, int cost,
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
                           chunk_t *chunk, int (*get_land_cost)(land_t type)) {
        for (int x = 0; x < CHUNK_X_WIDTH; x++) {
                for (int y = 0; y < CHUNK_Y_HEIGHT; y++) {
                        dist_map[x][y] = INT_MAX;
                }
        }

        struct sc_heap heap;
        struct sc_heap_data *elem;

        sc_heap_init(&heap, CHUNK_X_WIDTH * CHUNK_Y_HEIGHT);

        cord_t pc = chunk->pc_pos;

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

                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x - 1, y - 1}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x, y - 1}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x + 1, y - 1}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x - 1, y}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x + 1, y}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x - 1, y + 1}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x, y + 1}, cur_cost,
                                          get_land_cost);
                generate_dist_map_explore(&heap, chunk->terrain, dist_map,
                                          (cord_t){x + 1, y + 1}, cur_cost,
                                          get_land_cost);
        }
}

void explore_tile_lowest_distance(chunk_t *cur_chunk,
                                  int hiker_dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                                  cord_t possible_next_cord,
                                  int *best_next_cord_dist,
                                  cord_t *best_next_cord) {
        if (hiker_dist[possible_next_cord.x][possible_next_cord.y] >=
            *best_next_cord_dist)
                return;

        if (cur_chunk->entities[possible_next_cord.x][possible_next_cord.y]
                .entity_type != NO_ENTITY)
                return;

        best_next_cord->x = possible_next_cord.x;
        best_next_cord->y = possible_next_cord.y;

        *best_next_cord_dist = hiker_dist[best_next_cord->x][best_next_cord->y];
}

void move_to_lowest_dist(int hiker_dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT],
                         cord_t entity_pos, chunk_t *cur_chunk,
                         int num_entities, int gt, int (*get_land_cost)(land_t),
                         entity_type_t entity_type) {
        cord_t next_cord = entity_pos;
        int lowest_dist = INT_MAX;

        explore_tile_lowest_distance(
            cur_chunk, hiker_dist, (cord_t){entity_pos.x - 1, entity_pos.y - 1},
            &lowest_dist, &next_cord);
        explore_tile_lowest_distance(cur_chunk, hiker_dist,
                                     (cord_t){entity_pos.x, entity_pos.y - 1},
                                     &lowest_dist, &next_cord);
        explore_tile_lowest_distance(
            cur_chunk, hiker_dist, (cord_t){entity_pos.x + 1, entity_pos.y - 1},
            &lowest_dist, &next_cord);
        explore_tile_lowest_distance(cur_chunk, hiker_dist,
                                     (cord_t){entity_pos.x - 1, entity_pos.y},
                                     &lowest_dist, &next_cord);
        explore_tile_lowest_distance(cur_chunk, hiker_dist,
                                     (cord_t){entity_pos.x + 1, entity_pos.y},
                                     &lowest_dist, &next_cord);
        explore_tile_lowest_distance(
            cur_chunk, hiker_dist, (cord_t){entity_pos.x - 1, entity_pos.y + 1},
            &lowest_dist, &next_cord);
        explore_tile_lowest_distance(cur_chunk, hiker_dist,
                                     (cord_t){entity_pos.x, entity_pos.y + 1},
                                     &lowest_dist, &next_cord);
        explore_tile_lowest_distance(
            cur_chunk, hiker_dist, (cord_t){entity_pos.x + 1, entity_pos.y + 1},
            &lowest_dist, &next_cord);

        cur_chunk->entities[entity_pos.x][entity_pos.y].entity_type = NO_ENTITY;
        cur_chunk->entities[next_cord.x][next_cord.y].entity_type = entity_type;

        event_t *new_event = malloc(sizeof(event_t));
        new_event->pos = next_cord;

        int cost_of_moving_to_new_cord =
            get_land_cost(cur_chunk->terrain[next_cord.x][next_cord.y]);

        if (lowest_dist != INT_MAX) {
                sc_heap_add(cur_chunk->event_queue,
                            gt + cost_of_moving_to_new_cord * num_entities,
                            new_event);
        }
}

cord_t find_tile_in_direction(cord_t cur, dir_t dir) {
        switch (dir) {
        case NORTH_EAST:
                return (cord_t){cur.x - 1, cur.y - 1};
        case NORTH:
                return (cord_t){cur.x, cur.y - 1};
        case NORTH_WEST:
                return (cord_t){cur.x + 1, cur.y - 1};
        case EAST:
                return (cord_t){cur.x - 1, cur.y};
        case WEST:
                return (cord_t){cur.x + 1, cur.y};
        case SOUTH_EAST:
                return (cord_t){cur.x - 1, cur.y + 1};
        case SOUTH:
                return (cord_t){cur.x, cur.y + 1};
        case SOUTH_WEST:
                return (cord_t){cur.x + 1, cur.y + 1};
        }
}

dir_t opposite_direction(dir_t dir) {
        switch (dir) {
        case NORTH_EAST:
                return SOUTH_WEST;
        case NORTH:
                return SOUTH;
        case NORTH_WEST:
                return SOUTH_EAST;
        case WEST:
                return EAST;
        case EAST:
                return WEST;
        case SOUTH_EAST:
                return NORTH_WEST;
        case SOUTH:
                return NORTH;
        case SOUTH_WEST:
                return NORTH_EAST;
        }
}

void move_pacer(entity_t entity, cord_t entity_pos, chunk_t *cur_chunk, int gt,
                int num_entities) {
        dir_t *dir = (dir_t *)entity.data;
        cord_t new_pos = find_tile_in_direction(entity_pos, *dir);

        if (get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]) ==
                -1 ||
            cur_chunk->entities[new_pos.x][new_pos.y].entity_type !=
                NO_ENTITY) {
                *dir = opposite_direction(*dir);
                new_pos = find_tile_in_direction(entity_pos, *dir);
        }

        // If this happens twice, the pacer is stuck. So just don't move.
        if (get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]) ==
                -1 ||
            cur_chunk->entities[new_pos.x][new_pos.y].entity_type !=
                NO_ENTITY) {
                new_pos = entity_pos;
        }

        cur_chunk->entities[entity_pos.x][entity_pos.y] =
            (entity_t){NO_ENTITY, NULL};
        cur_chunk->entities[new_pos.x][new_pos.y] = (entity_t){PACER, dir};

        int cost_of_moving_to_new_cord =
            get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]);

        event_t *new_event = malloc(sizeof(event_t));
        new_event->pos = new_pos;

        sc_heap_add(cur_chunk->event_queue,
                    gt + cost_of_moving_to_new_cord * num_entities, new_event);
}

void move_wanderer(entity_t entity, cord_t entity_pos, chunk_t *cur_chunk,
                   int gt, int num_entities) {
        dir_t *dir = (dir_t *)entity.data;
        cord_t new_pos = find_tile_in_direction(entity_pos, *dir);

        land_t new_land = cur_chunk->terrain[new_pos.x][new_pos.y];
        land_t cur_land = cur_chunk->terrain[entity_pos.x][entity_pos.y];

        // TODO replace with solution that saves wanders biome
        if (new_land != cur_land ||
            cur_chunk->entities[new_pos.x][new_pos.y].entity_type !=
                NO_ENTITY) {
                *dir = rand() % NUM_DIRECTIONS;
                new_pos = find_tile_in_direction(entity_pos, *dir);
        }

        // Half assed solution. If new tile cannot be traversed, don't move.
        new_land = cur_chunk->terrain[new_pos.x][new_pos.y];
        if (new_land != cur_land ||
            cur_chunk->entities[new_pos.x][new_pos.y].entity_type !=
                NO_ENTITY) {
                new_pos = entity_pos;
        }

        cur_chunk->entities[entity_pos.x][entity_pos.y] =
            (entity_t){NO_ENTITY, NULL};
        cur_chunk->entities[new_pos.x][new_pos.y] = (entity_t){WANDERER, dir};

        int cost_of_moving_to_new_cord =
            get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]);

        event_t *new_event = malloc(sizeof(event_t));
        new_event->pos = new_pos;

        sc_heap_add(cur_chunk->event_queue,
                    gt + cost_of_moving_to_new_cord * num_entities, new_event);
}

void move_explorer(entity_t entity, cord_t entity_pos, chunk_t *cur_chunk,
                   int gt, int num_entities) {
        dir_t *dir = (dir_t *)entity.data;
        cord_t new_pos = find_tile_in_direction(entity_pos, *dir);

        if (get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]) ==
                -1 ||
            cur_chunk->entities[new_pos.x][new_pos.y].entity_type !=
                NO_ENTITY) {
                *dir = rand() % NUM_DIRECTIONS;
                new_pos = find_tile_in_direction(entity_pos, *dir);
        }

        if (get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]) ==
                -1 ||
            cur_chunk->entities[new_pos.x][new_pos.y].entity_type !=
                NO_ENTITY) {
                new_pos = entity_pos;
        }

        cur_chunk->entities[entity_pos.x][entity_pos.y] =
            (entity_t){NO_ENTITY, NULL};
        cur_chunk->entities[new_pos.x][new_pos.y] = (entity_t){EXPLORER, dir};

        int cost_of_moving_to_new_cord =
            get_land_cost_other(cur_chunk->terrain[new_pos.x][new_pos.y]);

        event_t *new_event = malloc(sizeof(event_t));
        new_event->pos = new_pos;

        sc_heap_add(cur_chunk->event_queue,
                    gt + cost_of_moving_to_new_cord * num_entities, new_event);
}

int main(int argc, char *argv[]) {
        int seed;

        chunk_t *world[WORLD_SIZE][WORLD_SIZE];
        cord_t cur_chunk_pos;

        int hiker_dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];
        int rival_dist[CHUNK_X_WIDTH][CHUNK_Y_HEIGHT];

        int num_trainers = DEFAULT_NUMTRAINERS;

        int opt;
        int option_index = 0;

        struct option long_options[] = {
            {
                "numtrainers",
                required_argument,
                0,
                'n',

            },
        };

        while ((opt = getopt_long(argc, argv, "n:", long_options,
                                  &option_index)) != -1) {
                switch (opt) {
                case 'n':
                        num_trainers = atoi(optarg);
                        printf("num_entities=%d\n", num_trainers);
                        break;
                case ':':
                        fprintf(stderr, "Option -%c requires an argument.\n",
                                optopt);
                        return -1;
                case '?':
                        fprintf(stderr, "Unknown option: -%c\n", optopt);
                        return -1;
                }
        }

        seed = time(NULL);
        // seed = 1709193713;
        printf("Using seed: %d\n", seed);
        srand(seed);

        for (int x = 0; x < WORLD_SIZE; x++) {
                for (int y = 0; y < WORLD_SIZE; y++) {
                        world[x][y] = NULL;
                }
        }

        cur_chunk_pos.x = 200;
        cur_chunk_pos.y = 200;

        create_chunk_if_not_exists(world, cur_chunk_pos, num_trainers);

        const int num_entities = num_trainers + 1;

        chunk_t *cur_chunk = world[cur_chunk_pos.x][cur_chunk_pos.y];

        for (int gt = 0; 1; gt++) {
                generate_distance_map(hiker_dist, cur_chunk,
                                      get_land_cost_hiker);
                generate_distance_map(rival_dist, cur_chunk,
                                      get_land_cost_rival);

                if (sc_heap_peek(cur_chunk->event_queue)->key == gt) {
                        struct sc_heap_data *data =
                            sc_heap_pop(cur_chunk->event_queue);

                        event_t event = *(event_t *)data->data;
                        cord_t entity_pos = event.pos;
                        entity_t entity =
                            cur_chunk->entities[entity_pos.x][entity_pos.y];

                        // printf("%d at cords (%d, %d)\n", entity,
                        // entity_pos.x, entity_pos.y);

                        switch (entity.entity_type) {
                        case PC:
                                print_chunk(cur_chunk);
                                usleep(250000);

                                event_t *new_event = malloc(sizeof(event_t));
                                new_event->pos = entity_pos;

                                sc_heap_add(cur_chunk->event_queue,
                                            gt + num_entities * 10, new_event);
                                break;
                        case HIKER:
                                move_to_lowest_dist(hiker_dist, entity_pos,
                                                    cur_chunk, num_entities, gt,
                                                    get_land_cost_hiker, HIKER);
                                break;
                        case RIVAL:
                                move_to_lowest_dist(rival_dist, entity_pos,
                                                    cur_chunk, num_entities, gt,
                                                    get_land_cost_rival, RIVAL);
                                break;
                        case PACER:
                                move_pacer(entity, entity_pos, cur_chunk, gt,
                                           num_entities);
                                break;
                        case WANDERER:
                                move_wanderer(entity, entity_pos, cur_chunk, gt,
                                              num_entities);
                                break;
                        case EXPLORER:
                                move_explorer(entity, entity_pos, cur_chunk, gt,
                                              num_entities);
                                break;
                        default:
                                break;
                        }

                        // free(data->data);
                }
        }

        // char *command;
        // size_t size_of_commands;
        // int fly_input_x, fly_input_y;

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
