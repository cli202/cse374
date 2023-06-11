// Cindy Li
// CSE 374
// HW5
// 5/10/2023

/* trie implements a trie, made of trieNodes. This includes
   code to build, search, and delete a trie
   CSE374, HW5, 22wi 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "trienode.h"

/* You'll want to prototype any helper functions you need here */

/* You'll need to implement the functions declare in trienode.h */
/* As well as the helpfer functions you prototype above. */

// Prototype functions
// this function return a string of numbers corresponding to the 
// word given according to the t9 predictive text
char* getKey(char* word);

// this function inserts a word into the trie given the root node,
// the corresponding key sequence of the word, and the word
void insert(trieNode* currRoot, char* key, char* word);


// this function creates new node by dynamically assigning memory 
trieNode* new_node() {
  trieNode* node = malloc(sizeof(trieNode));
  assert(node != NULL);
  // source : https://stackoverflow.com/questions/17522327/is-null-character-included-while-allocating-using-malloc
  // does null terminator take up space needed in malloc 
  node->word = (char*) malloc(MAXLEN * sizeof(char));
  assert(node->word != NULL);
  node->word[0] = '\0';
  for (int i = 0; i < BRANCHES; i++) {
    node->branches[i] = NULL;
  }
  return node;
}


// this function build sthe trie given a file 
trieNode* build_tree(FILE* dict) {
  char* word = (char*) malloc(MAXLEN);
  assert(word != NULL);
  trieNode* rootNode = new_node();
  char* key = (char*) malloc(MAXLEN);
  assert(key != NULL);

  // go through the file for each line
  while (fgets(word, MAXLEN, dict)) {
    // source https://stackoverflow.com/questions/14905937/is-that-char-null-terminator-is-including-in-the-length-count
    // checks whether strlen includes null terminator as a character 
    int len = strlen(word);

    // change all new line characters to null termiantor 
    if (word[len - 1] == '\n') {
      word[len - 1] = '\0';
    }
    char* new_key = getKey(word);
    // copy the new key into key and insert the word into the trie
    strncpy(key, new_key, MAXLEN);
    insert(rootNode, key, word);
    free(new_key);
  }
  // source: https://www.prepbytes.com/blog/c-programming/dangling-pointer-in-c/#:~:text=A%20dangling%20pointer%20in%20C%20is%20a%20pointer%20that%20points,before%20the%20memory%20is%20freed.
  // don't remeber where but I had a problem somewhere with free and dangling pointers
  free(key);
  free(word);
  return rootNode;
}

// this function checks if the word corresponding to the pattern
// exists in the trie or not. If it exists, return the word,
// else, return null
char* get_word(trieNode* currRoot, char* pattern) {
  trieNode* curr = currRoot;

  // go through trie. If branch exists, move to branch
  // else return null
  for (int i = 0; i < strlen(pattern); i++) {
    // source: https://www.geeksforgeeks.org/c-program-for-char-to-int-conversion/#
    // how to convert a char to an int (ASCII subtraction)
    int branch_ind = pattern[i] - '0';

    if (curr->branches[branch_ind] == NULL) {
      return NULL;
    }
    curr = curr->branches[branch_ind];
  }

  // if node exists but doens't have a word associated with it 
  // (meaning its a prefix only), return null
  // else return the word
  if (curr->word != NULL) {
    return curr->word;
  } else {
    return NULL;
  }
}

// this function recursively frees all nodes in the trie
void free_tree(trieNode* curr) {
   if (curr) {
    for (int i = 0; i < BRANCHES; i++) {
      if (curr->branches[i]) {
      	free_tree(curr->branches[i]);
      }
    }
    free(curr->word);
    free(curr);
  }
}

// this function return a string of numbers corresponding to the 
// word given according to the t9 predictive text
char* getKey(char* word) {
  char alphabet[26] = {2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6,
		       7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9};
  char* key = (char*) malloc(strlen(word) + 1);
  assert(key != NULL);
  
  // for each character in word, find the corresponding 
  // number in t9 text program and add it to key
  for (int i = 0; i < strlen(word); i++) {
    int index = word[i] - 'a';
    key[i] = alphabet[index] + '0';
  }

  // set last character in key to be null terminator 
  // and return key
  key[strlen(word)] = '\0';
  return key;
}

// this function inserts a word into the trie given the root node,
// the corresponding key sequence of the word, and the word
void insert(trieNode* currRoot, char* key, char* word) {
  trieNode* curr = currRoot;
  
  // for each character in the key, move through the trie accordingly
  for (int i = 0; i < strlen(key); i++) {
    int index = key[i] - '0' - 2;
    
    // if branch doesn't exist, create branch
    if (curr->branches[index] == NULL) {
      curr->branches[index] = new_node(); 
    }
    curr = curr->branches[index];
  }

  // if, for some reason, current node is null 
  // (perhaps new_node failed), return 
  if (curr == NULL) {
    return;
  }
  
  // if current node word is empty, copy word into curr->word
  // else, move down the # or 9th branch of the node until an empty
  // curr->word is found 
  if (curr->word[0] == '\0') {
    //source: https://www.tutorialspoint.com/c_standard_library/c_function_strncpy.htm
    // I forgot strncpy had to have 3 parameters
    strncpy(curr->word, word, MAXLEN);
  } else {
    trieNode* temp = curr;

    // move down the series of # branches
    while(temp && temp->branches[8]) {
      temp = temp->branches[8];
    }
    trieNode* newNode = new_node();
    temp->branches[8] = newNode;
    curr = newNode;
    strncpy(curr->word, word, MAXLEN);
  }
}
