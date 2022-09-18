#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#define HEAP_MAXSIZE 2 << 15
#define BLOCK_LIST_MAXSIZE 2 << 15

typedef struct {
  void *start;
  size_t size;
} Heap_Block;

typedef struct {
  size_t length;
  Heap_Block blocks[BLOCK_LIST_MAXSIZE];
} Heap_Block_List;


// init heap
char heap[HEAP_MAXSIZE] = {0};

Heap_Block_List heap_alloced_blocks = {0};
Heap_Block_List heap_freed_blocks = {
  .length = 1,
  .blocks = {{.start = heap, .size = sizeof(heap)}}
};


void heap_defrag() {
  // TODO
  // Merge all fragmented pieces of the freed heap space back into the base
}

void heap_block_list_remove(Heap_Block_List *list, size_t index) {
  assert(index < list->length);

  // shift all elements of the array by 1
  // starting with the index given in the parameter
  // which causes the array to not have empty indices in the middle
  for (size_t i = index; i < list->length - 1; i--) {
    list->blocks[index] = list->blocks[index+1];
  }
  list->length--;
}

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
  heap_defrag();

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
  list->length++;
}

void heap_block_list_dump_stdout(Heap_Block_List *list) {
  printf("dumping heap block list of size: %zu\n", list->length);
  for (int i = 0; i < list->length; i++) {
    Heap_Block block = list->blocks[i];
    printf("index: %d, start: %p, size: %zu\n",
        i,
        block.start,
        block.size
    );
  }
}


void *heap_malloc(size_t size) { 
  // TODO:
  // behave like malloc() and return a unique ptr if size == 0
  if (size <= 0) {
    return NULL;
  }
  // check if the size is within the limits of the heap memory
  //assert(size <= heap_freed_blocks.blocks[0].size);

  for (size_t i = 0; i < heap_freed_blocks.length; i++) {
    if (!(size <= heap_freed_blocks.blocks[i].size)) {
      // freed block is smaller than the necessary size
      continue;
    }

    Heap_Block b = heap_freed_blocks.blocks[i];

    // remove free block
    heap_block_list_remove(&heap_freed_blocks, i);

    // allocate memory to alloced blocks
    heap_block_list_insert(&heap_alloced_blocks, b.start, size);

    // if the user allocated size is smaller than the heap block
    // we can allocate the size delta as a new free block
    if (b.size > size) {
      size_t size_delta = b.size - size;
      heap_block_list_insert(&heap_freed_blocks, b.start + size_delta, size_delta);
    }
    return b.start;
  }

  // this means we are out of memory
  // TODO: error?
  return NULL;
}

void heap_free(void *ptr) {
  if (ptr == NULL) {
    // if heap_malloc() is asked to allocate heap space with the size 0
    // it returns NULL value as the pointer
    return;
  }

  int index = heap_block_list_find(&heap_alloced_blocks, ptr);
  if (index < 0) {
    // invalid pointer to a heap memory
    return;
  }
  assert(ptr == heap_alloced_blocks.blocks[index].start);

  Heap_Block b = heap_alloced_blocks.blocks[index];
  heap_block_list_insert(&heap_freed_blocks, b.start, b.size);
  heap_block_list_remove(&heap_alloced_blocks, index);
}

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
