#ifndef ENGINE_H
#define ENGINE_H

#define SIZE 8
#include <iostream>
#include <list>
#include <vector>
using namespace std;

class Engine {
	private:
		vector<char> board;
		char turn; // Who's turn ? 'w' white's turn, 'b' black's turn
		bool canWQ; // Can white king castle queen side?
		bool canWK;	// Can white king castle king side?
		bool canBQ; // Can black king castle queen side?
		bool canBK; // Can black king castle king side?


		void initBoard();
		bool isKingChecked();
		bool isOpponentPiece(char piece);
		bool isOwnPiece(char piece);

		list<int> getMoves(char piece, int line, int col);
		list<int> getRookMoves(int line, int col);
		list<int> getKnightMoves(int line, int col);
		list<int> getBishopMoves(int line, int col);
		list<int> getQueenMoves(int line, int col);
		list<int> getPawnMoves(int line, int col);
		list<int> getKingMoves(int line, int col);

		void getScorePieces(int &whiteScore, int &blackScore);

		void changeTurn();
		bool inBoard(int line, int col);

		void getBestMove(char &piece, int &line, int &col);
	public:
		Engine() {
			initBoard();
		}

		void printBoard();

		void importFen(char const *fen);
		int getScore();
};

#endif