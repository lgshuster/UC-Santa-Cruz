// Name: Leonid Shuster, ID #1616143
// Role: Subclass for King object, has method for checking attackers
// File name: King.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class King extends ChessPiece{
	public King(char t, String clr, int c, int r) {
		super(t, clr, c, r); // Call constructor of super class
	}

	boolean isAttacking(ChessPiece other) { // Piece.isAttacking(other piece)
		if (color != other.color) { // Only attack if opposite colors
			if (col == other.col && row + 1 == other.row) { // Above
				return true;
			}
			else if (col == other.col && row - 1 == other.row) { // Below
				return true;
			}
			else if (col + 1 == other.col && row == other.row) { // Right
				return true;
			}
			else if (col - 1 == other.col && row == other.row) { // Left
				return true;
			}
			else if (col + 1 == other.col && row + 1 == other.row) { // Top right
				return true;
			}
			else if (col - 1 == other.col && row + 1 == other.row) { // Top left
				return true;
			}
			else if (col + 1 == other.col && row - 1 == other.row) { // Bottom right
				return true;
			}
			else if (col - 1 == other.col && row - 1 == other.row) { // Bottom left
				return true;
			}
		}
		return false;
	}
}