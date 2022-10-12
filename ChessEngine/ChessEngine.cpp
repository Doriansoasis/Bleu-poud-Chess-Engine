// ChessEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Board.h"
using namespace std;


int main()
{
	Board board;

	cout << "Welcome to this chess program!!!" << endl << endl;

	cout << "Do yo want to start? (Y/N) : " << endl;

	char choice;

	cin >> choice;

	switch (toupper(choice)) {
	case 'Y':
		board.ShowBoardState();
		board.ProcessPlayerMove();
		break;
	case 'N':
		board.ProcessComputerMove();
		break;
	default:
		cout << "You have chosen....Poorly" << endl << endl << "goobye" << endl;
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
