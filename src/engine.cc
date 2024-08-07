#include "engine.h"

void printList(list<int> l);
bool isBlackPiece(char piece);
bool isWhitePiece(char piece);
float getPieceScore(char const piece, int const line, int const col);

float kingScores[8][8] = {
	{0, -2, -3, -4, -4, -3, -2, 0},
	{-1, -2, -2, -3, -3, -2, -2, -1},
	{-2, -3, -3, -3, -3, -3, -3, -2},
	{-2, -2, -2, -3, -3, -2, -2, -2},
	{-1, -2, -2, -2, -2, -2, -2, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1},
	{0, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0},
	{1, 2, 1, 0, 0, 1, 2, 1}
};

float knightScores[8][8] = {
    {-0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5},
    {-0.4, -0.2,  0.0,  0.2,  0.2,  0.0, -0.2, -0.4},
    {-0.3,  0.0,  0.5,  0.6,  0.6,  0.5,  0.0, -0.3},
    {-0.3,  0.2,  0.6,  0.8,  0.8,  0.6,  0.2, -0.3},
    {-0.3,  0.2,  0.6,  0.8,  0.8,  0.6,  0.2, -0.3},
    {-0.3,  0.0,  0.5,  0.6,  0.6,  0.5,  0.0, -0.3},
    {-0.4, -0.2,  0.0,  0.2,  0.2,  0.0, -0.2, -0.4},
    {-0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5}
};

float bishopScores[8][8] = {
    {-0.2, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.2},
    {-0.4, 0.5,  0.0,  0.0,  0.0,  0.0, 0.5, -0.4},
    {-0.3,  0.0,  0.5,  0.4,  0.4,  0.5,  0.0, -0.3},
    {-0.3,  0.0,  0.4,  0.8,  0.8,  0.4,  0.0, -0.3},
    {-0.3,  0.0,  0.4,  0.8,  0.8,  0.4,  0.0, -0.3},
    {-0.3,  0.0,  0.5,  0.4,  0.4,  0.5,  0.0, -0.3},
    {-0.4, 0.5,  0.0,  0.0,  0.0,  0.0, 0.5, -0.4},
    {-0.2, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.2}
};

float queenScores[8][8] = {
    {-0.2, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.2},
    {-0.4, 0.2,  0.2,  0.2,  0.2,  0.2, 0.2, -0.4},
    {-0.3,  0.3,  0.5,  0.4,  0.4,  0.5,  0.0, -0.3},
    {-0.3,  0.3,  0.4,  0.8,  0.8,  0.4,  0.0, -0.3},
    {-0.3,  0.3,  0.4,  0.8,  0.8,  0.4,  0.0, -0.3},
    {-0.3,  0.3,  0.5,  0.4,  0.4,  0.5,  0.0, -0.3},
    {-0.4, 0.2,  0.2,  0.2,  0.2,  0.2, 0.2, -0.4},
    {-0.2, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.2}
};

int DEPTH = 5;

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
	whiteKingPos = 0;
	blackKingPos = 0;
	nbMoves = 0;
}

