# Cindy Li
# CSE 374
# HW5
# 5/12/2023
# citations: https://www.gnu.org/software/make/manual/html_node/Makefile-Contents.html#:~:text='%20%23%20'%20in%20a%20line%20of,effectively%20blank%2C%20and%20is%20ignored.
# 

all: t9

t9: tnine.o trie.o
	gcc tnine.o trie.o -o t9

tnine.o: tnine.c trie.c
	gcc -Wall -g -std=c11 -c tnine.c 

trie.o: trie.c trienode.h
	gcc -Wall -g -std=c11 -c trie.c 

clean:
	rm -f *.o trie

# citations: https://www.gnu.org/software/make/manual/html_node/Makefile-Contents.html#:~:text='%20%23%20'%20in%20a%20line%20of,effectively%20blank%2C%20and%20is%20ignored.
# https://stackoverflow.com/questions/16931770/makefile4-missing-separator-stop
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
