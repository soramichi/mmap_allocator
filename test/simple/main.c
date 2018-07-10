#include <stdio.h>
#include "../../mmap_allocator.h"

int main() {  
  int* ptr = (int*)mm_malloc_normal(sizeof(int) * 8);
  int i;

  ptr[0] = 1;
  for(i=1; i<8; i++) {
    ptr[i] = ptr[i-1] * 2;
  }

  for(i=0; i<8; i++) {
    printf("%d ", ptr[i]);
  }
  printf("\n");
  
  return 0;
}
