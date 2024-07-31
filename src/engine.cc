#include "engine.h"

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

	cout << "King checked ? " << isKingChecked() << endl;
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

// Tells if the king is in check
bool Engine::isKingChecked() {
	list<int>::iterator it;
	list<int> moves;
	char king;

	int i, j;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (board[i * SIZE + j] != ' ' && isOpponentPiece(board[i * SIZE + j])) {
				moves = getMoves(board[i * SIZE + j], i, j);
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
	return !isOpponentPiece(piece);
}

list<int> Engine::getMoves(char piece, int line, int col) {
	list<int> moves;

	changeTurn();

	if (tolower(piece) == 'r') {
		moves = getRookMoves(line, col);
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
	return (line < 8 && line >= 0 && col < 8 && col >= 0);
}