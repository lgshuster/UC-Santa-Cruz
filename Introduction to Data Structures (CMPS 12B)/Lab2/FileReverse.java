// Name: Leonid Shuster, ID #1616143
// Role: Takes in input of lines and returns each token reversed on a separate line
// File name: FileReverse.java
// Instructions: Compile with command javac FileReverse.java or make and then command java FileReverse "some input" "some output"

import java.io.*;
import java.util.Scanner;

class FileReverse {
	public static void main(String[] args) throws IOException{

		// Check that number of command line arguments is at least 2
		if (args.length < 2) {
			System.out.println("Usage: java –jar FileTokens.jar <input file> <output file>");
			System.exit(1);
		}

		// Open files
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		// Loops until no more lines
		while (in.hasNextLine()) {
			// Trims leading and trailing spaces
			String line = in.nextLine().trim() + " ";

			// Splits line into tokens at every space
			String[] token = line.split("\\s+");

			// Reverses every token
			for (int i = 0; i < token.length; i++) {
				out.println(stringReverse(token[i])); // Calls function that reverses token
			}
		}
		// close files
		in.close();
		out.close();
	}

	// Function that reverses tokens
	public static String stringReverse(String s) {
		String reversedToken = ""; // Create empty string
		
		// Loop from the last letter
		for (int i = s.length() - 1; i > -1; i--) {
			reversedToken += s.charAt(i);
		}
		return reversedToken;
	}
}