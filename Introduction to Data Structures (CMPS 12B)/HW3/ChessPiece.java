// Name: Leonid Shuster, ID #1616143
// Role: Super class for every chess piece
// File name: ChessPiece.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class ChessPiece {
	char type; // Type of piece
	String color; // Type of color
	int col;
	int row;

	public ChessPiece (char t, String clr, int c, int r) {
		type = t;
		color = clr;
		col = c;
		row = r;
	}

	boolean isAttacking(ChessPiece other) { // Dummy function
		return false;
	}
}