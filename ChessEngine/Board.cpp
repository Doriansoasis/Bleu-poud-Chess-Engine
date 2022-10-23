#include "Board.h"

Board::Board()
{
	//to clean, maybe

	for (int i = 0; i < 8; i++)
	{
		for (int j = 2; j < 6; j++)
			currentposition[i][j] = nullptr;
	}


	for (int i = 0; i < 8; i++)
	{
		Piece* newpiece = new Piece(white, pawn);
		currentposition[i][1] = newpiece;
	}	
	Piece* newpiece1 = new Piece(white, rook);
	currentposition[0][0] = newpiece1;
	Piece* newpiece2 = new Piece(white, knight);
	currentposition[1][0] = newpiece2;
	Piece* newpiece3 = new Piece(white, bishop);
	currentposition[2][0] = newpiece3;
	Piece* newpiece4 = new Piece(white, queen);
	currentposition[3][0] = newpiece4;
	Piece* newpiece5 = new Piece(white, king);
	currentposition[4][0] = newpiece5;
	Piece* newpiece6 = new Piece(white, bishop);
	currentposition[5][0] = newpiece6;
	Piece* newpiece7 = new Piece(white, knight);
	currentposition[6][0] = newpiece7;
	Piece* newpiece8 = new Piece(white, rook);
	currentposition[7][0] = newpiece8;

	for (int i = 0; i < 8; i++)
	{
		Piece* newpiece = new Piece(black , pawn);
		currentposition[i][6] = newpiece;
	}
	Piece* newpiece9 = new Piece(black, rook);
	currentposition[0][7] = newpiece9;
	Piece* newpiece10 = new Piece(black, knight);
	currentposition[1][7] = newpiece10;
	Piece* newpiece11 = new Piece(black, bishop);
	currentposition[2][7] = newpiece11;
	Piece* newpiece12 = new Piece(black, queen);
	currentposition[3][7] = newpiece12;
	Piece* newpiece13 = new Piece(black, king);
	currentposition[4][7] = newpiece13;
	Piece* newpiece14 = new Piece(black, bishop);
	currentposition[5][7] = newpiece14;
	Piece* newpiece15 = new Piece(black, knight);
	currentposition[6][7] = newpiece15;
	Piece* newpiece16 = new Piece(black, rook);
	currentposition[7][7] = newpiece16;
}


Board::~Board()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (currentposition[i][j] != nullptr)
			{
				Piece* todelete = currentposition[i][j];
				delete todelete;
			}
		}
	}
}

void Board::cycleTurn()
{
	if (turn_order == white)
		turn_order = black;
	else turn_order = white;
}

int Board::getPieceValue(piece_type type)
{
	switch(type) {
	case pawn:
		return 4;
	case rook: 
		return 20;
	case knight:
		return 12;
	case bishop:
		return 12;
	case queen:
		return 36;
	case king:
		return 400;
	}
}

void Board::ProcessPlayerMove() {
	char oldxchar, newxchar;
	int oldx, oldy, newx, newy;
	cout << endl << endl << "Please choose a legal move, I can't stop you from cheating or breaking everything:(";
	cout << endl << "Enter x coordinate for chosen piece (A to F): ";
	cin >> oldxchar;

	oldx = charToInt(oldxchar);

	cout << "Enter y coordinate for chosen piece : (1-8): ";
	cin >> oldy;

	oldy--;

	cout << "Enter x coordinate for destination (A-H): ";
	cin >> newxchar;

	newx = charToInt(newxchar);

	cout << "Enter y coordinate for destination (1-8): ";
	cin >> newy;
	newy--;
	
	if (currentposition[oldx][oldy]->gettype() == king && oldy-newy == 0)
	{
		if (newx - oldx == 2)
		{
			currentposition[5][oldy] = currentposition[7][oldy];
			currentposition[7][oldy] = nullptr;
		}

		else if (newx - oldx == -2)
		{
			currentposition[3][oldy] = currentposition[0][oldy];
			currentposition[0][oldy] = nullptr;
		}
	}

	else if (currentposition[newx][newy] != nullptr)
	{
		delete currentposition[newx][newy];
	}
	
	currentposition[newx][newy] = currentposition[oldx][oldy];
	currentposition[oldx][oldy] = nullptr;


	//Check if enemy is checked for castling purposes
	if (canCastleleft || canCastleright) {
		cout << "is the AI king in check? (Y/N) : " << endl;

		char choice;

		cin >> choice;

		switch (toupper(choice)) {
		case 'Y':
			isChecked = true;
			break;
		case 'N':
			isChecked = false;
			break;
		default:
			isChecked = false;
			cout << "not in check assumed"<< endl;
		}
	}
	
	cycleTurn();
	
	ShowBoardState();

	ProcessComputerMove();
}

