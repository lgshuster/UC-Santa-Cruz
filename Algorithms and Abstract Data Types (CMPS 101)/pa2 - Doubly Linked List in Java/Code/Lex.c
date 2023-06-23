// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa2
// Role: Sorts words by lexicographic order by taking indices of words, placing accordingly into list, and then reverting back to words in sorted order

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define MAX 257

int main(int argc, char* argv[]){
	
	FILE* in;  // file handle for input
	FILE* out; // file handle for output

	char input[MAX];

	// check command line for correct number of arguments
	if(argc != 3) {
	  printf("Usage: %s <input file> <output file>\n", argv[0]);
	  exit(EXIT_FAILURE);
	}

	// open input file for reading
	in = fopen(argv[1], "r");
	if(in == NULL) {
	  printf("Unable to read from file %s\n", argv[1]);
	  exit(EXIT_FAILURE);
	}

	// open output file for writing
	out = fopen(argv[2], "w");
	if(out == NULL) {
	  printf("Unable to write to file %s\n", argv[2]);
	  exit(EXIT_FAILURE);
	}

	int lineCount = 0; // Counter to find out how big to make array for words

	// Loops to find out how many lines/words
	while (fgets(input, MAX, in) != NULL) {
	  lineCount++;
	}
	fclose(in);

	char words[lineCount][MAX]; // Array for strings
	int wordsIndex = 0; // Index for array that holds words
	in = fopen(argv[1], "r");

	// Loops to put all words into array for words
	while (fgets(input, MAX, in) != NULL) {
		strcpy(words[wordsIndex], input);
		wordsIndex++;
	}

	List list = newList(); // Create empty list
	append(list, 0); // Add first element

	// Sorts words array into words list in the form of their respective indices. A new word is
	//   compared to the word found at the cursor (through their indices), and then moved accordingly
	for (int i = 1; i < lineCount; i++) {
		while (index(list) != -1) { // Keep looping until cursor falls off the back of the list
			if (strcmp(words[i], words[get(list)]) <= 0) { // If current word comes before word found at cursor, insert before
				insertBefore(list, i);
				break;
			}
			else { // If current word does not come before word found at cursor, keep moving cursor forward until correct position is found
				moveNext(list);
			}
		}
		if (index(list) == -1) { // If cursor does fall off, just add to the back
			append(list, i);
		}
		moveFront(list); // Bring cursor to the front of the list after every insertion
	}

	// After list is sorted, now need to change indices back to their respective words found in words array
	for (int i = 0; i < lineCount; i++) { // Loops through list by moving cursor forward starting from front printing node's data along the way
		fprintf(out, "%s", words[get(list)]);
		moveNext(list);
	}

	freeList(&list); // Free list and nodes


	// close input and output files
	fclose(in);
	fclose(out);

	return(EXIT_SUCCESS);


}