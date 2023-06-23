// Name: Leonid Shuster, ID #1616143
// Role: Places n Queens on an n x n board such that no two queens attack each other, uses recursion
// File name: NQueens.java
// Instructions: Compile with command javac NQueens.java or make and then command java NQueens "some input" "some output"
// HAD HELP FROM SESH'S SOLUTION, ESPECIALLY THE RECURSIVE PART

import java.io.*;
import java.util.Scanner;

public class NQueens {
	private static Queen[] solution;
	public static void main(String[] args) throws IOException {

		// Check that number of command line arguments is at least 2
		if (args.length < 2) {
			System.out.println("Usage: java –jar NQueens.jar <input file> <output file>");
			System.exit(1);
		}

		// Open files
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		// Loops until no more lines
		while (in.hasNextLine()) {
			String line = in.nextLine().trim() + " "; // Trims leading and trailing spaces

			String[] token = line.split("\\s+"); // Splits line into tokens at every space

			// Separate tokens
			int boardSize = Integer.parseInt(token[0]);
			int firstCol = Integer.parseInt(token[1]);
			int firstRow = Integer.parseInt(token[2]);

			solution = new Queen[boardSize]; // Array of Queens of board size, each with column and row

			solution[0] = new Queen(firstCol, firstRow); // First queen is given

			if (PlaceQueens(boardSize, boardSize - 1)) { // If placing all queens comes back successfully
				for (int i = 1; i <= solution.length; i++) { // Loops over columns
					for (int j = 0; j < solution.length; j++) { // Loops over queens
						if (solution[j].col == i) { // If queen column equals current column
							out.print(solution[j].col);
							out.print(" ");
							out.print(solution[j].row);
							out.print(" ");
						}
					}
				}
				out.println();
			}
			else { // If placing queens was not successful
				out.println("No solution");
			}
		}
		// Close files
		in.close();
		out.close();
	}
	static boolean PlaceQueens(int boardSize, int queensLeft) { // Takes in how many queens left and array of queens
		int freeColumn = 0; // Initialize free column to 0
		int queensPlaced = boardSize - queensLeft; // Number of queens already placed

		if (queensLeft == 0) { // Base case
			return true;
		}

		// Tries to find valid position
		for (int col = 1; col < boardSize + 1; col++) { 
			if (IsSafeColumn(col, queensPlaced)) {
				freeColumn = col;
				break;
			}
		}

		Queen nextQueen = null;
		for (int row = 1; row < boardSize + 1; row++) { // Loops through rows
			boolean canPlace = true; // Checker for if queen can be placed
			nextQueen = new Queen(freeColumn, row); // Try to place queen
			if (IsBeingAttacked(nextQueen, queensPlaced)) { // If queen is being attacked in that spot, can't place there
				canPlace = false;
			}
			// Checks if piece can be placed
			if (canPlace) { // If piece can be placed, add piece to solutions and recurse with one less queen left
				solution[queensPlaced] = nextQueen;
				if (PlaceQueens(boardSize, queensLeft - 1)) { // If recursive calls comes back true
					return true;
				}
			}
		}
		return false;
	}

	static boolean IsSafeColumn(int c, int queensPlaced) { // Checks if column is safe (there are no queens that have the same column)
		for (int i = 0; i < queensPlaced; i++) { // Iterate through the solution array
			if (solution[i].col == c) { // If a queen from the solution array has the same column
				return false;
			}
		}
		return true;
	}

	static boolean IsBeingAttacked(Queen currentQueen, int queensPlaced) { // Checks if currentQueen is being attacked by any other queens on the stack
		for (int i = 0; i < queensPlaced; i++) { // Iterate through the solution array
			if (solution[i].isAttacking(currentQueen)) { // If queen from the solution array is attacking currentQueen
				return true;
			}
		}
		return false;
	}
}