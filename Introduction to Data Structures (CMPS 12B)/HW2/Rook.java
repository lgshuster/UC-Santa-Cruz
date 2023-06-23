// Name: Leonid Shuster, ID #1616143
// Role: Subclass for Rook object, has method for checking attackers
// File name: Rook.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class Rook extends ChessPiece{
	public Rook(char t, String clr, int c, int r) {
		super(t, clr, c, r); // Call constructor of super class
	}

	boolean isAttacking(ChessPiece other) { // Piece.isAttacking(other piece)
		int currentCol = col;
		int currentRow = row;

		if (color != other.color) { // Only attack if opposite colors
			if (col == other.col || row == other.row) { // Horizontal and vertical
				return true;
			}
		}
		return false;
	}
}