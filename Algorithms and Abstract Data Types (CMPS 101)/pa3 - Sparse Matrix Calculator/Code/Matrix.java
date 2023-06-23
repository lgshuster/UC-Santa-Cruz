// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa3
// Role: Creates matrix with array of lists that each store entries, also has various methods for matrix operations

class Matrix {
    private int matrixSize;
    private int matrixNNZ;
    private List[] rowList;

    private class Entry {
        private int col;
        private double val;

        // Constructor for Entry
        Entry(int newCol, double newVal) {
            col = newCol;
            val = newVal;
        }

        // Checks if two entries are equal
        public boolean equals(Object E) {
            boolean isEqualEntries = true;
            Entry otherEntry = (Entry) E;

            if (col != otherEntry.col || val != otherEntry.val) { // If cols and vals don't match, not equal
                isEqualEntries = false;
            }
            return isEqualEntries;
        }

        // Returns string of entry's data
        public String toString() {
            return "(" + col + ", " + val + ")";
        }
    }

    // Constructor for matrix
    Matrix(int n) {
        if (n >= 1) {
            matrixSize = n;
            matrixNNZ = 0;
            rowList = new List[n]; // Create array of n lists

            for (int i = 0; i < n; i++) { // Create n lists
                rowList[i] = new List();
            }
        }

    }

    // Returns size of matrix
    int getSize() {
        return matrixSize;
    }

    // Returns number of non zero entries
    int getNNZ() {
        return matrixNNZ;
    }

    // Checks if two matrices are equal
    public boolean equals(Object x) {
        Matrix otherMatrix = (Matrix) x;

        boolean isEqualMatrices = true;

        if (matrixSize != otherMatrix.matrixSize || matrixNNZ != otherMatrix.matrixNNZ) { // If matrices don't have same size or same NNZ entries, not equal
            isEqualMatrices = false;
        }
        else {
            for (int i = 0; i < matrixSize; i++) { // Loops through every row of both matrices
                List thisList = rowList[i];
                List otherList = otherMatrix.rowList[i];

                if (thisList.equals(otherList) == false) { // If one list doesn't match the other, not equal matrices
                    isEqualMatrices = false;
                    break;
                }
            }
        }
        return isEqualMatrices;
    }

    // Resets matrix to zero state
    void makeZero() {
        for (int i = 0; i < matrixSize; i++) { // Loops through every list/row and clears it
            rowList[i].clear();
        }
        matrixNNZ = 0;
    }

    // Returns copy of matrix
    Matrix copy() {
        Matrix newMatrix = new Matrix(matrixSize);

        if (matrixNNZ != 0) { // If matrix has entries with non-zero values
            for (int i = 0; i < matrixSize; i++) { // Loops through every row of matrix
                List currentList = rowList[i];

                while (currentList.index() != -1) { // Loop through list adding its entries to new matrix until list ends when cursor falls off
                    Entry currentEntry = (Entry) currentList.get();
                    newMatrix.changeEntry(i + 1, currentEntry.col, currentEntry.val);
                    currentList.moveNext();
                }
                currentList.moveFront(); // Move list cursor back to front
            }
        }
        return newMatrix;
    }

