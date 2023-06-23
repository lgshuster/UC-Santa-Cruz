// Name: Leonid Shuster, ID #1616143
// Role: Subclass for Knight object, has method for checking attackers
// File name: Knight.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class Knight extends ChessPiece{
	public Knight(char t, String clr, int c, int r) {
		super(t, clr, c, r); // Call constructor of super class
	}

	boolean isAttacking(ChessPiece other) { // Piece.isAttacking(other piece)
		int currentCol = col;
		int currentRow = row;

		if (color != other.color) { // Only attack if opposite colors
			if (col - 1 == other.col && row + 2 == other.row) { // Left 1 and then up 2
				return true;
			}
			else if (col - 2 == other.col && row + 1 == other.row) { // Left 2 and then up 1
				return true;
			}
			else if (col - 2 == other.col && row - 1 == other.row) { // Left 2 and then down 1
				return true;
			}
			else if (col - 1 == other.col && row - 2 == other.row) { // Left 1 and then down 2
				return true;
			}
			else if (col + 1 == other.col && row - 2 == other.row) { // Right 1 and then down 2
				return true;
			}
			else if (col + 2 == other.col && row - 1 == other.row) { // Right 2 and then down 1
				return true;
			}
			else if (col + 2 == other.col && row + 1 == other.row) { // Right 2 and then up 1
				return true;
			}
			else if (col + 1 == other.col && row + 2 == other.row) { // Right 1 and then up 2
				return true;
			}
		}
		return false;
	}
}