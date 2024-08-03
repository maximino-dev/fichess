#include "engine.h"

void printList(list<int> l);
bool isBlackPiece(char piece);
bool isWhitePiece(char piece);
int getPieceScore(char piece);

void Engine::initBoard() {

	int i;
	for (i = 0; i < SIZE * SIZE; i++) {
		board.push_back(' ');
	}

	turn = 'w';
	canWQ = true;
	canWK = true;
	canBQ = true;
	canBK = true;

	DEPTH = 5;
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

	cursor++;

	if (fen[cursor] == '-') {
		canWQ = false;
		canWK = false;
		canBQ = false;
		canBK = false;
	} else {
		while(fen[cursor] != ' ') {
			if (fen[cursor] == 'k') {
				canWK = true;
			} else if (fen[cursor] == 'q') {
				canWQ = true;
			} else if (fen[cursor] == 'K') {
				canBK = true;
			} else if (fen[cursor] == 'Q') {
				canBQ = true;
			}
			cursor++;
		}
	}
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
				changeTurn();
				moves = getMoves(board[i * SIZE + j], i, j);
				changeTurn();

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

bool Engine::isOpponentPiece(char const piece) {
	if ( (turn == 'b' and isupper(piece) > 0) || (turn == 'w' and islower(piece) > 0) ) {
		return true;
	}
	return false;
}

bool Engine::isOwnPiece(char const piece) {
	return piece != ' ' && !isOpponentPiece(piece);
}

list<int> Engine::getMoves(char const piece, int const line, int const col) {
	list<int> moves;

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
		case 'q':
			moves = getQueenMoves(line, col);
			break;
		case 'k':
			moves = getKingMoves(line, col);
			break;
		case 'p':
			moves = getPawnMoves(line, col);
			break;
	}

	return moves;
}

// Get possible positions for a rook in the board
list<int> Engine::getRookMoves(int const line, int const col) {
	list<int> moves;

	int horizontal[4] = {1, -1, 0, 0};
	int vertical[4] = {0, 0, -1, 1};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 4; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		if (inBoard(line_t, col_t) && isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				continue;
		}

		while (board[line_t * SIZE + col_t] == ' ' && inBoard(line_t, col_t)) {
			moves.push_back(line_t * SIZE + col_t);

			line_t = line_t + vertical[i];
			col_t = col_t + horizontal[i];

			if (inBoard(line_t, col_t) && isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				break;
			}
		}
	}

	return moves;
}

// Get possible positions for a knight in the board
list<int> Engine::getKnightMoves(int const line, int const col) {
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
list<int> Engine::getBishopMoves(int const line, int const col) {
	list<int> moves;

	int horizontal[4] = {1, 1, -1, -1};
	int vertical[4] = {-1, 1, -1, 1};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 4; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		if (inBoard(line_t, col_t) && isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				continue;
		}

		while (inBoard(line_t, col_t) && board[line_t * SIZE + col_t] == ' ') {
			moves.push_back(line_t * SIZE + col_t);

			line_t = line_t + vertical[i];
			col_t = col_t + horizontal[i];

			if (inBoard(line_t, col_t) && isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				break;
			}
		}
	}

	return moves;
}

// Get possible positions for a bishop in the board
list<int> Engine::getQueenMoves(int const line, int const col) {
	list<int> moves;

	int horizontal[8] = {1, 1, -1, -1, 1, -1, 0, 0};
	int vertical[8] = {-1, 1, -1, 1, 0, 0, -1, 1};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 8; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		if (inBoard(line_t, col_t) && isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				continue;
		}

		while (inBoard(line_t, col_t) && board[line_t * SIZE + col_t] == ' ') {
			moves.push_back(line_t * SIZE + col_t);

			line_t = line_t + vertical[i];
			col_t = col_t + horizontal[i];

			if (inBoard(line_t, col_t) && isOpponentPiece(board[line_t * SIZE + col_t])) {
				moves.push_back(line_t * SIZE + col_t);
				break;
			}
		}
	}

	return moves;
}

// Get possible positions for a pawn in the board
list<int> Engine::getPawnMoves(int const line, int const col) {
	list<int> moves;

	if (turn == 'b') {
		if (line == 1) {
			if (board[(line + 2) * SIZE + col] == ' ') {
				moves.push_back((line + 2) * SIZE + col);
			}
		}
		if (board[(line + 1) * SIZE + col] == ' ') {
			moves.push_back((line + 1) * SIZE + col);
		}
		if (inBoard(line + 1, col + 1) && isOpponentPiece(board[(line + 1) * SIZE + col + 1])) {
			moves.push_back((line + 1) * SIZE + col + 1);	
		}
		if (inBoard(line + 1, col - 1) && isOpponentPiece(board[(line + 1) * SIZE + col - 1])) {
			moves.push_back((line + 1) * SIZE + col - 1);	
		}
	} else {
		if (line == 7) {
			if (board[(line - 2) * SIZE + col] == ' ') {
				moves.push_back((line - 2) * SIZE + col);
			}
		}
		if (board[(line - 1) * SIZE + col] == ' ') {
			moves.push_back((line - 1) * SIZE + col);
		}
		if (inBoard(line - 1, col + 1) && isOpponentPiece(board[(line - 1) * SIZE + col + 1])) {
			moves.push_back((line - 1) * SIZE + col + 1);	
		}
		if (inBoard(line - 1, col - 1) && isOpponentPiece(board[(line - 1) * SIZE + col - 1])) {
			moves.push_back((line - 1) * SIZE + col - 1);	
		}
	}
	return moves;
}

