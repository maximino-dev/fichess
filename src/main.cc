/**
 * 
 * Chess engine created by Bogado Maximino (visit my website: maximino.fr)
 * or 
 * contact me (maximino.bogadogarcia@gmail.com)
 * 
 * it uses the Forsyth-Edwards Notation (.fen) to store chess positions
   _______________
8 |_|#|_|#|_|#|_|#|
7 |#|_|#|_|#|_|#|_|
6 |_|#|_|#|_|#|_|#|
5 |#|_|#|_|#|_|#|_|
4 |_|#|_|#|_|#|_|#|
3 |#|_|#|_|#|_|#|_|
2 |_|#|_|#|_|#|_|#|
1 |#|_|#|_|#|_|#|_|
   a b c d e f g h

*/

#include "engine.h"

int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		cout << "\tUsage: ./engine fen_position (with 'fen_position' a valid Forsyth-Edwards Notation) \n\
\tExample: ./engine rnb1kbnr/pppp1ppp/4p3/6q1/8/1P4P1/PBPPPP1P/RN1QKBNR b KQkq - 0 3\n\
The progam then tries to return the best move..." << endl;
		return 1;
	}

	Engine eng;

	eng.importFen(argv[1]);

	eng.printBoard();

	eng.getBestMove();

	return 0;
}