void Board::ProcessComputerMove() {

	CalculateMoveValue(turn_order, currentposition, maxIterations);

	cycleTurn();
	
	ShowBoardState();

	ProcessPlayerMove();
}

int Board::developmentIncentive(piece_color turn, Move move)
{
	int incentive = 0;

	//incentive to control center four squares
	if ((move.getNewx() == 3 || move.getNewx() == 4) && (move.getNewY() == 3 || move.getNewY() == 4))
		incentive =incentive + 2;

	//incentive to move forward and not back
	if ((turn == white && move.getNewY() > move.getOldy()) || (turn == black && move.getNewY() < move.getOldy()))
	{
		incentive++;
	}
	else if (move.getNewY() != move.getOldy())
		incentive--;

	//incentive to develop backline
	if ((turn == white && move.getOldy() == 0) || (turn == black && move.getOldy() == 7))
		incentive++;

	return incentive;
}

int Board::CalculateMoveValue(piece_color turn, Piece* position[8][8], int iterationsleft) {
	vector<Move> PossibleMoves = GenerateMovelist(turn, position);

	for (Move &move : PossibleMoves)
	{
		move.addToScoreDif(developmentIncentive(turn, move));
		
		if ((move.getNewY() == 0 || move.getNewY() == 7) && position[move.getOldx()][move.getOldy()]->gettype() == pawn)
		{
			move.addToScoreDif(32);
		}	
	}

	if (iterationsleft > 0)
	{
		for(auto& move: PossibleMoves)
		{
			Piece* BufferPiece = position[move.getNewx()][move.getNewY()];

			position[move.getNewx()][move.getNewY()] = position[move.getOldx()][move.getOldy()];
			position[move.getOldx()][move.getOldy()] = nullptr;
			

			if (move.getscoredif() < 350)
			{
				if (turn == white)
					move.addToScoreDif(-CalculateMoveValue(black, position, iterationsleft - 1));
				else
					move.addToScoreDif(-CalculateMoveValue(white, position, iterationsleft - 1));
			}


			position[move.getOldx()][move.getOldy()] = position[move.getNewx()][move.getNewY()];
			position[move.getNewx()][move.getNewY()] = BufferPiece;
		}

		int bestValue = PossibleMoves[0].getscoredif();

		for (auto& move: PossibleMoves)
		{
			bestValue = (bestValue < move.getscoredif()) ? move.getscoredif() : bestValue;
		}

		if (iterationsleft == maxIterations)
		{
			srand((unsigned)time(0));

			vector<Move> bestMoves;
			for (int i = 0; i < PossibleMoves.size(); i++)
			{
				if (PossibleMoves[i].getscoredif() == bestValue)
					bestMoves.push_back(PossibleMoves[i]);
			}

			cout << "There were " << bestMoves.size() << " good moves" << endl;

			int random = rand() % bestMoves.size();

			PlayComputerMove(bestMoves[random]);
		}

		return bestValue;
	}

	return 0;

}

