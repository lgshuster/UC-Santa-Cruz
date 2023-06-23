// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa1
// Role: Creates list with nodes that store data, also has various methods to help sort list

class List {
	private Node front; // Keeps track of front of the list
	private Node back; // Keeps track of back of the list
	private Node cursor; // Keeps track of the curso 

	private int listLength; // Keeps track of the length of the list
	private int cursorIndex; // Keeps track of the index of the cursor

	// Doubly linked list, each node has data and next node and previous node
	private class Node {
		private int data;
		private Node prev;
		private Node next;

		// Constructor for Node
		Node(int newData) {
			data = newData;
			prev = null;
			next = null;
		}

		// Returns string of node's data
		public String toString() {
			return String.valueOf(data);
		}
	}

	// Constructor for list
	void List() {
		front = null;
		back = null;
		cursor = null;
		listLength = 0;
		cursorIndex = 0;
	}

	// Returns length of the list
	int length() {
		Node current = front;

		listLength = 0;
		while (current != null) { // Loops until end of the list
			listLength++;
			current = current.next; // Moves onto next node
		}
		return listLength;
	}

	// Returns index of the cursor
	int index() {
		cursorIndex = 0;
		if (cursor != null) { // If cursor is defined
			Node current = front;

			while (current != cursor) { // Loops through nodes until cursor is found
				cursorIndex++;
				current = current.next;
			}
		}
		else { // If cursor is undefined
			cursorIndex = -1;
		}
		return cursorIndex;
	}

	// Returns front node's data
	int front() {
		length();
		if (listLength > 0) { // List length has to be greater than 0
			return front.data;
		}
		else {
			return -1;
		}
	}

	// Returns back node's data
	int back() {
		length();
		if (listLength > 0) { // List length has to be greater than 0
			return back.data;
		}
		else {
			return -1;
		}
	}

	// Returns node's data at the cursor's position
	int get() {
		length();
		index();
		if (listLength > 0 && cursorIndex >= 0) {
			return cursor.data;
		}
		else {
			return -1;
		}
	}

	// Returns if two lists are equal
	boolean equals(List L) {
		Node currentList = front;
		Node otherList = L.front;

		boolean isEqualLists = true;

		if (length() != L.length()) { // If the two lists aren't the same length
			isEqualLists = false;
		}
		else {
			while (currentList != null || otherList != null) { // Loop until either list ends
				if (currentList.data != otherList.data) { // If one element's data doesn't match the other one
					isEqualLists = false;
					break;
				}
				currentList = currentList.next;
				otherList = otherList.next;
			}
		}
		
		return isEqualLists;
	}

	// Clears list  
	void clear() {
		front = null;
		back = null;
		cursor = null;
		listLength = 0;
		cursorIndex = 0;
	}

	// Moves cursor to front of the list
	void moveFront() {
		length();
		if (listLength != 0) {
			cursor = front;
		}
	}

	// Moves cursor to back of the list
	void moveBack() {
		length();
		if (listLength != 0) {
			cursor = back;
		}
	}

	// Moves cursor one node back
	void movePrev() {
		if (cursor != null && cursor != front) { // If cursor is defined and not at the front, move previous
			cursor = cursor.prev;
		}
		else if (cursor != null && cursor == front) { // If cursor is defined and at the front, cursor becomes undefined
			cursor = null;
		}
	}

	// Moves cursor one node forward
	void moveNext() {
		if (cursor != null && cursor != back) { // If cursor is defined and not at the back, move next
			cursor = cursor.next;
		}
		else if (cursor != null && cursor == back) { // If cursor is defined and at the back, cursor becomes undefined
			cursor = null;
		}
	}

	// Adds node with new data to the front of the list
	void prepend(int data) {
		Node newElement = new Node(data);

		if (front == null) { // If list is empty, make the new node the front and back
			front = newElement;
			back = newElement;
			cursor = front;
		}
		else { // If list is not empty, make the new node the front of the list
			newElement.next = front;
			front.prev = newElement;
			front = newElement;
		}
	}

