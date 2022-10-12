#include "Move.h"

Move::Move(int ox, int oy, int nx, int ny, int score){
	oldx = ox;
	oldy = oy; 
	newx = nx; 
	newy = ny;
	scoredif = score;
}