void Board::PlayComputerMove(Move mover)
{
	//castle
	if (currentposition[mover.getOldx()][mover.getOldy()]->gettype() == king)
	{
		if (mover.getNewx() - mover.getOldx() == 2)
		{
			currentposition[5][mover.getOldy()] = currentposition[7][mover.getOldy()];
			currentposition[7][mover.getOldy()] = nullptr;
		}

		if (mover.getNewx() - mover.getOldx() == -2)
		{
			currentposition[3][mover.getOldy()] = currentposition[0][mover.getOldy()];
			currentposition[0][mover.getOldy()] = nullptr;
		}
		canCastleleft = false;
		canCastleright = false;
	}
	
	if ((canCastleleft || canCastleright) && currentposition[mover.getOldx()][mover.getOldy()]->gettype() == rook)
	{
		if (mover.getOldx() == 0)
		{
			canCastleleft = false;
		}
		else if (mover.getOldx() == 7)
		{
			canCastleright = false;
		}
	}

	if (currentposition[mover.getNewx()][mover.getNewY()] != nullptr)
		delete currentposition[mover.getNewx()][mover.getNewY()];

	currentposition[mover.getNewx()][mover.getNewY()] = currentposition[mover.getOldx()][mover.getOldy()];
	currentposition[mover.getOldx()][mover.getOldy()] = nullptr;

	if (currentposition[mover.getNewx()][mover.getNewY()]->gettype() == pawn && (mover.getNewY() == 0 || mover.getNewY() == 7))
		currentposition[mover.getNewx()][mover.getNewY()]->promote();

	cout << "The play was : ";

	switch (mover.getOldx())
	{
	case 0:
		cout << "A";
		break;
	case 1:
		cout << "B";
		break;
	case 2:
		cout << "C";
		break;
	case 3:
		cout << "D";
		break;
	case 4:
		cout << "E";
		break;
	case 5:
		cout << "F";
		break;
	case 6:
		cout << "G";
		break;
	case 7:
		cout << "H";
		break;
	}

	cout << mover.getOldy() + 1 << " ";

	switch (currentposition[mover.getNewx()][mover.getNewY()]->gettype())
	{
	case pawn:
		cout << "Pawn to ";
		break;
	case rook:
		cout << "Rook to ";
		break;
	case knight:
		cout << "Knight to ";
		break;
	case bishop:
		cout << "Bishop to ";
		break;
	case queen:
		cout << "Queen to ";
		break;
	case king:
		cout << "King to ";
		break;
	}
	

	cout << intToChar(mover.getNewx()) << mover.getNewY()+1<<endl;
	if (mover.getscoredif() > 300)
		cout << "Checkmate or inescapable checkmate" << endl;
	else if (mover.getscoredif() < -300)
		cout << "GOo job, there's no way I can get out of this one" << endl;
}

void Board::ShowBoardState() {

	cout << endl;
	string piece = "";
	for (int i = 7; i >= 0; i--)
	{
		cout << "\t";
		switch (i)
		{
			case 7:
				cout << 8;
				break;
			case 6:
				cout << 7;
				break;
			case 5:
				cout << 6;
				break;
			case 4:
				cout << 5;
				break;
			case 3:
				cout << 4;
				break;
			case 2:
				cout << 3;
				break;
			case 1:
				cout << 2;
				break;
			case 0:
				cout << 1;
				break;
		}
		cout << "     " << "|   ";
		for (int j = 0; j <= 7; j++)
		{
			if (currentposition[j][i] == nullptr)
			{
				piece += "        |  ";
			}
			else
			{
				switch (currentposition[j][i]->getcolor())
				{
				case white:
					piece += "w";
					break;
				case black:
					piece += "b";
					break;
				}

				switch (currentposition[j][i]->gettype())
				{
				case pawn:
					piece += "pawn   |  ";
					break;
				case rook:
					piece += "rook   |  ";
					break;
				case knight:
					piece += "knight |  ";
					break;
				case bishop:
					piece += "bishop |  ";
					break;
				case queen:
					piece += "queen  |  ";
					break;
				case king:
					piece += "king   |  ";
					break;
				}
			}
			cout << piece;
			piece = "";
		}
		cout << endl;
		cout << "\t" << "\t" << "--------------------------------------------------------------------------------------";
		cout << endl;
	}
	cout << "\t" << "\t" <<"    A     |    B     |    C     |    D     |    E     |    F     |    G     |    H" << endl << endl;
	system("pause");
}


vector<Move> Board::GenerateMovelist(piece_color turn, Piece* position[8][8])
{
	vector<Move> movelist;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if(position[i][j] != nullptr)
				if (position[i][j]->getcolor() == turn)
				{
					switch (position[i][j]->gettype())
					{
					case pawn:
						getPawnMoves(i, j, position, movelist);
						break;
					case rook:
						getRookMoves(i, j, position, movelist);
						break;
					case knight:
						getKnightMoves(i, j, position, movelist);
						break;
					case bishop:
						getBishopMoves(i, j, position, movelist);
						break;
					case queen:
						getQueenMoves(i, j, position, movelist);
						break;
					case king:
						getKingMoves(i, j, position, movelist);
						break;
					}
				}
		}
	return movelist;
}

