#ifndef ENGINE_H
#define ENGINE_H

#define SIZE 8
#include <iostream>
#include <string>

class Engine {
	private:
		char board[SIZE][SIZE];
		char turn; // Who's turn ? 'w' white's turn, 'b' black's turn

		void initBoard(); 
	public:
		Engine() {
			initBoard();
		}

		void printBoard();

		void importFen(char const *fen);
};

#endif