	// Adds node with new data to the end of the list
	void append(int data) {
		Node newElement = new Node(data);

		if (front == null) { // If list is empty, make the new node the front and back
			front = newElement;
			back = newElement;
			cursor = front;
		}
		else { // If list is not empty, make the new node the back of the list
			newElement.prev = back;
			back.next = newElement;
			back = newElement;
		}
	}

	// Adds node with new data before node at cursor's position
	void insertBefore(int data) {
		length();
		index();
		if (listLength > 0 && cursorIndex >= 0) { // If list length is greater than 0 and cursor is defined
			Node newElement = new Node(data);

			if (cursor.prev == null) { // If trying to insert at the beginning of the list, make the new node the front of the list
				front = newElement;
				newElement.next = cursor;
				cursor.prev = newElement;
			}
			else { // If not at the beginning of the list
				Node beforeCursor = cursor.prev;

				newElement.prev = beforeCursor;
				newElement.next = cursor;
				beforeCursor.next = newElement;
				cursor.prev = newElement;	
			}	
		}
		
	}

	// Adds node with new data after node at cursor's position
	void insertAfter(int data) {
		length();
		index();
		if (listLength > 0 && cursorIndex >= 0) { // If list length is greater than 0 and cursor is defined
			Node newElement = new Node(data);
			Node afterCursor = cursor.next;

			if (cursor.next == null) { // If trying to insert at the end of the list, make the new node the end of the list
				back = newElement;
				newElement.prev = cursor;
				cursor.next = newElement;
			}
			else { // If not at the end of the list
				newElement.prev = cursor;
				newElement.next = afterCursor;
				cursor.next = newElement;
				afterCursor.prev = newElement;
			}
		}
	}

	// Deletes node at the front of the list
	void deleteFront() {
		length();
		if (listLength > 0) { // If list length is greater than 0
			if (cursor == front) { // If cursor is at the front, cursor becomes undefined
				cursor = null;
			}
			front = front.next;
			if (front == null) { // If list becomes empty, make front and back null
				front = null;
				back = null;
			}
			else { // If list does not become empty, make front's previous node null
				front.prev = null;
			}
		}
	}

	// Deletes node at the end of the list
	void deleteBack() {
		length();
		if (listLength > 0) { // If list length is greater than 0
			if (cursor == back) { // If cursor is at the back, cursor becomes undefined
				cursor = null;
			}
			back = back.prev;
			if (back == null) { // If list becomes empty, make front and back null
				front = null;
				back = null;
			}
			else { // If list does not become empty, make back's next node null
				back.next = null;
			}
		}
	}

	// Deletes node at the cursor's position
	void delete() {
		length();
		index();
		if (listLength > 0 && cursorIndex >= 0) { // If list length is greater than 0 and cursor is defined
			if (cursor == front) { // If cursor is at the front, delete front node
				deleteFront();
			}
			else if (cursor == back) { // If cursor is at the back, delete back node
				deleteBack();
			}
			else { // If cursor is somewhere in between
				Node beforeCursor = cursor.prev;
				Node afterCursor = cursor.next;

				beforeCursor.next = cursor.next;
				afterCursor.prev = cursor.prev;

				cursor = null;
			}
		}
	}

	// Returns string of all nodes' data
	public String toString() {
		Node current = front;

		String listString = "";
		while (current != null) { // Loops until end of the list 
			listString = listString + current.data + " ";
			current = current.next;
		}
		return listString;
	}

	// Returns copied list
	List copy() {
		Node current = front;
		List copyList = new List();

		while (current != null) { // Loops until end of the list appending data into the new list
			copyList.append(current.data);
			current = current.next;
		}
		return copyList;
	}

	// Returns concatenated list
	List concat(List L) {
		Node currentList = front;
		Node otherList = L.front;
		List concatList = new List();

		while (currentList != null) { // Loops until end of the list appending data into the new list
			concatList.append(currentList.data);
			currentList = currentList.next;
		}

		while (otherList != null) {
			concatList.append(otherList.data); // Loops until end of the list appending data into the same new list
			otherList = otherList.next;
		}
		return concatList;
	}
}