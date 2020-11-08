#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  for (int i = 0; i < atoi(argv[1]); i++) {
    std::cout << i << " " << std::flush;
  }
  return 0;
}
