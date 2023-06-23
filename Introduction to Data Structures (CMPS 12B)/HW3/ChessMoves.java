// Name: Leonid Shuster, ID #1616143
// Role: Creates chessboard, moves and captures chessboard pieces, and checks for checks on king
// File name: ChessMoves.java
// Instructions: Compile with command javac ChessMoves.java or make and then command java -jar ChessMoves.jar "some input" "some output"

import java.io.*;
import java.util.Scanner;
import java.lang.*;

public class ChessMoves {
	public static void main(String[] args) throws IOException {
		// Check that number of command line arguments is at least 2
		if (args.length < 2) {
			System.out.println("Usage: java –jar ChessMoves.jar <input file> <output file>");
			System.exit(1);
		}
		// Open files
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));

		// Loops until no more lines
		while (in.hasNextLine()) {
			// Trims leading and trailing spaces
			String line = in.nextLine().trim();

			// Separates pieces and moves
			String[] separation = line.split(":");

			// Separates pieces and starting positions
			String[] pieces = separation[0].split(" ");

			// Separates positions and moves
			String[] moves = separation[1].split(" ");

			LinkedList chessPiecesList = new LinkedList();

			for (int i = 0; i < pieces.length - 1; i = i+3) {
				char pieceType;
				String newColor;
				int newCol;
				int newRow;

				pieceType = pieces[i].charAt(0); // Set type of piece

				if (Character.isUpperCase(pieceType)) { // Check for color: uppercase is black, lowercase is white
					newColor = "black";
				}
				else {
					newColor = "white";
				}

				newCol = Integer.parseInt(pieces[i+1]); // Set col
				newRow = Integer.parseInt(pieces[i+2]); // Set row

				ChessPiece newChessPiece;

				if (pieceType == 'k' || pieceType == 'K') {
					newChessPiece = new King(pieceType, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (pieceType == 'q' || pieceType == 'Q') {
					newChessPiece = new Queen(pieceType, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (pieceType == 'r' || pieceType == 'R') {
					newChessPiece = new Rook(pieceType, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (pieceType == 'b' || pieceType == 'B') {
					newChessPiece = new Bishop(pieceType, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (pieceType == 'n' || pieceType == 'N') {
					newChessPiece = new Knight(pieceType, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
				else if (pieceType == 'p' || pieceType == 'P') {
					newChessPiece = new Pawn(pieceType, newColor, newCol, newRow); // Create new piece
					chessPiecesList.insert(newChessPiece); // Insert chesspiece into linked list
				}
			}

			int numMoves = 1; // Odd number is white, even numbers are black (white moves first)
			boolean isLegalSoFar = true; // Truth value for legalality
			int startCol = 0;
			int startRow = 0;
			int moveCol = 0;
			int moveRow = 0;

			for (int i = 1; i < moves.length - 1; i = i+4) { // Loops every four for starting col and row and ending col and row
				if (isLegalSoFar) { // If moves are legal so far
					startCol = Integer.parseInt(moves[i]);
					startRow = Integer.parseInt(moves[i+1]);
					moveCol = Integer.parseInt(moves[i+2]);
					moveRow = Integer.parseInt(moves[i+3]);

					if (!chessPiecesList.isValid()) { // If there isn't more than one of a piece, or mone than one piece on a space
						isLegalSoFar = false;
						break;
					}

					ChessPiece pieceAtPos = chessPiecesList.findWithPos(startCol, startRow); // Piece to move
					ChessPiece dummyPiece; // Dummy piece to check if starting piece can move to spot

					if (pieceAtPos == null) { // If there is no piece at the starting space
						isLegalSoFar = false;
						break;
					}

					if (pieceAtPos.color == "black") {
						dummyPiece = new ChessPiece('q', "white", moveCol, moveRow); // Dummy piece just to check if piece can reach that spot, has to be opposite color
					}
					else {
						dummyPiece = new ChessPiece('Q', "black", moveCol, moveRow); // Dummy piece just to check if piece can reach that spot, has to be opposite color
					}

					if (numMoves == 1 && Character.isUpperCase(pieceAtPos.type)) { // First move is always white
						isLegalSoFar = false;
						break;
					}

					// Makes sure that white or black never move twice (white is always odd and should not be divisible by 2 and black is always even and should be divisible by 2)
					if ((numMoves % 2 == 0) && pieceAtPos.color == "white") { // If black should be moving but piece is white
						isLegalSoFar = false;
						break;
					}

					if ((numMoves % 2 != 0) && pieceAtPos.color == "black") { // If white should be moving but piece is black
						isLegalSoFar = false;
						break;
					}

					if (canMove(chessPiecesList, pieceAtPos, dummyPiece)) { // If piece can physically move to the spot and if any other pieces are not blocking its path
						if (chessPiecesList.findWithPos(moveCol, moveRow) != null) { // If there is a piece at the move spot that is a different color, it can be captured
							if (pieceAtPos.color != chessPiecesList.findWithPos(moveCol, moveRow).color && 
							chessPiecesList.findWithPos(moveCol, moveRow).type != 'k' && chessPiecesList.findWithPos(moveCol, moveRow).type != 'K') { // If the two pieces don't have the same color, and spot does not have a king (can't capture king)
								chessPiecesList.delete(moveCol, moveRow); // Deletes piece that was captured
							}
							else { // If pieces have same color or spot has king
								isLegalSoFar = false;
								break;
							}
						}
						pieceAtPos.col = moveCol; // Update col
						pieceAtPos.row = moveRow; // Update row
						numMoves++; // Update number of moves

						if (isKingInCheck(chessPiecesList, pieceAtPos)) { // Checks if king is in check
							isLegalSoFar = false;
						}
					}
					else { // If piece can't move to spot
						isLegalSoFar = false;
					}
				}
			}
			if (isLegalSoFar) { // If all moves are legal
				out.println("legal");
			}
			else { // If any moves are not legal
				out.print(startCol + " ");
				out.print(startRow + " ");
				out.print(moveCol + " ");
				out.print(moveRow + " ");
				out.println("illegal");
			}
		}
		// Close files
		in.close();
		out.close();
	}
	static boolean canMove(LinkedList list, ChessPiece startingPiece, ChessPiece dummyPieceAtSpot) { // Checks path of move
		int tempCol = startingPiece.col;
		int tempRow = startingPiece.row;
		int colToMove = dummyPieceAtSpot.col;
		int rowToMove = dummyPieceAtSpot.row;

		if (startingPiece.isAttacking(dummyPieceAtSpot)) { // If piece can physically move to the spot (based on piece rules)
			while (tempCol != colToMove || tempRow != rowToMove) { // Loops until move spot is reached; first finds what starting piece is, then checks what direction, and then keeps going in direction until move is reached
				if (startingPiece.type == 'k' || startingPiece.type == 'K') { // Kings
					if (startingPiece.col == colToMove && startingPiece.row + 1 == rowToMove) { // Above
						tempRow++;
					}
					else if (startingPiece.col == colToMove && startingPiece.row - 1 == rowToMove) { // Below
						tempRow--;
					}
					else if (startingPiece.col + 1 == colToMove && startingPiece.row == rowToMove) { // Right
						tempCol++;
					}
					else if (startingPiece.col - 1 == colToMove && startingPiece.row == rowToMove) { // Left
						tempCol--;
					}
					else if (startingPiece.col + 1 == colToMove && startingPiece.row + 1 == rowToMove) { // Top right
						tempCol++;
						tempRow++;
					}
					else if (startingPiece.col - 1 == colToMove && startingPiece.row + 1 == rowToMove) { // Top left
						tempCol--;
						tempRow++;
					}
					else if (startingPiece.col + 1 == colToMove && startingPiece.row - 1 == rowToMove) { // Bottom right
						tempCol++;
						tempRow--;
					}
					else if(startingPiece.col - 1 == colToMove && startingPiece.row - 1 == rowToMove) { // Bottom left
						tempCol--;
						tempRow--;
					}
				}
				else if (startingPiece.type == 'q' || startingPiece.type == 'Q') { // Queens
					if (startingPiece.col == colToMove && startingPiece.row < rowToMove) { // Above
						tempRow++;
					}
					else if (startingPiece.col == colToMove && startingPiece.row > rowToMove) { // Below
						tempRow--;
					}
					else if (startingPiece.col < colToMove && startingPiece.row == rowToMove) { // Right
						tempCol++;
					}
					else if (startingPiece.col > colToMove && startingPiece.row == rowToMove) { // Left
						tempCol--;
					}
					else if (startingPiece.col < colToMove && startingPiece.row < rowToMove) { // Top right
						tempCol++;
						tempRow++;
					}
					else if (startingPiece.col > colToMove && startingPiece.row < rowToMove) { // Top left
						tempCol--;
						tempRow++;
					}
					else if (startingPiece.col < colToMove && startingPiece.row > rowToMove) { // Bottom right
						tempCol++;
						tempRow--;
					}
					else if(startingPiece.col > colToMove && startingPiece.row > rowToMove) { // Bottom left
						tempCol--;
						tempRow--;
					}
				}
				else if (startingPiece.type == 'r' || startingPiece.type == 'R') { // Rooks
					if (startingPiece.col == colToMove && startingPiece.row < rowToMove) { // Above
						tempRow++;
					}
					else if (startingPiece.col == colToMove && startingPiece.row > rowToMove) { // Below
						tempRow--;
					}
					else if (startingPiece.col < colToMove && startingPiece.row == rowToMove) { // Right
						tempCol++;
					}
					else if (startingPiece.col > colToMove && startingPiece.row == rowToMove) { // Left
						tempCol--;
					}
				}
				else if (startingPiece.type == 'b' || startingPiece.type == 'B') { // Bishops
					if (startingPiece.col < colToMove && startingPiece.row < rowToMove) { // Top right
						tempCol++;
						tempRow++;
					}
					else if (startingPiece.col > colToMove && startingPiece.row < rowToMove) { // Top left
						tempCol--;
						tempRow++;
					}
					else if (startingPiece.col < colToMove && startingPiece.row > rowToMove) { // Bottom right
						tempCol++;
						tempRow--;
					}
					else if(startingPiece.col > colToMove && startingPiece.row > rowToMove) { // Bottom left
						tempCol--;
						tempRow--;
					}
				}
				else if (startingPiece.type == 'n' || startingPiece.type == 'N') { // Knights
					if (startingPiece.col - 1 == colToMove && startingPiece.row + 2 == rowToMove) { // Left 1 and then up 2
						tempCol--;
						tempRow = tempRow + 2;
					}
					else if (startingPiece.col - 2 == colToMove && startingPiece.row + 1 == rowToMove) { // Left 2 and then up 1
						tempCol = tempCol - 2;
						tempRow++;
					}
					else if (startingPiece.col - 2 == colToMove && startingPiece.row - 1 == rowToMove) { // Left 2 and then down 1
						tempCol = tempCol - 2;
						tempRow--;
					}
					else if (startingPiece.col - 1 == colToMove && startingPiece.row - 2 == rowToMove) { // Left 1 and then down 2
						tempCol--;
						tempRow = tempRow - 2;
					}
					else if (startingPiece.col + 1 == colToMove && startingPiece.row - 2 == rowToMove) { // Right 1 and then down 2
						tempCol++;
						tempRow = tempRow - 2;
					}
					else if (startingPiece.col + 2 == colToMove && startingPiece.row - 1 == rowToMove) { // Right 2 and then down 1
						tempCol = tempCol + 2;
						tempRow--;
					}
					else if (startingPiece.col + 2 == colToMove && startingPiece.row + 1 == rowToMove) { // Right 2 and then up 1
						tempCol = tempCol + 2;
						tempRow++;
					}
					else if (startingPiece.col + 1 == colToMove && startingPiece.row + 2 == rowToMove) { // Right 1 and then up 2
						tempCol++;
						tempRow = tempRow + 2;
					}
				}
				else if (startingPiece.type == 'p' || startingPiece.type == 'P') { // Pawns
					
				}
				if (list.findWithPos(tempCol, tempRow) != null) { // If a piece is found at that position in the path
					if (tempCol == colToMove && tempRow == rowToMove) { // If piece reached move spot, there could be a different piece on it
						if (startingPiece.color == list.findWithPos(tempCol, tempRow).color) { // If the different piece has a different color, it can be captured, otherwise move is not valid
							return false;
						}
						continue;
					}
					return false;
				}
			}
			return true; // Piece can move safely to the spot
		}
		else { // If piece cannot physically move to spot (based on piece rules)
			return false;
		}
	}

	static boolean isKingInCheck(LinkedList list, ChessPiece startingPiece) { // Checks if king is in check
		ChessPiece kingPiece;

		// Finds king depending on which color just went
		if (Character.isUpperCase(startingPiece.type)) { // If black just went
			kingPiece = list.findWithPiece('K');
		}
		else {
			kingPiece = list.findWithPiece('k'); // If whtie just went 
		}

		int kingCol = kingPiece.col;
		int kingRow = kingPiece.row;
		int tempCol = kingPiece.col;
		int tempRow = kingPiece.row;

		ChessPiece attackingPiece;

		// Checks all directions of king to see which piece might be a threat; first loops until it hits edge of board or finds a piece, then checks if the piece can even physically move to the king and attack it
		while (tempRow != 8) { // Above king
			tempRow++;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempRow != 1) { // Below king
			tempRow--;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempCol != 8) { // Right of king
			tempCol++;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempCol != 1) { // Left of king
			tempCol--;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempCol != 8 && tempRow != 8) { // Top right of king
			tempCol++;
			tempRow++;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempCol != 1 && tempRow != 8) { // Top left of king
			tempCol--;
			tempRow++;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempCol != 8 && tempRow != 1) { // Bottom right of king
			tempCol++;
			tempRow--;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		while (tempCol != 1 && tempRow != 1) { // Bottom left of king
			tempCol--;
			tempRow--;
			if (list.findWithPos(tempCol, tempRow) != null) { // Checks if there is a piece at position
				attackingPiece = list.findWithPos(tempCol, tempRow); // Finds piece at that position
				if (canMove(list, attackingPiece, kingPiece) && attackingPiece.isAttacking(kingPiece)) { // Checks if piece can physically move to the king and attack it
					return true;
				}
				break; // Stop looping if piece cannot move or attack king, no need to look further in that direction since it would block other pieces
			}
		}
		// Reset column and row
		tempCol = kingCol;
		tempRow = kingRow;

		return false; // If no pieces and check the king
	}
}