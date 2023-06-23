class Queen {
	int col;
	int row;

	public Queen (int c, int r) {
		col = c;
		row = r;
	}

	boolean isAttacking(Queen q) { // Queen.isAttacking(other queen)
		int currentCol = col;
		int currentRow = row;

		if (col == q.col || row == q.row) { // Horizontal and vertical
			return true;
		}
		else if (col < q.col && row < q.row) { // Top right
			for (int i = currentCol; i <= q.col; i++) {
				if (currentCol == q.col && currentRow == q.row) {
					return true;
				}
				currentCol++;
				currentRow++;
			}
		}
		else if (col > q.col && row < q.row) { // Top left
			for (int i = currentCol; i >= q.col; i--) {
				if (currentCol == q.col && currentRow == q.row) {
					return true;
				}
				currentCol--;
				currentRow++;
			}
		}
		else if (col < q.col && row > q.row) { // Bottom right
			for (int i = currentCol; i <= q.col; i++) {
				if (currentCol == q.col && currentRow == q.row) {
					return true;
				}
				currentCol++;
				currentRow--;
			}
		}
		else if (col > q.col && row > q.row) { // Bottom left
			for (int i = currentCol; i >= q.col; i--) {
				if (currentCol == q.col && currentRow == q.row) {
					return true;
				}
				currentCol--;
				currentRow--;
			}
		}
		return false;
	}
}