// Get possible positions for a king in the board
list<int> Engine::getKingMoves(int const line, int const col) {
	list<int> moves;

	int horizontal[8] = {1, 1, -1, -1, 1, -1, 0, 0};
	int vertical[8] = {-1, 1, -1, 1, 0, 0, -1, 1};

	int line_t;
	int col_t;

	int i;
	for (i = 0; i < 8; i++) {
		line_t = line + vertical[i];
		col_t = col + horizontal[i];

		if (inBoard(line_t, col_t) && (board[line_t * SIZE + col_t] == ' ' || 
			isOpponentPiece(board[line_t * SIZE + col_t])) ) {
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

bool Engine::inBoard(int const line, int const col) {
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

int Engine::getScore() {

	int whiteScore = 0;
	int blackScore = 0;

	getScorePieces(whiteScore, blackScore);

	return whiteScore - blackScore;
}

void Engine::getScorePieces(int &whiteScore, int &blackScore) {
	int i;
	for (i = 0; i < SIZE * SIZE; i++) {
		if (isWhitePiece(board[i])) {
			whiteScore = whiteScore + getPieceScore(board[i]);
		} else if (isBlackPiece(board[i])) {
			blackScore = blackScore + getPieceScore(board[i]);
		}
	}
}

bool isBlackPiece(char const piece) {
	return islower(piece) != 0;
}

bool isWhitePiece(char const piece) {
	return piece != ' ' && !isBlackPiece(piece);
}

int getPieceScore(char const piece) {
	char piece_t = tolower(piece);

	switch (piece_t) {
		case 'r':
			return 5;
		case 'q':
			return 9;
		case 'b':
		case 'n':
			return 3;
		case 'p':
			return 1;
		case 'k':
			return 10000;
	}

	return 0;
}

void Engine::getBestMove() {
	char bestPiece = ' ';
	int bestLine = 0;
	int bestCol = 0;
	int OPS = 0;
	recursiveBestMove(DEPTH, bestPiece, bestLine, bestCol, OPS);

	cout << "Best move : "<< bestPiece << bestLine << bestCol << endl;
	cout << OPS << " positions tested" << endl;
}

int Engine::recursiveBestMove(int depth, char &bestPiece, int &bestLine, int &bestCol, int &OPS) {

	int positionScore = getScore();

	if (depth == 0) {
		OPS++;
		return positionScore;
	}

	list<int>::iterator it;
	list<int> moves;

	vector<char> board_temp;

	int i, j;

	char currentPiece;
	int currentScore;
	
	int currentLine;
	int currentCol;

	int bestScore = -100000;
	int worstScore = 100000;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (isOwnPiece(board[i * SIZE + j])) {
				currentPiece = board[i * SIZE + j];
				moves = getMoves(board[i * SIZE + j], i, j);
				for (it = moves.begin(); it != moves.end(); ++it)
				{
					currentLine = *it / SIZE;
					currentCol = *it % SIZE;

					board_temp = board;

					play(currentPiece, i, j, currentLine, currentCol);

					changeTurn();

					currentScore = recursiveBestMove(depth - 1, bestPiece, bestLine, bestCol, OPS);

					changeTurn();

					board = board_temp;

					if ((currentScore > 9000 && turn == 'w') || (currentScore < -9000 && turn == 'b') ) {
						if (depth == DEPTH) {
							bestPiece = currentPiece;
							bestLine = currentLine;
							bestCol	= currentCol;
						}
						return currentScore;
					} else if (currentScore > bestScore) {
						bestScore = currentScore;
						if (depth == DEPTH && turn == 'w') {
							bestPiece = currentPiece;
							bestLine = currentLine;
							bestCol	= currentCol;
						}
					} else if (currentScore < worstScore) {
						worstScore = currentScore;
						if (depth == DEPTH && turn == 'b') {
							bestPiece = currentPiece;
							bestLine = currentLine;
							bestCol	= currentCol;
						}
					}
				}
			}
		}
	}

	if (turn == 'b') {
		return worstScore;
	}
	return bestScore;
}

void Engine::play(char piece, int const fromLine, int const fromCol, int const line, int const col) {
	board[fromLine * SIZE + fromCol] = ' ';
	board[line * SIZE + col] = piece;
}