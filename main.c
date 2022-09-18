#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#define HEAP_MAXSIZE 65535
#define BLOCK_LIST_MAXSIZE 1024

typedef struct {
  void *start;
  size_t size;
} Heap_Block;

typedef struct {
  size_t length;
  Heap_Block blocks[BLOCK_LIST_MAXSIZE];
} Heap_Block_List;

void heap_block_list_remove(Heap_Block_List *list, void* ptr);

int heap_block_list_find(Heap_Block_List *list, void* ptr) {
  // naively searches through the heap list
  // returns index of an element if it matches the given pointer in "ptr"
  // O(n)
  // TODO: convert to a binary search if bored
  for (int i = 0; i < list->length; i++) {
    if (list->blocks[i].start == ptr) {
      return i;
    }
  }
  return -1;
}

void heap_block_list_sort(Heap_Block_List *list) {
  if (list->length == 0) {
    return;
  }

  // naively sort the blocks into a sequential order -- O(n)
  for (size_t i = list->length; i > 0; i--) {
    // check if the current ptr is bigger than the previous
    // if yes, stop
    if (list->blocks[i].start > list->blocks[i-1].start) {
      break;
    }
    // swap
    const Heap_Block hb = list->blocks[i];
    list->blocks[i] = list->blocks[i-1];
    list->blocks[i-1] = hb;
  }
}

void heap_block_list_insert(Heap_Block_List *list, void* ptr, size_t size) {
  assert(list->length < BLOCK_LIST_MAXSIZE);

  list->blocks[list->length].start = ptr;
  list->blocks[list->length].size = size;
  heap_block_list_sort(list);
  list->length += 1;
}


void heap_block_list_dump_stdout(Heap_Block_List *list) {
  printf("dumping heap block list of size: %zu\n", list->length);
  for (int i = 0; i < list->length; i++) {
    Heap_Block block = list->blocks[i];
    printf("start: %p, size: %zu\n",
        block.start,
        block.size
    );
  }
}

// init heap
char heap[HEAP_MAXSIZE] = {0};
size_t heap_size = 0;

Heap_Block_List heap_alloced_blocks = {0};
Heap_Block_List heap_freed_blocks = {0};

void *heap_malloc(size_t size) { 
  // TODO:
  // behave like malloc() and return a unique ptr if size == 0
  if (size <= 0) {
    return NULL;
  }
  // check if the size is within the limits of the heap memory
  assert(heap_size + size <= HEAP_MAXSIZE);

  // ptr to the heap block
  void* ptr = heap + heap_size;
  // size of the block
  heap_size += size;

  // allocate
  heap_block_list_insert(&heap_alloced_blocks, ptr, size);
  return ptr;
}

void heap_free(void *ptr) {
  if (ptr == NULL) {
    // if heap_malloc() is asked to allocate heap space with the size 0
    // it returns NULL value as the pointer
    return;
  }
  int index = heap_block_list_find(&heap_alloced_blocks, ptr);
  printf("found index: %d\n", index);
  // TODO:
  // Better error than just assert
  assert(index >= 0);

  Heap_Block b = heap_alloced_blocks.blocks[index];
  heap_block_list_insert(&heap_freed_blocks, b.start, b.size);
  // TODO:
  // heap_block_list_remove();
}
void *realloc(void *ptr, size_t size);

int main() {
  for (int i = 0; i < 10; i++) {
    void *p = heap_malloc(i);
    if (i % 2 == 0) {
      heap_free(p);
    }
  }
  heap_block_list_dump_stdout(&heap_alloced_blocks);
  heap_block_list_dump_stdout(&heap_freed_blocks);
  return 0;
}