    // Adds entry of value x to i,j position of matrix, only adds non-zero values
    void changeEntry(int i, int j, double x) {
        if ((i >= 1 && i <= matrixSize) && (j >= 1 && j <= matrixSize)) { // Matrix is at least size 1 and smaller than max size
            Entry newEntry = new Entry(j, x);
            List currentList = rowList[i-1];

            if (currentList.length() == 0 && x != 0) { // If list is empty and value is non-zero, append
                currentList.append(newEntry);
            }
            else { // List is not empty
                while (currentList.index() != -1) { // Loop until right position is found, or until list ends when cursor falls off
                    Entry currentEntry = (Entry) currentList.get();

                    if (newEntry.col <= currentEntry.col) { // If new entry belongs before current entry
                        if (x != 0) { // Insert if it's a non-zero number
                            currentList.insertBefore(newEntry);
                        }

                        if (newEntry.col == currentEntry.col) { // If columns are the same, delete old value
                            currentList.delete();
                            matrixNNZ--;
                        }
                        break;
                    }
                    else { // If not in correct position, move cursor forward
                        currentList.moveNext();

                        if (currentList.index() == -1 && x != 0) { // If cursor falls off and value is non-zero, new entry belongs in the back, append
                            currentList.append(newEntry);
                        }
                    }
                }
            }
            currentList.moveFront(); // Move cursor of current list back to the front

            if (x != 0) { // Increment NNZ entries if non-zero value
                matrixNNZ++;
            }
        }
    }

    // Returns matrix multiplied by a scalar
    Matrix scalarMult(double x) {
        Matrix newMatrix = new Matrix(matrixSize);

        if (matrixNNZ != 0) {
            for (int i = 0; i < matrixSize; i++) { // Loops through every row of matrix
                List currentList = rowList[i];

                while (currentList.index() != -1) { // // Loop through list adding its scalar multiples of values in entries to new matrix until list ends when cursor falls off
                    Entry currentEntry = (Entry) currentList.get();
                    newMatrix.changeEntry(i + 1, currentEntry.col, x * currentEntry.val);
                    currentList.moveNext();
                }
                currentList.moveFront(); // Move cursor of current list back to the front
            }
        }
        return newMatrix;
    }

    // Adds two matrices
    Matrix add(Matrix M) {
        Matrix newMatrix = new Matrix(matrixSize);
        Matrix otherMatrix = M.copy();

        if (matrixSize == otherMatrix.matrixSize) { // If matrices are the same size, otherwise can't add
            for (int i = 0; i < matrixSize; i++) { // Loops through every row of both matrices and compares entries pointed by the cursor
                List thisList = rowList[i];
                List otherList = otherMatrix.rowList[i];

                // Loop through lists until one of the cursors fall off and list ends, add entries whose cols come first and move cursor of that list, if two entries have the
                    // same col, add the values of the two entries and move both cursors
                while (thisList.index() != -1 && otherList.index() != -1) {
                    Entry thisEntry = (Entry) thisList.get();
                    Entry otherEntry = (Entry) otherList.get();

                    if (thisEntry.col < otherEntry.col) { // If this list's column is less than other list's column, add this list's entry and move this list's cursor
                        newMatrix.changeEntry(i + 1, thisEntry.col, thisEntry.val);
                        thisList.moveNext();
                    }
                    else if (thisEntry.col > otherEntry.col) { // If this list's column is greater than other list's column, add other list's entry and move other list's cursor
                        newMatrix.changeEntry(i + 1, otherEntry.col, otherEntry.val);
                        otherList.moveNext();
                    }
                    else if (thisEntry.col == otherEntry.col) { // If this list's column is equal to other list's column, add both lists' entries and move both lists' cursors
                        newMatrix.changeEntry(i + 1, thisEntry.col, thisEntry.val + otherEntry.val);
                        thisList.moveNext();
                        otherList.moveNext();
                    }

                }
                if (thisList.index() == -1 && otherList.index() != -1) { // If this list's cursor fell off and list ends, go through other list and add the remaining entries to new matrix
                    while (otherList.index() != -1) { // Loops until cursor falls off and list ends
                        Entry otherEntry = (Entry) otherList.get();

                        newMatrix.changeEntry(i + 1, otherEntry.col, otherEntry.val);
                        otherList.moveNext();
                    }
                }
                else if (thisList.index() != -1 && otherList.index() == -1) { // If other list's cursor fell off and list ends, go through this list and add the remaining entries to new matrix
                    while (thisList.index() != -1) { // Loops until cursor falls off and list ends
                        Entry thisEntry = (Entry) thisList.get();

                        newMatrix.changeEntry(i + 1, thisEntry.col, thisEntry.val);
                        thisList.moveNext();
                    }
                }
                // Move both lists' cursor back to the front
                thisList.moveFront();
                otherList.moveFront();
            }
        }
        return newMatrix;
    }

