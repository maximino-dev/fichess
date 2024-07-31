#ifndef ENGINE_H
#define ENGINE_H

#define SIZE 8
#include <iostream>
#include <list>
using namespace std;

class Engine {
	private:
		char board[SIZE * SIZE];
		char turn; // Who's turn ? 'w' white's turn, 'b' black's turn

		void initBoard();
		bool isKingChecked();
		bool isOpponentPiece(char piece);
		bool isOwnPiece(char piece);

		list<int> getMoves(char piece, int line, int col);
		list<int> getRookMoves(int line, int col);
		void changeTurn();
		bool inBoard(int line, int col);
	public:
		Engine() {
			initBoard();
		}

		void printBoard();

		void importFen(char const *fen);
};

#endif