#ifndef PIECE_H
#define PIECE_H
#include "Enums.h"

class Piece {
private: 
	piece_color color;
	piece_type type;

public:
	piece_color getcolor() { return color; };
	piece_type gettype() { return type; };

	Piece(piece_color c, piece_type t);

	void promote() { type = queen; }
};

#endif