    // Subtracts two matrices
    Matrix sub(Matrix M) {
        Matrix newMatrix = new Matrix(matrixSize);

        if (matrixSize == M.matrixSize) { // If matrices are the same size, otherwise can't subtract
            newMatrix = add(M.scalarMult(-1)); // To subtract, just multiply second matrix by -1 and add
        }
        return newMatrix;
    }

    // Returns tranpose of matrix
    Matrix transpose() {
        Matrix newMatrix = new Matrix(matrixSize);

        if (matrixSize != 0) {
            for (int i = 0; i < matrixSize; i++) { // Loops through every row and gives its columns as rows and rows as columns

                List currentList = rowList[i];

                while (currentList.index() != -1) { // Loops until cursor falls off and list ends
                    Entry currentEntry = (Entry) currentList.get();
                    newMatrix.changeEntry(currentEntry.col, i + 1, currentEntry.val);
                    currentList.moveNext();
                }
                currentList.moveFront(); // Move cursor of current list back to the front
            }
        }
        return newMatrix;

    }

    // Returns multiplication of two matrices
    Matrix mult(Matrix M) {
        Matrix newMatrix = new Matrix(matrixSize);

        if (matrixSize == M.matrixSize) { // If matrices are the same size, otherwise can't multiply
            // Tranposes second matrix and then takes a single row at a time and gets the dot product of every column, becomes the value for the entry for the new matrix
            Matrix T = M.transpose();

            for (int i = 0; i < matrixSize; i++) { // Loops through every row of first matrix
                List thisList = rowList[i];
                if (thisList.length() != 0) { // If list is not empty
                    for (int j = 0; j < matrixSize; j++) { // Loops through every row (now column) of second matrix
                        List otherList = T.rowList[j];
                        if (otherList.length() != 0) { // If list is not empty
                            double multSum = 0;

                            // Similar to add, but only cares about entries that have the same column. Loop through lists until one of the cursors fall off and list ends, multiply
                                // values of entries whose columns match and otherwise move cursor of list of whichever entry has a smaller column
                            while (thisList.index() != -1 && otherList.index() != -1) {
                                Entry thisEntry = (Entry) thisList.get();
                                Entry otherEntry = (Entry) otherList.get();

                                if (thisEntry.col < otherEntry.col) { // If this list's column is less than other list's column, add this list's entry and move this list's cursor
                                    thisList.moveNext();
                                } else if (thisEntry.col > otherEntry.col) { // If this list's column is greater than other list's column, add other list's entry and move other list's cursor
                                    otherList.moveNext();
                                } else if (thisEntry.col == otherEntry.col) { // If this list's column is equal to other list's column, multiply both lists' entries and move both lists' cursors
                                    multSum = multSum + (thisEntry.val * otherEntry.val);
                                    thisList.moveNext();
                                    otherList.moveNext();
                                }
                            }
                            // Add entry to new matrix with running dot product of a row and column and move both lists' cursors back to the front
                            newMatrix.changeEntry(i + 1, j + 1, multSum);
                            thisList.moveFront();
                            otherList.moveFront();
                        }
                    }
                }
            }
        }
        return newMatrix;
    }

    // Returns string form of matrx, utilizes string form of list and entry
    public String toString() {
        String matrixString = "";

        for (int i = 0; i < matrixSize; i++) { // Loop through every row of matrix

            List currentList = rowList[i];

            if (currentList.length() != 0) { // If list is not empty
                matrixString = matrixString + (i+1) + ": " + String.valueOf(currentList) + "\n";
            }
        }
        return matrixString;
    }


}