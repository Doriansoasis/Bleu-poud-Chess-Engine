#ifndef MOVE_H
#define MOVE_H

#include "Enums.h"

class Move {
private:
	int oldx;
	int oldy;
	int newx;
	int newy;
	int scoredif;

public:
	Move() {};
	Move(int ox, int oy, int nx, int ny, int score);
	int getOldx() { return oldx; }
	int getOldy() { return oldy; }
	int getNewx() { return newx; }
	int getNewY() { return newy; }
	int getscoredif() { return scoredif; }
	void addToScoreDif(int data) { scoredif += data; }
	
};

#endif
