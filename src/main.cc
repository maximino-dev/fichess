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

int main() {
	
	Engine eng;

	eng.importFen("rnbqkbnr/pppp1ppp/8/8/3Pp3/3Q3N/PPP1PPPP/RNB1KB1R b KQkq - 1 3");

	eng.printBoard();

	eng.getBestMove();

	return 0;
}