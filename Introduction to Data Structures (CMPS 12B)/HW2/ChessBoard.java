// Name: Leonid Shuster, ID #1616143
// Role: Creates linked list of chess pieces, checks if board is valid, checks if piece is at a specific spot, and if that piece attacks any other pieces
// File name: ChessBoard.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

import java.io.*;
import java.util.Scanner;
import java.lang.*;

public class ChessBoard {
	public static void main(String[] args) throws IOException {
		// Check that number of command line arguments is at least 2
		if (args.length < 2) {
			System.out.println("Usage: java –jar ChessBoard.jar <input file> <output file>");
			System.exit(1);
		}
		// Open files
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		// Loops until no more lines
		while (in.hasNextLine()) {
			// Trims leading and trailing spaces
			String line = in.nextLine().trim();

			// Separates query from piece coordinates
			String[] separation = line.split(":");

			// Separates query coordinates into two elements
			String[] query = separation[0].split(" ");

			// Separates pieces and position coordinates
			String[] pieces = separation[1].split(" ");

			LinkedList chessPiecesList = new LinkedList();

			for (int i = 1; i < pieces.length - 1; i = i+3) {
				char newPiece;
				String newColor;
				int newCol;
				int newRow;

				newPiece = pieces[i].charAt(0); // Set type of piece

				if (Character.isUpperCase(newPiece)) { // Check for color: uppercase is black, lowercase is white
					newColor = "black";
				}
				else {
					newColor = "white";
				}

				newCol = Integer.parseInt(pieces[i+1]); // Set col
				newRow = Integer.parseInt(pieces[i+2]); // Set row

				ChessPiece newChessPiece;

				if (newPiece == 'k' || newPiece == 'K') {
					newChessPiece = new King(newPiece, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (newPiece == 'q' || newPiece == 'Q') {
					newChessPiece = new Queen(newPiece, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (newPiece == 'r' || newPiece == 'R') {
					newChessPiece = new Rook(newPiece, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (newPiece == 'b' || newPiece == 'B') {
					newChessPiece = new Bishop(newPiece, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (newPiece == 'n' || newPiece == 'N') {
					newChessPiece = new Knight(newPiece, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (newPiece == 'p' || newPiece == 'P') {
					newChessPiece = new Pawn(newPiece, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
			}

			if (chessPiecesList.isValid()) { // If chessboard is valid (no pieces on the same spot)
				int queryCol = Integer.parseInt(query[0]); 
				int queryRow = Integer.parseInt(query[1]);

				ChessPiece pieceAtQuery = chessPiecesList.find(queryCol, queryRow); // Returns chesspiece if found and null if not

				if (pieceAtQuery == null) { // No pieces on query
					out.print("-");
				}
				else {
					out.print(pieceAtQuery.type);
					out.print(" ");
					if (chessPiecesList.checkAttacking(pieceAtQuery)) { // Check if piece at query is attacking other pieces
						out.print("y");
					}
					else {
						out.print("n");
					}
				}
			}
			else {
				out.print("Invalid");
			}
			out.println();
		}
		// Close files
		in.close();
		out.close();
	}
}