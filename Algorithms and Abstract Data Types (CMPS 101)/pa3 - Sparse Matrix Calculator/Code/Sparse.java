// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa3
// Role: Creates two matrices with given criteria from text file and performs operations on them

import java.io.*;
import java.util.Scanner;

public class Sparse {
	public static void main(String[] args) throws IOException {

		// Check that number of command line arguments is at least 2
		if (args.length < 2 || args.length > 2) {
			System.err.println("Usage: java –jar Lex <input file> <output file>");
			System.exit(1);
		}

		// Open files
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		// Get first line of text file
		String line = in.nextLine().trim() + " "; // Trims leading and trailing spaces
		String[] token = line.split(" "); // Splits line into tokens at every space

		int n = Integer.parseInt(token[0]); // Size of matrices
		int a = Integer.parseInt(token[1]); // Number of non-zero entries of A
		int b = Integer.parseInt(token[2]); // Number of non-zero entries of B

		// Create two new matrices
		Matrix A = new Matrix(n);
		Matrix B = new Matrix(n);

		in.nextLine(); // Advance one line

		for (int i = 0; i < a; i++) { // Loops through a lines of text file to get a non-zero entries of A and add it to A
			line = in.nextLine().trim() + " "; // Trims leading and trailing spaces
			token = line.split(" "); // Splits line into tokens at every space

			int row = Integer.parseInt(token[0]);
			int col = Integer.parseInt(token[1]);
			double val = Double.parseDouble(token[2]);

			A.changeEntry(row, col, val);
		}

		in.nextLine();

		for (int i = 0; i < b; i++) { // Loops through a lines of text file to get b non-zero entries of b and add it to b
			line = in.nextLine().trim() + " "; // Trims leading and trailing spaces
			token = line.split(" "); // Splits line into tokens at every space

			int row = Integer.parseInt(token[0]);
			int col = Integer.parseInt(token[1]);
			double val = Double.parseDouble(token[2]);

			B.changeEntry(row, col, val);
		}

		// Print out all matrices/matrix manipulations
		out.println("A has " + A.getNNZ() + " non-zero entries:");
		out.println(A);

		out.println("B has " + B.getNNZ() + " non-zero entries:");
		out.println(B);

		out.println("(1.5)*A =");
		out.println(A.scalarMult(1.5));

		out.println("A+B =");
		out.println(A.add(B));

		out.println("A+A =");
		out.println(A.add(A));

		out.println("B-A =");
		out.println(B.sub(A));

		out.println("A-A =");
		out.println(A.sub(A));

		out.println("Transpose(A) =");
		out.println(A.transpose());

		out.println("A*B =");
		out.println(A.mult(B));

		out.println("B*B =");
		out.println(B.mult(B));

		// Close files
		in.close();
		out.close();
	}
}