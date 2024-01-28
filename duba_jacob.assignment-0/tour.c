#include <stdio.h>

#define BOARD_SIZE 5
#define NUM_TILES (BOARD_SIZE * BOARD_SIZE)
#define NOT_VISITED 0
#define VISITED 1

void rec(int board[BOARD_SIZE][BOARD_SIZE], int path[NUM_TILES], int i, int j,
         int count) {
  board[i][j] = VISITED;
  path[count - 1] = (i * 5) + (j + 1);

  if (count == NUM_TILES) {
    int i;
    for (i = 0; i < NUM_TILES; i++) {
      printf("%d", path[i]);

      if (i == NUM_TILES - 1) {
        printf("\n");
      } else {
        printf(",");
      }
    }
  } else {
    if (i - 2 >= 0 && j - 1 >= 0 && board[i - 2][j - 1] == NOT_VISITED) {
      rec(board, path, i - 2, j - 1, count + 1);
    }
    if (i + 2 < BOARD_SIZE && j - 1 >= 0 &&
        board[i + 2][j - 1] == NOT_VISITED) {
      rec(board, path, i + 2, j - 1, count + 1);
    }
    if (i - 2 >= 0 && j + 1 < BOARD_SIZE &&
        board[i - 2][j + 1] == NOT_VISITED) {
      rec(board, path, i - 2, j + 1, count + 1);
    }
    if (i + 2 < BOARD_SIZE && j + 1 < BOARD_SIZE &&
        board[i + 2][j + 1] == NOT_VISITED) {
      rec(board, path, i + 2, j + 1, count + 1);
    }
    if (i - 1 >= 0 && j - 2 >= 0 && board[i - 1][j - 2] == NOT_VISITED) {
      rec(board, path, i - 1, j - 2, count + 1);
    }
    if (i + 1 < BOARD_SIZE && j - 2 >= 0 &&
        board[i + 1][j - 2] == NOT_VISITED) {
      rec(board, path, i + 1, j - 2, count + 1);
    }
    if (i - 1 >= 0 && j + 2 < BOARD_SIZE &&
        board[i - 1][j + 2] == NOT_VISITED) {
      rec(board, path, i - 1, j + 2, count + 1);
    }
    if (i + 1 < BOARD_SIZE && j + 2 < BOARD_SIZE &&
        board[i + 1][j + 2] == NOT_VISITED) {
      rec(board, path, i + 1, j + 2, count + 1);
    }
  }

  board[i][j] = NOT_VISITED;
  return;
}

int main(int argc, char *argv[]) {
  int board[BOARD_SIZE][BOARD_SIZE];

  int i;
  int j;
  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      board[i][j] = NOT_VISITED;
    }
  }

  int path[NUM_TILES];

  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      rec(board, path, i, j, 1);
    }
  }

  return 0;
}
