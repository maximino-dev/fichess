#include "engine.h"

void printList(list<int> l);

void Engine::initBoard() {
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++) {
			board[i * SIZE + j] = ' ';
		}
	}

	turn = 'w';
}

// Import a fen position (Forsyth-Edwards Notation) into our board
void Engine::importFen(char const *fen) {
	int column;
	int cursor = 0;

	// Fill board
	for (int i = 0; i < SIZE; i++)
	{
		column = 0;
		while (fen[cursor] != '/' && fen[cursor] != ' ')
		{
			if (isalpha(fen[cursor])) {
				board[i * SIZE + column] = fen[cursor];
				column++;
			} else if (isdigit(fen[cursor])) {
				column = column + (fen[cursor] - '0');
			} else {
				cout << "ERROR importing fen" << endl;
				return; 
			}
			cursor++;
		}
		cursor++;
	}

	turn = fen[cursor];

	cout << "King checked ? " << endl << isKingChecked() << endl;
}

void Engine::printBoard() {
	char line = '8';
	char col = 'a';
	int i;

	if (turn == 'b') {
		cout << "Black's turn" << endl;
	} else {
		cout << "White's turn" << endl;
	}

	// Print board
	for (i = 0; i < SIZE; i++)
	{
		cout << line << " | ";
		for (int j = 0; j < SIZE; j++) {
			cout << board[i * SIZE + j] << " | ";
		}
		cout << endl;
		line--;
	}
	cout << "    _______________________________" << endl;
	cout << "    ";
	for (i = 0; i < SIZE; ++i)
	{
		cout << col << " | ";
		col++;
	}
	cout << endl;
}

// Tells if the actual player's king is in check
bool Engine::isKingChecked() {
	list<int>::iterator it;
	list<int> moves;
	char king;

	int i, j;
	int line, col;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (isOpponentPiece(board[i * SIZE + j])) {
				moves = getMoves(board[i * SIZE + j], i, j);
				for (it = moves.begin(); it != moves.end(); ++it)
				{
					line = *it / SIZE;
					col = *it % SIZE;
					if ((turn == 'w' && board[line * SIZE + col] == 'K') ||
						(turn == 'b' && board[line * SIZE + col] == 'k')) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Engine::isOpponentPiece(char piece) {
	if ( (turn == 'b' and isupper(piece) > 0) || (turn == 'w' and islower(piece) > 0) ) {
		return true;
	}
	return false;
}

bool Engine::isOwnPiece(char piece) {
	return piece != ' ' && !isOpponentPiece(piece);
}

list<int> Engine::getMoves(char piece, int line, int col) {
	list<int> moves;

	changeTurn();

	char piece_t = tolower(piece);

	switch (piece_t) {
		case 'r':
			moves = getRookMoves(line, col);
			break;
		case 'n':
			moves = getKnightMoves(line, col);
			break;
		case 'b':
			moves = getBishopMoves(line, col);
			break;
	}

	changeTurn();

	return moves;
}

// Get possible positions for a rook in the board
list<int> Engine::getRookMoves(int line, int col) {
	list<int> moves;

	int horizontal[4] = {1, -1, 0, 0};
	int vertical[4] = {0, 0, -1, 1};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 4; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		while (board[line_t * SIZE + col_t] == ' ' && inBoard(line_t, col_t)) {
			moves.push_back(line_t * SIZE + col_t);

			line_t = line_t + vertical[i];
			col_t = col_t + horizontal[i];

			if (isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				break;
			}
		}
	}

	return moves;
}

// Get possible positions for a knight in the board
list<int> Engine::getKnightMoves(int line, int col) {
	list<int> moves;

	int horizontal[8] = {2, 2, -2, -2, -1, 1, 1, -1};
	int vertical[8] = {1, -1, 1, -1, 2, 2, -2, -2};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 8; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		if (inBoard(line_t, col_t) && !isOwnPiece(board[line_t * SIZE + col_t])) {
			moves.push_back(line_t * SIZE + col_t);
		}
	}

	return moves;
}

// Get possible positions for a bishop in the board
list<int> Engine::getBishopMoves(int line, int col) {
	list<int> moves;

	int horizontal[4] = {1, 1, -1, -1};
	int vertical[4] = {-1, 1, -1, 1};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 4; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		while (inBoard(line_t, col_t) && board[line_t * SIZE + col_t] == ' ') {
			moves.push_back(line_t * SIZE + col_t);

			line_t = line_t + vertical[i];
			col_t = col_t + horizontal[i];

			if (isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				break;
			}
		}
	}

	return moves;
}


void Engine::changeTurn() {
	if (turn == 'w') {
		turn = 'b';
	} else {
		turn = 'w';
	}
}

bool Engine::inBoard(int line, int col) {
	return (line < SIZE && line >= 0 && col < SIZE && col >= 0);
}

void printList(list<int> l) {
	int line,col;
	list<int>::iterator it;
	for (it = l.begin(); it != l.end(); ++it)
	{
		line = *it / SIZE;
		col = *it % SIZE;
		cout << line << " " << col << endl;
	}
}