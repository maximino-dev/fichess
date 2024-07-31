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

#include<iostream>
#include "engine.h"

int main() {
	
	Engine eng;

	eng.importFen("1nbqkbnr/1ppp1ppp/2r5/p3p3/4P3/2K5/PPPP1PPP/RNBQ1BNR w k - 4 5");

	eng.printBoard();

	return 0;
}