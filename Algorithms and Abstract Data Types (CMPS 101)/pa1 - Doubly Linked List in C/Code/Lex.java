// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa1
// Role: Sorts words by lexicographic order by taking indices of words, placing accordingly into list, and then reverting back to words in sorted order

import java.io.*;
import java.util.Scanner;

public class Lex {
	public static void main(String[] args) throws IOException {

		// Check that number of command line arguments is at least 2
		if (args.length < 2 || args.length > 2) {
			System.err.println("Usage: java –jar Lex <input file> <output file>");
			System.exit(1);
		}

		// Open files
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		int wordsCount = 0; // Counter to find out how big to make array for words

		// Loops to find out how many lines/words
		while (in.hasNextLine()) {
			String line = in.nextLine().trim() + " ";
			wordsCount++;
		}

		in = new Scanner(new File(args[0])); // Get input file again
		String words[] = new String[wordsCount];
		int wordsIndex = 0; // Index for array that holds words

		// Loops to put all words into array for words
		while (in.hasNextLine()) {
			String line = in.nextLine().trim() + " "; // Trims leading and trailing spaces
			words[wordsIndex] = line; // Puts word into array for words
			wordsIndex++;
		}

		List wordsList = new List(); // Create empty list

		wordsList.append(0); // Add first element

		// Sorts words array into words list in the form of their respective indices. A new word is 
		//   compared to the word found at the cursor (through their indices), and then moved accordingly
		for (int i = 1; i < words.length; i++) {
			String currentWord = words[i];

			if (currentWord.compareTo(words[wordsList.get()]) <= 0) { // If current word comes before word found at cursor, insert before
				wordsList.insertBefore(i);
			}
			else { // If current word does not come before word found at cursor, keep moving cursor forward until correct position is found
				boolean isWordSorted = false; // Becomes true when word is in the right position

				// Keeps moving cursor forward until current word is in correct position
				while (isWordSorted == false) {
					if (wordsList.index() == -1) { // If cursor falls of the back of the list, just add to the back
						wordsList.append(i);
						isWordSorted = true;
					}
					else if (currentWord.compareTo(words[wordsList.get()]) <= 0) { // If current word comes before word found at cursor, insert before
						wordsList.insertBefore(i);
						isWordSorted = true;
					}
					else { // If not in correct position, move cursor forward
						wordsList.moveNext();
					}
				}
			}
			wordsList.moveFront(); // Bring cursor to the front of the list after every insertion
		}

		// After list is sorted, now need to change indices back to their respective words found in words array 
		for (int i = 0; i < words.length; i++) { // Loops through list by moving cursor forward starting from front printing node's data along the way
			out.println(words[wordsList.get()]);
			wordsList.moveNext();
		}

		// Close files
		in.close();
		out.close();
	}
}