// Import a fen position (Forsyth-Edwards Notation) into our board
void Engine::importFen(char const *fen) {
	int column;
	int cursor = 0;
	int i;

	// Fill board
	for (i = 0; i < SIZE; i++)
	{
		column = 0;
		while (fen[cursor] != '/' && fen[cursor] != ' ')
		{
			if (isalpha(fen[cursor])) {
				board[i * SIZE + column] = fen[cursor];
				if (board[i * SIZE + column] == 'K') {
					whiteKingPos = i * SIZE + column;
				} else if (board[i * SIZE + column] == 'k') {
					blackKingPos = i * SIZE + column;
				}
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
	cursor++;

	if (fen[cursor] == '-') {
		canWQ = false;
		canWK = false;
		canBQ = false;
		canBK = false;
		cursor++;
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

	cursor++; // en passant data;
	while(fen[cursor] != ' ') {
		cursor++;
	}

	cursor++; // Nb king moves;
	while(fen[cursor] != ' ') {
		cursor++;
	}

	cursor++;

	// Nb moves;
	i = 0;
	while(cursor < strlen(fen)) {
		nbMoves = nbMoves * 10 * i + (fen[cursor] - '0');
		cursor++;
		i++;
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
			if (board[(line + 2) * SIZE + col] == ' ' && board[(line + 1) * SIZE + col] == ' ') {
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
		if (line == 6) {
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

float Engine::getScore() {

	float whiteScore = 0;
	float blackScore = 0;

	getScorePieces(whiteScore, blackScore);

	return whiteScore - blackScore;
}

void Engine::getScorePieces(float &whiteScore, float &blackScore) {
	int i;
	list<int> moves;
	list<int>::iterator it;

	for (i = 0; i < SIZE * SIZE; i++) {
		if (isWhitePiece(board[i])) {
			whiteScore = whiteScore + getPieceScore(board[i], i / SIZE, i % SIZE);
		} else if (isBlackPiece(board[i])) {
			blackScore = blackScore + getPieceScore(board[i], i / SIZE, i % SIZE);
		}
	}
}

bool isBlackPiece(char const piece) {
	return islower(piece) != 0;
}

bool isWhitePiece(char const piece) {
	return piece != ' ' && !isBlackPiece(piece);
}

float getPieceScore(char const piece, int const line, int const col) {
	char piece_t = tolower(piece);

	switch (piece_t) {
		case 'r':
			return 5.0;
		case 'q':
			return 9.0 + queenScores[line][col];
		case 'b':
			return 3.0 + bishopScores[line][col];
		case 'n':
			return 3.0 + knightScores[line][col];
		case 'p':
			return 1.0;
		case 'k':
			return 10000.0;
	}

	return 0.0;
}

void Engine::getBestMove() {
	char bestPiece = ' ';
	int fromLine = 0;
	int fromCol = 0;
	int bestLine = 0;
	int bestCol = 0;
	int OPS = 0;
	recursiveBestMove(DEPTH, fromLine, fromCol, bestPiece, bestLine, bestCol, -100000, 100000, OPS);

	cout << "Best move : "<< bestPiece << bestLine << bestCol << " From : " << fromLine << fromCol << endl;
	cout << OPS << " positions tested" << endl;
}

float Engine::recursiveBestMove(int depth, int &fromLine, int &fromCol, 
	char &bestPiece, int &bestLine, int &bestCol, float const ABestScore, float const AWorstScore, int &OPS) {

	OPS++;

	if (depth == 0 ) {
		return getScore();
	}

	list<int>::iterator it;
	list<int> moves;

	vector<char> board_temp;

	int i, j;

	char currentPiece;
	float currentScore;
	
	int currentLine;
	int currentCol;

	float bestScore = -100000;
	float worstScore = 100000;

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

					currentScore = recursiveBestMove(depth - 1, i, j, bestPiece, bestLine, bestCol, bestScore, worstScore, OPS);

					changeTurn();

					board = board_temp;

					if ((currentScore > 9000 && turn == 'w') || (currentScore < -9000 && turn == 'b') ) {
						if (depth == DEPTH) {
							fromLine = i;
							fromCol = j;
							bestPiece = currentPiece;
							bestLine = currentLine;
							bestCol	= currentCol;
						}
						return currentScore;
					} else if (currentScore > bestScore) {
						bestScore = currentScore;
						if (turn == 'w') {
							if (bestScore >= AWorstScore) {
								return bestScore;
							}

							if (depth == DEPTH) {
								fromLine = i;
								fromCol = j;
								bestPiece = currentPiece;
								bestLine = currentLine;
								bestCol	= currentCol;
							}
						}
					} else if (currentScore < worstScore) {
						worstScore = currentScore;
						if (turn == 'b') {
							if (worstScore <= ABestScore) {
								return worstScore;
							}

							if (depth == DEPTH && turn == 'b') {
								fromLine = i;
								fromCol = j;
								bestPiece = currentPiece;
								bestLine = currentLine;
								bestCol	= currentCol;
							}
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