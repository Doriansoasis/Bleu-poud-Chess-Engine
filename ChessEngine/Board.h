#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Enums.h"
#include "Move.h"
#include <ppl.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <ctime>

using namespace concurrency;
using namespace std;

class Board {
private:
	piece_color turn_order = white;
	bool isComputerWhite = true;
	bool canCastleleft = true;
	bool canCastleright = true;
	bool isChecked = false;

	int whitescore = 0;
	int blackscore = 0;

	const int maxIterations = 5;

	Piece* currentposition[8][8];

public:	
	Board();
	~Board();
	
	int CalculateMoveValue(piece_color turn, Piece* position[8][8], int iterationsleft);
	inline vector<Move> GenerateMovelist(piece_color turn, Piece* position[8][8]);
	inline int developmentIncentive(piece_color turn, Move move);

	void ProcessPlayerMove();
	void ProcessComputerMove();
	void PlayComputerMove(Move mover);
	void ShowBoardState();
	void cycleTurn();
	
	void getPawnMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getRookMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getKnightMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getBishopMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getQueenMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getKingMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	inline int getPieceValue(piece_type type);


	int charToInt(char y);
	char intToChar(int x);
};


#endif