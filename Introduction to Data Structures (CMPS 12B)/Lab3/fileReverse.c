/*
*  Name: Leonid Shuster, ID #1616143
*  Role: Takes in input of lines and reverses each word on a separate line
*  File name: fileReverse.c
*  Instructions: Compile with command gcc –std=c99 –Wall –o fileReverse fileReverse.c
*   or make and then command ./fileReverse "some input" "some output"
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void stringReverse(char* s); // Function prototype for reversing string

int main(int argc, char* argv[]){
   FILE* in;  /* file handle for input */  
   FILE* out; /* file handle for output */
   char word[256]; /* char array to store words from input file */

   /* check command line for correct number of arguments */
   if(argc != 3) {
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   /* open input file for reading */
   in = fopen(argv[1], "r");
   if(in == NULL){
      printf("Unable to read from file %s\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   /* open output file for writing */
   out = fopen(argv[2], "w");
   if(out == NULL){
      printf("Unable to write to file %s\n", argv[2]);
      exit(EXIT_FAILURE);
   }

   /* read words from input file, print on separate lines to output file*/
   while(fscanf(in, " %s", word) != EOF ){
      stringReverse(word);
      fprintf(out, "%s\n", word);
   }

   /* close input and output files */
   fclose(in);
   fclose(out);

   return(EXIT_SUCCESS);
}
void stringReverse (char* s) {
   char* beginning = s; // Initializes pointer to beginning
   char* end = s; // Initializes pointer to beginning 
   char temp; // Temporary character

   while (*end != 0) { // Iterates through string until it reaches "/0"
      end++;
   }

   end = end - 1; // Subtract 1 to get last character of string

   int i;
   for (i = 0; i < (strlen(s) / 2); i++) { // Goes through string switching beginning and end until reaches halfway
      temp = *end; // Saves last charaacter
      *end = *beginning; // Assigns last character to first character
      *beginning = temp; // Assigns first character to saved last character

      beginning++; // Advance through string
      end--; // Goes back through string
   }
}

