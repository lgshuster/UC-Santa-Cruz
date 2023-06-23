// Name: Leonid Shuster, ID #1616143
// Role: Subclass for Bishop object, has method for checking attackers
// File name: Bishop.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class Bishop extends ChessPiece{
	public Bishop(char t, String clr, int c, int r) {
		super(t, clr, c, r); // Call constructor of super class
	}

	boolean isAttacking(ChessPiece other) { // Piece.isAttacking(other piece)
		int currentCol = col;
		int currentRow = row;

		if (color != other.color) { // Only attack if opposite colors
			if (col < other.col && row < other.row) { // Top right
				for (int i = currentCol; i <= other.col; i++) {
					if (currentCol == other.col && currentRow == other.row) {
						return true;
					}
					currentCol++;
					currentRow++;
				}
			}
			else if (col > other.col && row < other.row) { // Top left
				for (int i = currentCol; i >= other.col; i--) {
					if (currentCol == other.col && currentRow == other.row) {
						return true;
					}
					currentCol--;
					currentRow++;
				}
			}
			else if (col < other.col && row > other.row) { // Bottom right
				for (int i = currentCol; i <= other.col; i++) {
					if (currentCol == other.col && currentRow == other.row) {
						return true;
					}
					currentCol++;
					currentRow--;
				}
			}
			else if (col > other.col && row > other.row) { // Bottom left
				for (int i = currentCol; i >= other.col; i--) {
					if (currentCol == other.col && currentRow == other.row) {
						return true;
					}
					currentCol--;
					currentRow--;
				}
			}
		}
		return false;
	}
}