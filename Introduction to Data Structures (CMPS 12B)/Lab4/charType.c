/*
*  Name: Leonid Shuster, ID #1616143
*  Role: Takes in line of characters and parses based on type of character, then prints out how many of each type of character
*  File name: charType.c
*  Instructions: Compile with command gcc –std=c99 –Wall –o charType charType.c
*   or make and then command ./charType "some input" "some output"
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX 257 // Max length of line (with null terminator)

void extract_chars(char* s, char* a, char* d, char* p, char* w); // Prototype for extracting characters
void print_chars(char* s, char* a, char* d, char* p, char* w, FILE* output, int line); // Prototype for printing characters

int main(int argc, char* argv[]){
   FILE* in;  /* file handle for input */  
   FILE* out; /* file handle for output */
   char* string_ptr = NULL; // Pointer for allocated memory of string
   char* alpha_ptr = NULL; // Pointer for allocated memory of alphabetic characters
   char* digit_ptr = NULL; // Pointer for allocated memory of digit characters
   char* punct_ptr = NULL; // Pointer for allocated memory of punctuation characters
   char* space_ptr = NULL; // Pointer for allocated memory of whitespace characters
   int currentLine = 1; // Counter for current line

   /* check command line for correct number of arguments */
   if(argc != 3) {
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   /* open input file for reading */
   in = fopen(argv[1], "r");
   if(in == NULL) {
      printf("Unable to read from file %s\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   /* open output file for writing */
   out = fopen(argv[2], "w");
   if(out == NULL) {
      printf("Unable to write to file %s\n", argv[2]);
      exit(EXIT_FAILURE);
   }

   // NOTE: calloc initializes an array with all elements to 0, while malloc does not
   string_ptr = (char*) calloc(MAX, sizeof(char)); // Allocated memory for string
   alpha_ptr = (char*) calloc(MAX, sizeof(char)); // Allocated memory for alphabetic characters
   digit_ptr = (char*) calloc(MAX, sizeof(char)); // Allocated memory for digit characters
   punct_ptr = (char*) calloc(MAX, sizeof(char)); // Allocated memory for punctuation characters
   space_ptr = (char*) calloc(MAX, sizeof(char)); // Allocated memory for whitespace characters
   
   while (fgets(string_ptr, MAX, in) != NULL) { // Keeps reading file until fails to read
      extract_chars(string_ptr, alpha_ptr, digit_ptr, punct_ptr, space_ptr); // Call function to separate characters into different arrays

      print_chars(string_ptr, alpha_ptr, digit_ptr, punct_ptr, space_ptr, out, currentLine); // Call function to print arrays
      currentLine++;

      // Clears previous string, initializes everything to 0's
      memset(string_ptr, '\0', MAX);
      memset(alpha_ptr, '\0', MAX);
      memset(digit_ptr, '\0', MAX);
      memset(punct_ptr, '\0', MAX);
      memset(space_ptr, '\0', MAX);
   }
   // Free allocated memory
   free(string_ptr);
   string_ptr = NULL;

   free(alpha_ptr);
   alpha_ptr = NULL;

   free(digit_ptr);
   digit_ptr = NULL;

   free(punct_ptr);
   punct_ptr = NULL;

   free(space_ptr);
   space_ptr = NULL;

   /* close input and output files */
   fclose(in);
   fclose(out);

   return(EXIT_SUCCESS);
}

void extract_chars(char* s, char* a, char* d, char* p, char* w) {
   char currentChar;

   while(*s != '\0') { // Dereference pointer and acquire character at that point, loop until no more letters
      currentChar = *s;

      // Find what kind of character, then add to its array and move pointer over
      if (isalpha((int)currentChar)) { // Check for alphabetic
         *a = currentChar;
         a++;
      }
      if (isdigit((int)currentChar)) { // Check for digit
         *d = currentChar;
         d++;
      }
      if (ispunct((int)currentChar)) { // Check for punctuation
         *p = currentChar;
         p++;
      }
      if (isspace((int)currentChar)) { // Check for whitespace
         *w = currentChar;
         w++;
      }
      s++;
   }
   return;
}

void print_chars(char* s, char* a, char* d, char* p, char* w, FILE* output, int line) {
   fprintf(output, "line %d contains:\n", line);

   // Checkers for singular and plural words
   if (strlen(a) > 1) {
      fprintf(output, "%d alphabetic characters: %s\n", (int)strlen(a), a);
   }
   else {
      fprintf(output, "%d alphabetic character: %s\n", (int)strlen(a), a);
   }

   if (strlen(d) > 1) {
      fprintf(output, "%d numeric characters: %s\n", (int)strlen(d), d);
   }
   else {
      fprintf(output, "%d numeric character: %s\n", (int)strlen(d), d);
   }

   if (strlen(p) > 1) {
      fprintf(output, "%d punctuation characters: %s\n", (int)strlen(p), p);
   }
   else {
      fprintf(output, "%d punctuation character: %s\n", (int)strlen(p), p);
   }

   if (strlen(w) > 1) {
      fprintf(output, "%d whitespace characters: %s\n", (int)strlen(w), w);
   }
   else {
      fprintf(output, "%d whitespace character: %s\n", (int)strlen(w), w);
   }
   return;
}