// Name: Leonid Shuster, ID #1616143
// Role: Subclass for Pawn object, has method for checking attackers
// File name: Pawn.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class Pawn extends ChessPiece{
	public Pawn(char t, String clr, int c, int r) {
		super(t, clr, c, r); // Call constructor of super class
	}

	boolean isAttacking(ChessPiece other) { // Piece.isAttacking(other piece)
		int currentCol = col;
		int currentRow = row;

		if (color != other.color) {
			if (color == "white") {
				if (col - 1 == other.col && row + 1 == other.row) {
					return true;
				}
				else if (col + 1 == other.col && row + 1 == other.row) {
					return true;
				}
			}
			else if (color == "black") {
				if (col - 1 == other.col && row - 1 == other.row) {
					return true;
				}
				else if (col + 1 == other.col && row - 1 == other.row) {
					return true;
				}
			}
		}
		return false;
	}
}