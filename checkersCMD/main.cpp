#include <iostream>
using namespace std;
#include "Board.h"
int main() {
	Board board;
	(*(board.getSoldiers()->begin()))->getMoveLogic(&board);
	return 0;
}