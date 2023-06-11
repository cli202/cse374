// Cindy Li
// CSE 374
// HW5
// 5/20/2023

/* tnine is a program that drives a trie / t9 program.  This code
   will build a trie, according to trienode.  It will also run
   an interactive session where the user can retrieve words using
   t9 sequences.
   CSE374, HW5, 22wi 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "trienode.h"

// run_session run the on-going decoding interaction with the
// user.  It requires a previously build wordTrie to work.
void run_session(trieNode *wordTrie);

int main(int argc, char **argv) {
  FILE *dictionary = NULL;
  trieNode *wordTrie = NULL;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [DICTIONARY]\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    dictionary = fopen(argv[1], "r");
    if (!dictionary) {
      fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  }

  // build the trie
  wordTrie = build_tree(dictionary);
  // fclose(dictionary);
  
  // run interactive session
  // initializing variables
  char* input = (char*) malloc(MAXLEN);
  assert(input != NULL);
  char* word = NULL;
  char* prev_word = (char*) malloc(MAXLEN);
  assert(prev_word != NULL);
  
  // prompt user for input
  printf("Enter \"exit\" to quit.\n");
  printf("Enter Key Sequence (or \"#\" for next word):\n");

  // while user input is not longer than MAXLEN and does not
  // equal 'exit', keep running session
  while (fgets(input, MAXLEN, stdin) &&
	 strncmp(input, "exit\n", strlen(input)) != 0) {

     // if user inputs a number (with or without # at end)
     // change all numbers & # into trie readable indices 
     if (isdigit(input[0])) {
       input[strlen(input) - 1] = '\0'; 
       for (int i = 0; i < strlen(input); i++) {
         if (input[i] == '#') {
           input[i] = '8';
         } else {
           input[i] = input[i] - '2' + '0';
	       }
       }
       // call corresponding word
       word = get_word(wordTrie, input);
    // else if user just imputs #
    } else {
      // copy previous word into input and add index 8 to the end
      strncpy(input, prev_word, MAXLEN);
      strncat(input, "8\0", 2);
      // get the word
      word = get_word(wordTrie, input);
    }  

    // if word is not null, print it out
    // else print there's no words left
    if (word) {
      printf("%s\n", word);
    } else {
      printf("There are no more T9onyms\n");
    }

    // setup for next iteration
    prev_word = strncpy(prev_word, input, MAXLEN);
    printf("Enter \"exit\" to quit.\n");
    printf("Enter Key Sequence (or \"#\" for next word):\n");
  }

  // clean up
  fclose(dictionary);
  free(input);
  free(prev_word);
  free_tree(wordTrie);

  // webpages used for debugging
  // source: https://stackoverflow.com/questions/2987207/why-do-i-get-a-c-malloc-assertion-failure
  // malloc assertion failure
  // source: https://stackoverflow.com/questions/2612447/pinpointing-conditional-jump-or-move-depends-on-uninitialized-values-valgrin
  // conditional jump or move depends on unitialized values
  // source: https://sourceforge.net/p/valgrind4win/wiki/DevelopmentEnvironment/
  // using valgrind4 
  // source: https://stackoverflow.com/questions/14493707/display-value-found-at-given-address-gdb 
  // https://stackoverflow.com/questions/14493707/display-value-found-at-given-address-gdb
  // using gdb
  
  return(EXIT_SUCCESS);
}





