#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#define HEAP_MAXSIZE 65535
#define HEAP_ALLOC_MAXSIZE 1024

typedef struct {
  void *start;
  size_t size;
} Heap_Block;

// init heap
char heap[HEAP_MAXSIZE] = {0};
size_t heap_size = 0;

// allocated blocks
Heap_Block heap_alloced_blocks[HEAP_ALLOC_MAXSIZE] = {0};
size_t heap_alloc_size = 0;

void *heap_malloc(size_t size) { 
  // check if the size is within the limits of the heap memory
  assert(heap_size + size <= HEAP_MAXSIZE);

  // ptr to the heap block
  void* result = heap + heap_size;
  // size of the block
  heap_size += size;

  // check if the block fits into the allocated block limit
  assert(heap_alloc_size < HEAP_ALLOC_MAXSIZE);

  const Heap_Block block = {
    .start = result,
    .size = size,
  };

  heap_alloced_blocks[heap_alloc_size++] = block;
  return result;
}

void heap_print_alloc_blocks() {
  printf("Dumping heap chunks, length: %zu \n", heap_alloc_size);
  for (size_t i = 0; i < heap_alloc_size; i++) {
    printf("start: %p, size: %d\n",
        heap_alloced_blocks[i].start,
        heap_alloced_blocks[i].size
    );
  }
}

void heap_free(void *ptr);
void *realloc(void *ptr, size_t size);

int main() {
  //char *mem_root = heap_malloc(20);
  for (int i = 0; i < 20; i++) {
    heap_malloc(i);
  }
  heap_print_alloc_blocks();
  //heap_free(mem_root);
  return 0;
}
