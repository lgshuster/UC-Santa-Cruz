// Name: Leonid Shuster, ID #1616143
// Role: Creates Linked List, also had methods for inserting into list, checking valid chessboard, finding piece on query spot, and check if that piece attacks other pieces
// File name: LinkedList.java
// Instructions: Compile with command javac ChessBoard.java or make and then command java -jar ChessBoard.jar "some input" "some output"

class LinkedList {
	Node head;

	void LinkedList() { // Initialize an empty list
		head = null;
	}

	void insert(ChessPiece newPiece) {
		Node latest = new Node(newPiece); // Create new node
		Node current = head; // Keep track of head

		if (head == null) { // If empty list
			head = latest;
		}
		else {
			while (current.next != null) { // Get to end of list
				current = current.next;
			}
			current.next = latest; // Set end of list to newest node
		}
	}

	boolean isValid() {
		Node current = head; // Pointer for current piece
		Node other = head; // Pointer for pieces to check

		while (current != null) { // While pointer for current piece is not at the end of the list
			other = head;
			while (other != null) { // While pointer for other pieces are not at the end of the list
				if (other == current) { // If it loops over itself
					if (other.next == null) { // If pointers are both at the end of the list
						return true;
					}
				}
				else if (current.piece.col == other.piece.col && current.piece.row == other.piece.row) { // If pieces are on the same spot
					return false;
				}
				other = other.next; // Check other pieces for current piece
			}
			current = current.next; // Choose different piece be checked
		}
		return true;
	}

	ChessPiece find(int c, int r) {
		Node current = head; // Pointer for beginning of list
		while(current != null) { // While pointer for piece is not at the end of the list
			if (current.piece.col == c && current.piece.row == r) { // If current piece is on the spot
				return current.piece;
			}
			current = current.next; // Go to next piece
		}
		return null;
	}

	boolean checkAttacking(ChessPiece queryPiece) {
		Node current = head;
		while (current != null) {
			if (current.piece == queryPiece) { // If it loops over itself
				if (current.next == null) {
					return false;
				}
			}
			else if (queryPiece.isAttacking(current.piece)) {
				return true;
			}
			current = current.next;
		}
		return false;
	}
}