// Copyright (2023) Cidy Li
/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"
#include <limits.h>

/* print_stats is a helper function that prints performance
   information about the memory system.  Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats()
*/
void print_stats(clock_t start);

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size);

/* Synopsis:   bench (main)
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/
int main(int argc, char** argv ) {
  // Initialize the parameters
  int NTRIALS = 10000;
  int PCTGET = 50;
  int PCTLARGE = 10;
  int SMALL_L = 200;
  int LARGE_L = 20000;
  int RANDOM_SEED = 0;

  // setting RANDOM_SEED to current time
  time_t curr_time = time(NULL);
  RANDOM_SEED = (int) (curr_time % INT_MAX);

  /* write code to set the above values based on the input arguments
     if there is not a corresponding input argument use the
     default value in the spec. */
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (i == 1) {
        NTRIALS = atoi(argv[i]);
      } else if (i == 2) {
        PCTGET = atoi(argv[i]);
      } else if (i == 3) {
        PCTLARGE = atoi(argv[i]);
      } else if (i == 4) {
        SMALL_L = atoi(argv[i]);
      } else if (i == 5) {
        LARGE_L = atoi(argv[i]);
      } else if (i == 6) {
        RANDOM_SEED = atoi(argv[i]);
      }
    }
  }

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];  // upperbound block storage
  int ntrials = 0, nblocks = 0;
  clock_t start = clock();
  int size = 0;

  // initialize rand
  srand(RANDOM_SEED);

  // perform NTRIALS mem operations
  while (ntrials < NTRIALS) {
    // if randomly generated number is smaller than
    // percent of get, allocate memory
    if (rand() % 100 < PCTGET) {
      void* ptr;

      // if randomly generated number is less than the
      // percent of large blocks, make a large block
      if (rand() % 100 < PCTLARGE) {
        size_t block_size = (size_t)
          (rand() % (LARGE_L - SMALL_L + 1)) + SMALL_L;
        ptr = getmem(block_size);
        fill_mem(ptr, size);

      // make a small block
      } else {
        size_t block_size = (size_t) (rand() % SMALL_L + 1);
        ptr = getmem(block_size);
        fill_mem(ptr, size);
      }

      // set pointer to array of pointers
      blocks[nblocks] = ptr;
      nblocks++;

    // free memory
    } else {
      if (size > 0) {
        int index = rand() % size;
        freemem(blocks[index]);
        blocks[index] = blocks[nblocks - 1];
        nblocks--;
      }
    }

    // if the current trial is at some 10% of the
    // total number of trials (e.g. 10%, 20%, 30%, etc.),
    // print current stats
    int ten_percent = NTRIALS / 10;
    if (ten_percent != 0) {
      if ((ntrials + 1) % ten_percent == 0) {
        print_stats(start);
      }
    }
    ntrials++;
  }

  return EXIT_SUCCESS;
}

/* Define helper functions here. */

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size) {
  unsigned char* block_ptr = (unsigned char*) ptr;

  // if size of block is larger than 16,
  // only set first 16 bytes
  if (size > 16) {
    size = 16;
  }

  // set bytes to 0xFE
  for (int i = 0 ; i < size; i++) {
    block_ptr[i] = 0xFE;
  }
}

/* print_stats is a helper function that prints performance
   information about the memory system.  Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats()
*/
void print_stats(clock_t start) {
  // initialize values
  clock_t end = clock();
  uintptr_t* total_size = (uintptr_t*) malloc(sizeof(uintptr_t*));
  uintptr_t* total_free = (uintptr_t*) malloc(sizeof(uintptr_t*));
  uintptr_t* free_blocks = (uintptr_t*) malloc(sizeof(uintptr_t*));

  // get stats and calculate CPU time from start
  get_mem_stats(total_size, total_free, free_blocks);
  double cpu_time = (double) (end - start) / CLOCKS_PER_SEC;

  // calculate average blocks freed
  int average_free;
  if (free_blocks == 0) {
    average_free = *total_free;
  } else {
    average_free = *total_free / *free_blocks;
  }

  // print to stdout
  printf("CPU time taken: %.10f\n", cpu_time);
  printf("Total amount of storage used: %lu\n", *total_size);
  printf("Total number of blocks freed: %lu\n", *total_free);
  printf("Average number of bytes in the freed storage: %d\n", average_free);
  printf("\n");

  free(total_size);
  free(total_free);
  free(free_blocks);
}
