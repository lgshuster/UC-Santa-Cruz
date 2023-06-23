// Name: Leonid Shuster, ID #1616143
// Role: Creates node that has object ChessPiece and next
// File name: ChessPiece.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class Node { // Node stores piece of type ChessPiece and pointer for next node
	ChessPiece piece;
	Node next;

	Node(ChessPiece newPiece) { // Constructor for Node
		piece = newPiece;
		next = null;
	}
}