/* freemem.c
   implements freemem (free) for memory system
   CSE 374 HW6
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* Are there helper functions that only freemem needs?  Declare here. */
int is_adjacent(uintptr_t address1, uintptr_t address2,
                  size_t size1, size_t size2);
void combine_blocks(uintptr_t address1, uintptr_t address2);
void insert_node(uintptr_t address);

/* This method adds the block of storage at location p back into freelist
while keeping the ascending order of addresses in freelist. 
If p is null, returns immediately */
void freemem(void* p) {
  if (p == NULL) {
    return;
  }
  check_heap();

  uintptr_t address = (uintptr_t) p;
  address = address - NODESIZE;

  // insert the block back into freelist
  insert_node(address);
  check_heap();
}

/* This function adds the given block of memory back into freelist
and combines adjacent blocks */
void insert_node(uintptr_t address) {
  freeNode* curr = freelist;

  // if freelist is empty, add block to start of freelist
  if (curr == NULL) {
    freelist = (freeNode*) (address);

  // if address of given block is smaller than that of first block in freelist,
  // given block becomes new first block in freelist
  } else if ((uintptr_t) (curr) > address) {
    freeNode* new_node = (freeNode*) (address);
    new_node->next = freelist;
    freelist = new_node;

  // go through freelist and find address larger than given address
  // put given block before the larger address
  } else {
    while (curr->next != NULL && (uintptr_t) (curr->next) < address) {
      curr = curr->next;
    }
    freeNode* new_node = (freeNode*) (address);

    // if all blocks have smaller addresses,
    // tag given block of memory onto the end of freelist
    if (curr->next != NULL) {
      new_node->next = curr->next;
      curr->next = new_node;

      // check if new_node is is adjacent to any of the blocks
      // that points to it and it points to 
      // if adjacent, combine 
      if (is_adjacent((uintptr_t) curr, address, curr->size, new_node->size)) {
        combine_blocks((uintptr_t) (curr), address);
      }
      if (is_adjacent(address, (uintptr_t) new_node->next, new_node->size, new_node->next->size)) {
        combine_blocks(address, (uintptr_t) new_node->next);
      }
    } else {
      curr->next = (freeNode*) (address);
      if (is_adjacent((uintptr_t) curr, address, curr->size, new_node->size)) {
        combine_blocks((uintptr_t) (curr), address);
      }
    }
  }
}

// This function checks if two addresses are adjacent to each other
int is_adjacent(uintptr_t address1, uintptr_t address2,
                size_t size1, size_t size2) {
  if (address1 > address2) {
    return (address1 - address2 - size2) == NODESIZE + 1;
  } else {
    return (address2 - address1 - size1) == NODESIZE + 1;
  }
}

// This function combines the two addresses given
void combine_blocks(uintptr_t address1, uintptr_t address2) {
      freeNode* curr = (freeNode*) address1;
      freeNode* next_node = (freeNode*) address2;
      curr->size = curr->size + next_node->size + NODESIZE;
      curr->next = next_node->next;
}