void Board::getPawnMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist) {

	piece_color turn = position[x][y]->getcolor();

	if (turn == white && y != 7)
	{
		if (position[x][y + 1] == nullptr)
		{
			Move legalmove(x, y, x, y+1, 0);
			if (y + 1 == 7)
				legalmove.addToScoreDif(32);
			Movelist.push_back(legalmove);

			if (y == 1 && position[x][y + 2] == nullptr)
			{
				Move legalmove(x, y, x, y + 2, 0);
				Movelist.push_back(legalmove);
			}
		}

		if (x > 0)
		{
			if (position[x-1][y+1] != nullptr)
				if (position[x - 1][y + 1]->getcolor() != turn)
				{
					Move legalmove(x, y, x-1, y + 1, getPieceValue(position[x - 1][y + 1]->gettype()));
					if (y + 1 == 7)
						legalmove.addToScoreDif(32);
					Movelist.push_back(legalmove);
				}
		}

		if (x < 7)
		{
			if (position[x + 1][y + 1] != nullptr)
				if (position[x + 1][y + 1]->getcolor() != turn)
				{
					Move legalmove(x, y, x + 1, y + 1, getPieceValue(position[x + 1][y + 1]->gettype()));
					if (y + 1 == 7)
						legalmove.addToScoreDif(32);
					Movelist.push_back(legalmove);
				}
		}
	}

	else if (turn == black && y != 0)
	{
		if (position[x][y - 1] == nullptr)
		{
			Move legalmove(x, y, x, y - 1, 0);
			if (y - 1 == 0)
				legalmove.addToScoreDif(32);
			Movelist.push_back(legalmove);

			if (y == 6 && position[x][y - 2] == nullptr)
			{
				Move legalmove(x, y, x, y - 2, 0);
				Movelist.push_back(legalmove);
			}
		}

		if (x > 0)
		{
			if (position[x - 1][y - 1] != nullptr)
				if (position[x - 1][y - 1]->getcolor() != turn)
				{
					Move legalmove(x, y, x - 1, y - 1, getPieceValue(position[x - 1][y -1]->gettype()));
					if (y - 1 == 0)
						legalmove.addToScoreDif(32);
					Movelist.push_back(legalmove);
				}
		}

		if (x < 7)
		{
			if (position[x + 1][y - 1] != nullptr)
				if (position[x + 1][y - 1]->getcolor() != turn)
				{
					Move legalmove(x, y, x + 1, y - 1, getPieceValue(position[x + 1][y - 1]->gettype()));
					if (y - 1 == 0)
						legalmove.addToScoreDif(32);
					Movelist.push_back(legalmove);
				}
		}
	}
}

void Board::getRookMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist) {

	piece_color turn = position[x][y]->getcolor();
	
	for (int testx = x-1; testx >= 0; testx--)
	{
		if (position[testx][y] == nullptr)
		{
			Move legalmove(x, y, testx, y,0);
			Movelist.push_back(legalmove);
		}

		else if (position[testx][y]->getcolor() != turn)
		{
			Move legalmove(x, y, testx, y, getPieceValue(position[testx][y]->gettype()));
			Movelist.push_back(legalmove);
			testx = -1;
		}

		else if (position[testx][y]->getcolor() == turn)
			testx = -1;
	}

	for (int testx = x+1; testx <= 7; testx++)
	{
		if (position[testx][y] == nullptr)
		{
			Move legalmove(x, y, testx, y, 0);
			Movelist.push_back(legalmove);
		}

		else if (position[testx][y]->getcolor() != turn)
		{
			Move legalmove(x, y, testx, y, getPieceValue(position[testx][y]->gettype()));
			Movelist.push_back(legalmove);
			testx = 8;
		}

		else if (position[testx][y]->getcolor() == turn)
			testx = 8;
	}

	for (int testy = y-1; testy >= 0; testy--)
	{
		if (position[x][testy] == nullptr)
		{
			Move legalmove(x, y, x, testy, 0);
			Movelist.push_back(legalmove);
		}

		else if (position[x][testy]->getcolor() != turn)
		{
			Move legalmove(x, y, x, testy, getPieceValue(position[x][testy]->gettype()));
			Movelist.push_back(legalmove);
			testy = -1;
		}

		else if (position[x][testy]->getcolor() == turn)
			testy = -1;
	}

	for (int testy = y + 1; testy <= 7; testy++)
	{
		if (position[x][testy] == nullptr)
		{
			Move legalmove(x, y, x, testy, 0);
			Movelist.push_back(legalmove);
		}

		else if (position[x][testy]->getcolor() != turn)
		{
			Move legalmove(x, y, x, testy, getPieceValue(position[x][testy]->gettype()));
			Movelist.push_back(legalmove);
			testy = 8;
		}

		else if (position[x][testy]->getcolor() == turn)
			testy = 8;
	}
}

