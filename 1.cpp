#include <stdio.h>
#include <stdlib.h>

int main() {
  while(true) {
    int i;
    scanf("%d", &i);

    if (i == 42) {
      break;
    }

    printf("%d\n", i);
  }
}
