/* getmem.c
   implements getmem (malloc) for memory system
   CSE 374 HW6
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* This code will be filled in by the students.  Your primary job is to
   implement getmem */

/* initialize global variables ?*/
freeNode* freelist = NULL;
uintptr_t totalmalloc = 0;

/* Define your functions below: */
uintptr_t get_block(uintptr_t size);
void split_node(freeNode* node, uintptr_t size);
uintptr_t find_space(uintptr_t size);
uintptr_t more_space(uintptr_t size);
void insert(uintptr_t size, uintptr_t memory);
freeNode* new_freeNode(size_t size, uintptr_t memory, freeNode* next);
void remove_block(uintptr_t address);

/* The function returns a void* pointer to a block of storage the same
 size or a bit larger than the given size parameter.
 If size is less than or equal to 0, return NULL */
void* getmem(uintptr_t size) {
  check_heap();

  // checks that size is greater than 0
  // if not, return NULL
  if (size <= 0) {
    return NULL;
  }
  // check if size is multiple of 16
  // if not, make it multiple of 16
  size = size + (16 - size % 16);

  // gets a pointer to the block of memory
  uintptr_t address = get_block(size);
  check_heap();

  // remove the given block of memory from freelist
  remove_block(address);
  check_heap();

  return (void*) (address + NODESIZE);
}

// This method removes the given block from the freelist
void remove_block(uintptr_t address) {
  freeNode* curr = freelist;

  // if block is head of freelist, move head of freelist to next node
  if ((uintptr_t) (curr) == address) {
    freelist = curr->next;
  }

  // search through freelist for given block
  // once found, remove it from freelist
  while (curr->next != NULL) {
    if ((uintptr_t) (curr->next) == address) {
      curr->next = curr->next->next;
      return;
    }
    curr = curr->next;
  }
}

/* This functions gets the block of given size from the freelist.
  If freelist does not have enough memory, get more memory using malloc 
  If first block found is much bigger than given size, split it into 
  two blocks and return the first, smaller block */
uintptr_t get_block(uintptr_t size) {
  // find the address of a block of size in freelist
  uintptr_t address = find_space(size);

  // if no blocks in freelist are big enough,
  // get more space and insert into freelist
  if (address == 0) {
    address = more_space(size);
  }
  check_heap();

  freeNode* result = (freeNode*) (address);

  // while (curr_address != address && curr != NULL) {
  //   curr = curr->next;
  //   curr_address = (uintptr_t) (curr);
  // }

  // if the size of block found is significantly larger than size,
  // split the block into two blocks
  if (result != NULL && result->size > (size + NODESIZE + MINCHUNK)) {
    split_node(result, size);
  }
  check_heap();
  return address;
}

// This function splits a block of memory into two
void split_node(freeNode* node, uintptr_t size) {
  freeNode* new_node = (freeNode*)((uintptr_t)(node) + size + NODESIZE + 1);

  new_node->size = node->size - size - NODESIZE - 1;
  new_node->next = node->next;

  node->size = size;
  node->next = new_node;
}

/* This functions look through the freelist and tries to find a block size large.
  If block larger than size is found, return the block.
  Else return 0 */
uintptr_t find_space(uintptr_t size) {
  freeNode* curr = freelist;
  while (curr != NULL) {
    if (curr->size >= (size + NODESIZE)) {
      return (uintptr_t) (curr);
    }
    curr = curr->next;
  }
  return 0;
}

/* This functions mallocs more space since the freelist doesn't have 
  enough blocks to accomodate the required size.
  It returns the pointer to the new chunk that's added to freelist.
  The size of block is either BIGCHUNK or size (if size is larger than BIGCHUNK) */
uintptr_t more_space(uintptr_t size) {
  // sets size either to BIGCHUNK or size, whichever is bigger
  if (size < BIGCHUNK) {
    size = BIGCHUNK;
  } else {
    size = size + (16 - (size % 16));
  }

  // mallocs space that is size large
  uintptr_t memory = (uintptr_t) malloc(size + NODESIZE);
  if ((void*) memory == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    freelist = NULL;
    return 0;
  }
  totalmalloc += size + NODESIZE;

  // inserts the block of memory into freelist
  insert(size, memory);
  check_heap();
  return memory;
}

/* This method inserts a block of memory into freelist while 
  keeping freelist in ascending address order */
void insert(uintptr_t size, uintptr_t address) {
  freeNode* curr = freelist;

  // if freelist is empty, put new block at start of freelist
  if (curr == NULL) {
      freelist = new_freeNode(size, address, NULL);
  // if address of given block is smaller than that of first block in freelist,
  // given block becomes new first block in freelist
  } else if ((uintptr_t) (curr) > address) {
      freeNode* new_node = (freeNode*) (address);
      new_node->next = freelist;
      freelist = new_node;
  // insert given address into freelist
  // while maintaining ascending address order
  } else {
      while (curr->next != NULL && (uintptr_t) (curr->next) < address) {
         curr = curr->next;
      }
      if (curr->next != NULL) {
         curr->next = new_freeNode(size, address, curr->next);
      } else {
        // if all blocks in freelist have smaller addresses,
        // put given block at end of freelist
         curr->next = new_freeNode(size, address, NULL);
      }
  }
}

/* This method creates a new freeNode* object by setting its size and 
next pointer to whatever is given */
freeNode* new_freeNode(size_t size, uintptr_t memory, freeNode* next) {
  freeNode* new_node = (freeNode*) (memory);
  new_node->size = size;
  new_node->next = next;
  return new_node;
}
