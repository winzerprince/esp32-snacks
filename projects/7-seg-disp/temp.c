#include <stdio.h>

int main() {

  char c;
  while ((c = getchar()) != EOF) {
    if (c == '\n')
      ;
    else {

      printf("%d\n", c);
    }
  }
}