void Board::getKnightMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist) {

	piece_color turn = position[x][y]->getcolor();

	if (x > 1 && y > 0)
	{
		if (position[x - 2][y - 1] == nullptr)
		{
			Move legalmove(x, y, x - 2, y - 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 2][y - 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 2, y - 1, getPieceValue(position[x - 2][y - 1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x > 0 && y > 1)
	{
		if (position[x - 1][y - 2] == nullptr)
		{
			Move legalmove(x, y, x - 1, y - 2, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 1][y - 2]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 1, y - 2, getPieceValue(position[x - 1][y - 2]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x < 7 && y < 6)
	{
		if (position[x + 1][y + 2] == nullptr)
		{
			Move legalmove(x, y, x + 1, y + 2, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 1][y + 2]->getcolor() != turn)
		{
			Move legalmove(x, y, x +1, y +2, getPieceValue(position[x +1][y +2]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x < 6 && y < 7)
	{
		if (position[x + 2][y + 1] == nullptr)
		{
			Move legalmove(x, y, x+2, y + 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 2][y + 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x +2, y +1, getPieceValue(position[x +2][y +1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x > 1 && y < 7)
	{
		if (position[x - 2][y + 1] == nullptr)
		{
			Move legalmove(x, y, x - 2, y + 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 2][y + 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 2, y + 1, getPieceValue(position[x - 2][y + 1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x > 0 && y < 6)
	{
		if (position[x - 1][y + 2] == nullptr)
		{
			Move legalmove(x, y, x - 1, y + 2, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 1][y + 2]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 1, y + 2, getPieceValue(position[x - 1][y +2]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x < 7 && y > 1)
	{
		if (position[x + 1][y - 2] == nullptr)
		{
			Move legalmove(x, y, x + 1, y - 2, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 1][y - 2]->getcolor() != turn)
		{
			Move legalmove(x, y, x +1, y -2, getPieceValue(position[x +1][y - 2]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x < 6 && y > 0)
	{
		if (position[x + 2][y - 1] == nullptr)
		{
			Move legalmove(x, y, x + 2, y - 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 2][y - 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x +2, y - 1, getPieceValue(position[x +2][y - 1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}
}

void Board::getBishopMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist) {

	piece_color turn = position[x][y]->getcolor();

	for (int quad1 = 1; 7 >= x+quad1 && 7 >= y+quad1; quad1++)
	{
		if (position[x + quad1][y + quad1] == nullptr)
		{
			Move legalmove(x, y, x + quad1, y + quad1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + quad1][y + quad1]->getcolor() == turn)
			quad1 = 8;

		else if (position[x + quad1][y + quad1]->getcolor() != turn)
		{
			Move legalmove(x, y, x + quad1, y + quad1, getPieceValue(position[x + quad1][y + quad1]->gettype()));
			Movelist.push_back(legalmove);
			quad1 = 8;
		}
	}

	for (int quad2 = 1; 0 <= x-quad2 && 7 >= y+quad2; quad2++)
	{
		if (position[x - quad2][y + quad2] == nullptr)
		{
			Move legalmove(x, y, x - quad2, y + quad2, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - quad2][y + quad2]->getcolor() == turn)
			quad2 = 8;

		else if (position[x - quad2][y + quad2]->getcolor() != turn)
		{
			Move legalmove(x, y, x - quad2, y + quad2, getPieceValue(position[x -quad2][y + quad2]->gettype()));
			Movelist.push_back(legalmove);
			quad2 = 8;
		}
	}

	for (int quad3 = 1; 0 <= x-quad3 && 0 <= y-quad3; quad3++)
	{
		if (position[x - quad3][y - quad3] == nullptr)
		{
			Move legalmove(x, y, x - quad3, y - quad3, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - quad3][y - quad3]->getcolor() == turn)
			quad3 = 8;

		else if (position[x - quad3][y - quad3]->getcolor() != turn)
		{
			Move legalmove(x, y, x - quad3, y - quad3, getPieceValue(position[x - quad3][y - quad3]->gettype()));
			Movelist.push_back(legalmove);
			quad3 = 8;
		}
	}

	for (int quad4 = 1; 7 >= x+quad4 && 0 <= y-quad4; quad4++)
	{
		if (position[x + quad4][y - quad4] == nullptr)
		{
			Move legalmove(x, y, x + quad4, y - quad4, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + quad4][y - quad4]->getcolor() == turn)
			quad4 = 8;

		else if (position[x + quad4][y - quad4]->getcolor() != turn)
		{
			Move legalmove(x, y, x + quad4, y - quad4, getPieceValue(position[x + quad4][y - quad4]->gettype()));
			Movelist.push_back(legalmove);
			quad4 = 8;
		}
	}
}

void Board::getQueenMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist) {
	getRookMoves(x, y, position, Movelist);
	getBishopMoves(x, y, position, Movelist);
}

void Board::getKingMoves(int x, int y, Piece* position[8][8], vector<Move>& Movelist) {

	piece_color turn = position[x][y]->getcolor();

	if (x > 0)
	{
		if (position[x - 1][y] == nullptr)
		{
			Move legalmove(x, y, x - 1, y, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 1][y]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 1, y, getPieceValue(position[x - 1][y]->gettype()));
			Movelist.push_back(legalmove);
		}
	}
		

	if (x < 7)
	{
		if (position[x + 1][y] == nullptr)
		{
			Move legalmove(x, y, x + 1, y, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 1][y]->getcolor() != turn)
		{
			Move legalmove(x, y, x + 1, y, getPieceValue(position[x + 1][y]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (y > 0)
	{
		if (position[x][y - 1] == nullptr)
		{
			Move legalmove(x, y, x, y - 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x][y-1]->getcolor() != turn)
		{
			Move legalmove(x, y, x, y-1, getPieceValue(position[x][y-1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (y < 7)
	{
		if (position[x][y + 1] == nullptr)
		{
			Move legalmove(x, y, x, y + 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x][y+1]->getcolor() != turn)
		{
			Move legalmove(x, y, x, y + 1, getPieceValue(position[x][y+1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x > 0 && y > 0)
	{
		if (position[x - 1][y - 1] == nullptr)
		{
			Move legalmove(x, y, x - 1, y - 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 1][y-1]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 1, y-1, getPieceValue(position[x - 1][y-1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x > 0 && y < 7)
	{
		if (position[x - 1][y + 1] == nullptr)
		{
			Move legalmove(x, y, x - 1, y + 1,0);
			Movelist.push_back(legalmove);
		}
		else if (position[x - 1][y + 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x - 1, y+1, getPieceValue(position[x - 1][y + 1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x < 7 && y < 7)
	{
		if (position[x + 1][y + 1] == nullptr)
		{
			Move legalmove(x, y, x + 1, y + 1,0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 1][y + 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x + 1, y+1, getPieceValue(position[x + 1][y + 1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	if (x < 7 && y > 0)
	{
		if (position[x + 1][y - 1] == nullptr)
		{
			Move legalmove(x, y, x + 1, y - 1, 0);
			Movelist.push_back(legalmove);
		}
		else if (position[x + 1][y - 1]->getcolor() != turn)
		{
			Move legalmove(x, y, x + 1, y - 1, getPieceValue(position[x + 1][y - 1]->gettype()));
			Movelist.push_back(legalmove);
		}
	}

	//castling
	if (x == 4 && (y == 0 || y == 7) && !isChecked)
	{
		if (canCastleleft && position[3][y] == nullptr && position[2][y] == nullptr && position[1][y] == nullptr && position[0][y] != nullptr)
			if (position[0][y]->gettype() == rook && position[0][y]->getcolor() == turn)
			{
				Move legalmove(x, y, 2, y, 4);
				Movelist.push_back(legalmove);
			}
		if (canCastleright && position[5][y] == nullptr && position[6][y] == nullptr && position[7][y] != nullptr)
			if (position[7][y]->gettype() == rook && position[7][y]->getcolor() == turn)
			{
				Move legalmove(x, y, 6, y, 4);
				Movelist.push_back(legalmove);
			}
	}
}

int Board::charToInt(char y) {
	switch (toupper(y))
	{
	case 'A':
		return 0;
	case 'B':
		return 1;
	case 'C':
		return 2;
	case 'D':
		return 3;
	case 'E':
		return 4;
	case 'F':
		return 5;
	case 'G':
		return 6;
	case 'H':
		return 7;
	default:
		return -1;
	}
}

char Board::intToChar(int x)
{
	switch (x)
	{
	case 0:
		return 'A';
	case 1:
		return 'B';
	case 2:
		return 'C';
	case 3:
		return 'D';
	case 4:
		return 'E';
	case 5:
		return 'F';
	case 6:
		return 'G';
	case 7:
		return 'H';
	default:
		return -1;
	}
}