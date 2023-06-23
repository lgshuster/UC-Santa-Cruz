// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa3
// Role: Creates list with nodes that store entries, also has various methods to help with matrix operations

class List {
    private Node front; // Keeps track of front of the list
    private Node back; // Keeps track of back of the list
    private Node cursor; // Keeps track of the cursor

    private int listLength; // Keeps track of the length of the list
    private int cursorIndex; // Keeps track of the index of the cursor

    // Doubly linked list, each node has data and next node and previous node
    private class Node {
        private Object data;
        private Node next;
        private Node prev;

        // Constructor for Node
        Node(Object newData) {
            data = newData;
            next = null;
            prev = null;
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
        return listLength;
    }

    // Returns index of the cursor, null if cursor is undefined
    int index() {
        if (cursor == null) {
            cursorIndex = -1;
        }
        return cursorIndex;
    }

    // Returns front node's data, null if there is no front
    Object front() {
        if (listLength > 0) { // If list length has to be greater than 0
            return front.data;
        }
        else {
            return null;
        }
    }

    // Returns back node's data, null if there is no back
    Object back() {
        if (listLength > 0) {
            return back.data;
        }
        else {
            return null;
        }
    }

    // Returns node's data at the cursor's position, null if there is no cursor
    Object get() {
        if (listLength > 0 && cursorIndex >= 0) { // If list length has to be greater than 0 and cursor is defined
            return cursor.data;
        }
        else {
            return null;
        }
    }

    // Returns if two lists are equal
    public boolean equals(Object x) {
        List otherList = (List) x;
        Node thisNode = front;
        Node otherNode = otherList.front;

        boolean isEqualLists = true;

        if (length() != otherList.length()) { // If the two lists aren't the same length, not equal
            isEqualLists = false;
        }
        else {
            while (thisNode != null && otherNode != null) { // Loop until either list ends
                if ((thisNode.data).equals(otherNode.data) == false) { // If one element's data doesn't match the other one, not equal
                    isEqualLists = false;
                    break;
                }
                thisNode = thisNode.next;
                otherNode = otherNode.next;
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
        cursorIndex = -1;
    }

    // Moves cursor to front of the list
    void moveFront() {
        if (listLength != 0) {
            cursor = front;
        }
        cursorIndex = 0;
    }

    // Moves cursor to back of the list
    void moveBack() {
        if (listLength != 0) {
            cursor = back;
        }
        cursorIndex = listLength - 1;
    }

    // Moves cursor one node back
    void movePrev() {
        if (cursor != null && cursor != front) { // If cursor is defined and not at the front, move previous
            cursor = cursor.prev;
            cursorIndex--;
        }
        else if (cursor != null && cursor == front) { // If cursor is defined and at the front, cursor becomes undefined
            cursor = null;
            cursorIndex = -1;
        }
    }

    // Moves cursor one node forward
    void moveNext() {
        if (cursor != null && cursor != back) { // If cursor is defined and not at the back, move next
            cursor = cursor.next;
            cursorIndex++;
        }
        else if (cursor != null && cursor == back) { // If cursor is defined and at the back, cursor becomes undefined
            cursor = null;
            cursorIndex = -1;
        }
    }

    // Adds node with new data to the front of the list
    void prepend(Object newData) {
        Node newElement = new Node(newData);

        if (front == null) { // If list is empty, make the new node the front and back and update cursor
            front = newElement;
            back = newElement;
            cursorIndex = -1;
        }
        else { // If list is not empty, make the new node the front of the list
            newElement.next = front;
            front.prev = newElement;
            front = newElement;
            cursorIndex++;
        }
        listLength++;
    }

    // Adds node with new data to the end of the list
    void append(Object newData) {
        Node newElement = new Node(newData);

        if (front == null) { // If list is empty, make the new node the front and back and update cursor
            front = newElement;
            back = newElement;
            cursorIndex = 0;
        }
        else { // If list is not empty, make the new node the back of the list
            newElement.prev = back;
            back.next = newElement;
            back = newElement;
        }
        listLength++;
    }

    // Adds node with new data before node at cursor's position
    void insertBefore(Object newData) {
        if (listLength > 0 && cursorIndex >= 0) {
            if (cursor.prev == null) { // If trying to insert at the beginning of the list, prepend
                prepend(newData);
            }
            else { // If not at the beginning of the list
                Node newElement = new Node(newData);
                Node beforeCursor = cursor.prev;

                newElement.prev = beforeCursor;
                newElement.next = cursor;
                beforeCursor.next = newElement;
                cursor.prev = newElement;
                cursorIndex++;
                listLength++;
            }
        }
    }

    // Adds node with new data after node at cursor's position
    void insertAfter(Object newData) {
        if (listLength > 0 && cursorIndex >= 0) {
            if (cursor.next == null) { // If trying to insert at the end of the list, append
                append(newData);
            }
            else { // If not at the end of the list
                Node newElement = new Node(newData);
                Node afterCursor = cursor.next;

                newElement.prev = cursor;
                newElement.next = afterCursor;
                cursor.next = newElement;
                afterCursor.prev = newElement;
                listLength++;
            }
        }
    }

    // Deletes node at the front of the list
    void deleteFront() {
        if (listLength > 0) {
            if (cursor == front) { // If cursor is at the front, cursor becomes undefined
                cursor = null;
                cursorIndex = -1;
            }
            front = front.next;
            if (front == null) { // If list becomes empty, clear list
                back = null;
            }
            else { // If list does not become empty, make front's previous node null
                front.prev = null;
            }
            cursorIndex--;
            listLength--;
        }
    }

    // Deletes node at the end of the list
    void deleteBack() {
        if (listLength > 0) {
            if (cursor == back) { // If cursor is at the back, cursor becomes undefined
                cursor = null;
                cursorIndex = -1;
            }
            back = back.prev;
            if (back == null) { // If list becomes empty, clear list
                front = null;
            }
            else { // If list does not become empty, make back's next node null
                back.next = null;
            }
            listLength--;
        }
    }

    // Deletes node at the cursor's position
    void delete() {
        if (listLength > 0 && cursorIndex >= 0) {
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
                cursorIndex = -1;
                listLength--;
            }
        }
    }

    // Returns string form of list, utilizes string form of entry
    public String toString() {
        Node current = front;

        String listString = "";
        while (current != null) { // Loop until end of the list creating string
            listString = listString + String.valueOf(current) + " ";
            current = current.next;
        }
        return listString;
    }
}