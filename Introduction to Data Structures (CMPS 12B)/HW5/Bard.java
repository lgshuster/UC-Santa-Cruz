// Name: Leonid Shuster, ID #1616143
// Role: Parse shakespeare.txt and depending on the query l, k, return the k most frequent word of length l
// File name: Bard.java
// Instructions: Compile with command javac Bard.java or make and then command java Bard "some input" "some output"

import java.io.*;
import java.util.*;

public class Bard {
	private static Hashtable<String, Integer> wordHash = new Hashtable<String, Integer>(); // Hash table with (key, value) as (String, frequency)

	public static void main(String[] args) throws IOException {

		// Check that number of command line arguments is at least 2
		if (args.length < 2) {
			System.out.println("Usage: java –jar NQueens.jar <input file> <output file>");
			System.exit(1);
		}

		// Open files
		Scanner in = new Scanner(new File(args[0]));
		Scanner shakespeare = new Scanner(new File("shakespeare.txt"));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		ParseText(shakespeare); // Calls function that breaks up shakespeare text and adds words and their frequencies to a hash table

		while (in.hasNextLine()) { // Loops until no more lines
			ArrayList<String> wordList = new ArrayList<String>(); // Create array list of strings, each loop has a new array list of words of a certain length
			Enumeration<String> keysEnum = wordHash.keys(); // Enumeration to iterate over hash table

			String line = in.nextLine();
			String query[] = line.trim().split("\\s+");

			int length = Integer.parseInt(query[0]);
			int rank = Integer.parseInt(query[1]);

			while (keysEnum.hasMoreElements()) { // Lopps until no more keys in hash table
				String key = keysEnum.nextElement(); 
				if (key.length() == length) { // If string length matches query length, add to array list
					wordList.add(key);
				}
			}

			if (wordList.size() == 0 || rank > wordList.size() - 1) { // If array list is empty, print - and continue to next query
				out.println("-");
				continue;
			}

			wordList = SortList(wordList); // Calls function that sorts list first by frequency and then lexiographically is frequencies match

			out.println(wordList.get(rank)); // Print out x most frequent word
		}		

		// Close files
		in.close();
		out.close();
	}

	static void ParseText(Scanner input) {
		while (input.hasNextLine()) { // Loops until no more lines
			String line = input.nextLine(); // Gets line
			line = TokenizeString(line); // Calls function that gets rid of most punctuation

			String tokens[] = line.trim().split("\\s+"); // Trims beginning and ending whitespace and split string into array of words

			for (int i = 0; i < tokens.length; i++) { // Iterate over array and change each word to lowercase, and then add to hash table
				tokens[i] = tokens[i].toLowerCase();
				AddToHash(tokens[i]); // Calls function that adds string to hash table
			}
		}
	}

	static String TokenizeString(String str) {
		// Replace most punctuation with whitespace
		str = str.replace("?", " ");
		str = str.replace(",", " ");
		str = str.replace(".", " ");
		str = str.replace("!", " ");
		str = str.replace(":", " ");
		str = str.replace(";", " ");
		str = str.replace("[", " ");
		str = str.replace("]", " ");

		return str;
	}

	static void AddToHash(String str) {
		if (wordHash.containsKey(str)) { // If hash table already has the string
			wordHash.put(str, wordHash.get(str) + 1); // Get key and value and reinsert key with incremented frequency
		}
		else { // If hash table doesn't have the string
			wordHash.put(str, 1); // Add key with frequency 1
		}
	}

	static ArrayList<String> SortList(ArrayList<String> list) {
		for (int i = 0; i < list.size() - 1; i++) { // Goes through every element one by one
			for (int j = i + 1; j < list.size(); j++) { // Checks against every other element in the list
				String currentKey = list.get(i); // Current word
				String nextKey = list.get(j); // Next word
				int currentNumber = wordHash.get(currentKey); // Current word's frequency
				int nextNumber = wordHash.get(nextKey); // Next word's frequency

				if (wordHash.get(currentKey) < wordHash.get(nextKey))  { // If current word's frequency is less than next word's frequency, swap
					Collections.swap(list, i, j);
				}

				if (currentNumber == nextNumber) { // If current word's frequency is equal to next word's frequency, swap if they are not lexiographically ordered
					if (currentKey.compareTo(nextKey) > 0) { // If s1 > s2, returns positive number, means not lexiographically ordered
						Collections.swap(list, i, j);
					}
				}
			}
		}	
		return list;		
	}
}