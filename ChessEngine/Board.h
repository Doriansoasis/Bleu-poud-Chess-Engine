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

	vector<Piece*> blackTeam;
	vector<Piece*> whiteTeam;

	const int maxIterations = 4;

	Piece* currentposition[8][8];

public:	
	Board();
	~Board();
	void cycleTurn();

	int CalculateMoveValue(piece_color turn, Piece* position[8][8], int iterationsleft);

	vector<Move> GenerateMovelist(piece_color turn, Piece* position[8][8]);

	void ProcessPlayerMove();
	void ProcessComputerMove();

	void PlayComputerMove(Move mover);

	void ShowBoardState();
	int getPieceValue(piece_type type);
	void getPawnMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getRookMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getKnightMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getBishopMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getQueenMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	void getKingMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist);
	int developmentIncentive(piece_color turn, Move move);

	int charToInt(char y);
	char intToChar(int x);
};


#endif