#ifndef ENGINE_H
#define ENGINE_H

#define SIZE 8
#include <cstring>
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
		int whiteKingPos;
		int blackKingPos;

		int nbMoves;

		void initBoard();
		bool isKingChecked();
		bool isOpponentPiece(char const piece);
		bool isOwnPiece(char const piece);

		list<int> getMoves(char const piece, int const line, int const col);
		list<int> getRookMoves(int const line, int const col);
		list<int> getKnightMoves(int const line, int const col);
		list<int> getBishopMoves(int const line, int const col);
		list<int> getQueenMoves(int const line, int const col);
		list<int> getPawnMoves(int const line, int const col);
		list<int> getKingMoves(int const line, int const col);

		void getScorePieces(float &whiteScore, float &blackScore);

		void changeTurn();
		bool inBoard(int const line, int const col);

		float recursiveBestMove(int depth, int &fromLine, int &fromCol, char &bestPiece, int &bestLine, int &bestCol, int &OPS);
		void play(char piece, int const fromLine, int const fromCol, int const line, int const col);
	public:
		Engine() {
			initBoard();
		}

		void printBoard();

		void importFen(char const *fen);
		float getScore();
		void getBestMove();
};

#endif