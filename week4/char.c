#include <ctype.h>
#include <stdio.h>

char *words[] = {
    "apple",     "bonanza", "compiler",       "debugger", "emacs",
    "flags",     "git",     "hash",           "ignore",   "jpeg",
    "kotlin",    "linux",   "microprocessor", "neovim",   "output",
    "processor", "querry",  "rust",           "string",   "tuple",
    "undefined", "verilog", "what",           "xerox",    "yellow",
    "zip",
};

int main(int argc, char *argv[]) {
  printf("%c\n", 'A');
  printf("%c\n", 0x41);
  printf("%c\n", 65);
  printf("%c\n", 0101);

  printf("%c is for %s\n", argv[1][0], words[toupper(argv[1][0]) - 'A']);

  return 0;
}
