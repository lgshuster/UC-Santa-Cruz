// Name: Leonid Shuster, ID #1616143
// Role: Places n Queens on an n x n board such that no two queens attack each other, uses stack instead of recursion
// File name: NQueens.java
// Instructions: Compile with command javac NQueens.java or make and then command java NQueens "some input" "some output"

import java.io.*;
import java.util.Scanner;
import java.util.Stack;

public class NQueens {
	private static Stack<Queen> queenStack = new Stack<Queen>(); // Stack for queens
	private static Queen[] solution; // Array of queens
	private static Queen[] input; // Array of input queens

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

			String[] token = line.split(" "); // Splits line into tokens at every space

			int boardSize = Integer.parseInt(token[0]); // Board size is first element of the array

			int queenCol = 0;
			int queenRow = 0;
			boolean isValid = true; // Truth value for if input queens attack each other
			input = new Queen[token.length/2]; // Array for input queens, all cols + all rows / 2
			int inputIndex = 0; // Index for input queen array

			for (int i = 1; i < token.length - 1; i = i + 2) { // Iterate every two for column and row
				queenCol = Integer.parseInt(token[i]);
				queenRow = Integer.parseInt(token[i+1]);
				Queen newPiece = new Queen(queenCol, queenRow);
				queenStack.push(newPiece);
				if (IsBeingAttacked(newPiece)) { // Check if new piece is being attacked by others
					isValid = false;
					break;
				}
				input[inputIndex] = newPiece; // Insert queen into input queen array
				inputIndex++;
			}

			if (isValid && PlaceQueens(boardSize)) { // If no input queens attack each other and placing queens was successful
				solution = new Queen[boardSize]; //Array for solution, should be size of board
				int stackSize = queenStack.size();

				for (int i = 0; i < stackSize; i++) { // Iterate over stack and pop every element into array solution
					solution[i] = queenStack.pop();
				}

				for (int c = 1; c <= solution.length; c++) { // Loops over columns
					for (int j = 0; j < solution.length; j++) { // Loops over queens
						if (solution[j].col == c) { // If queen column equals current column
							out.print(solution[j].col);
							out.print(" ");
							out.print(solution[j].row);
							out.print(" ");
						}
					}
				}
				out.println();
			}
			else {
				out.println("No solution");
			}

			while (!queenStack.empty()) { // Clear stack if placing queens was not successful
				queenStack.pop();
			}
		}
		// Close files
		in.close();
		out.close();
	}

	static boolean PlaceQueens(int boardSize) {
		int currentColumn = 0;
		int currentRow = 1;
		while (queenStack.size() != boardSize) {
			// Tries to find valid position
			for (int col = 1; col <= boardSize; col++) { // Loops through columns
				if (IsSafeColumn(col)) { // If column is safe, save free column and break out of loop
					currentColumn = col;
					break;
				}
			}

			Queen nextQueen = null;
			boolean canPlace = false;
			for (int row = currentRow; row <= boardSize; row++) { // Loops through rows
				nextQueen = new Queen(currentColumn, row); // Try to place queen
				if (!IsBeingAttacked(nextQueen)) { // If queen is not being attacked in that spot, break out of loop
					canPlace = true;
					break;
				}
			}

			// Checks if piece can be placed
			if (canPlace) { // If piece can be placed, push to stack and reset column
				queenStack.push(nextQueen);
				currentRow = 1;
			}
			else { // If piece cannot be placed, backtrack and set previous column
				if (queenStack.empty()) { // If stack is empty
					return false;
				}
				Queen currentQueen = queenStack.pop(); // Pop last queen
				currentRow = currentQueen.row + 1; // Increase its row by one

				int currentCol = currentQueen.col;
				if (currentCol == input[input.length - 1].col) { // If the queen popped off the stack was one of the inputs, return false (cannot move input queens)
					return false;
				}
			}
		}
		return true;
	}

	static boolean IsBeingAttacked(Queen currentQueen) { // Checks if currentQueen is being attacked by any other queens on the stack
		for (int i = 0; i < queenStack.size(); i++) {
			if (queenStack.get(i) == currentQueen) { // If it iterates over itself
				continue;
			}
			if (queenStack.get(i).isAttacking(currentQueen)) { // If queen from stack attacks currentQueen
				return true;
			}
		}
		return false;
	}

	static boolean IsSafeColumn(int c) { // Checks if column is safe (there are no queens that have the same column)
		for (int i = 0; i < queenStack.size(); i++) {
			if (queenStack.get(i).col == c) { // If a queen from the stack has the same column
				return false;
			}
		}
		return true;
	}

}