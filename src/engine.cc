#include "engine.h"

void Engine::initBoard() {
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++) {
			board[i][j] = ' ';
		}
	}

	turn = 'w';
}

void Engine::importFen(char const *fen) {
	int column;
	int cursor = 0;

	// Fill board
	for (int i = 0; i < SIZE; i++)
	{
		column = 0;
		while (fen[cursor] != '/' && fen[cursor] != ' ')
		{
			if (std::isalpha(fen[cursor])) {
				board[i][column] = fen[cursor];
				column++;
			} else if (std::isdigit(fen[cursor])) {
				column = column + (fen[cursor] - '0');
			} else {
				std::cout << "ERROR importing fen" << std::endl;
				return; 
			}
			cursor++;
		}
		cursor++;
	}

	cursor++;

	// Fill turn
	turn = fen[cursor];
}

void Engine::printBoard() {
	char line = '8';
	char col = 'a';
	int i;

	if (turn == 'b') {
		std::cout << "Black's turn" << std::endl;
	} else {
		std::cout << "White's turn" << std::endl;
	}

	// Print board
	for (i = 0; i < SIZE; i++)
	{
		std::cout << line << " | ";
		for (int j = 0; j < SIZE; j++) {
			std::cout << board[i][j] << " | ";
		}
		std::cout << std::endl;
		line--;
	}
	std::cout << "    _______________________________" << std::endl;
	std::cout << "    ";
	for (i = 0; i < SIZE; ++i)
	{
		std::cout << col << " | ";
		col++;
	}
	std::cout << std::endl;
}