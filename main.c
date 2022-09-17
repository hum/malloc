#include <sys/types.h>
#include <stdio.h>

#define HEAP_MAXSIZE 65535

// init heap
char heap[HEAP_MAXSIZE] = {0};
size_t heap_size = 0;

void heap_malloc(size_t size) { }

void heap_free(void *ptr) { }

int main() {
  printf("hello, world\n");
  return 0;
}
