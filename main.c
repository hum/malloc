#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#define HEAP_MAXSIZE 65535

// init heap
char heap[HEAP_MAXSIZE] = {0};
size_t heap_size = 0;

void *heap_malloc(size_t size) { 
  assert(heap_size + size <= HEAP_MAXSIZE);

  void* result = heap + heap_size;
  heap_size += size;
  return result;
}

void heap_free(void *ptr);
void *realloc(void *ptr, size_t size);

int main() {
  char *mem_root = heap_malloc(20);
  for (int i = 0; i < 20; i++) {
    mem_root[i] = 'A' + i;
    printf("%c", mem_root[i]);
  }
  printf("\n");
  return 0;
}
