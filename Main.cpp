//************************************************************************************************************************************
//		CPrawn is made and created by JJ Lowe. The concept is based off of an Atari game, Surround which was a port of Blockade. (1976-1977)
//
//		The idea was to make a functional Atari-esque/Ascii game with all the restrictions of C++.
//
//		This was basically just made to stretch my knowledge of C++ and to pass time. 
//
//		Any code used in this program can be used for educational purposes or for personal programs.
//************************************************************************************************************************************

#include <conio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#define _WIN32_WINNT 0x0500
#include <Windows.h>

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
void gotoXY(int x, int y)	// For the record this isn't the same as goto in code
							// This refers to moving the cursor on the screen so that everything gets printed in the correct place
							// This is how you can do games in C++ without having to reprint the screen every time
							// It becomes more complicated as essentially two layers are presented, one being the vector of vectors below
							// The second being where the cursor is on top, you compare the two to get hit detection
							
{
	CursorPosition.X = x; // Locates column
	CursorPosition.Y = y; // Locates Row
	SetConsoleCursorPosition(console,CursorPosition); // Sets position for next thing to be printed 
}

void remove_scrollbar()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    COORD new_size = 
    {
        info.srWindow.Right - info.srWindow.Left + 1,
        info.srWindow.Bottom - info.srWindow.Top + 1
    };
    SetConsoleScreenBufferSize(handle, new_size);

	// This sets the screen buffer size to the actual size of the screen 
	// Forcing the scrollbar to go away
}

void MakeArena(vector <vector <char> >& ArenaO, vector <char>& vcharO, int lengthO, int widthO, int P1ScoreO, int P2ScoreO)
{
	for (int i = 0; i < lengthO; i++) // This is 1 vector of blanks
		vcharO.push_back(' ');

	for (int i = 0; i < widthO; i++) // This pushes that one vector on width amount of times
		ArenaO.push_back(vcharO);
		
	for (int i = 0; i < widthO; i++) // This just makes the sides look nice
		for (int j = 0; j < lengthO; j += (lengthO - 1))
			ArenaO[i][j] = '|';

	for (int i = 0; i < widthO; i += (widthO - 1)) // This just makes the top and bottom look nice
		for (int j = 0; j < lengthO; j ++)
			ArenaO[i][j] = '-';

	ArenaO[0][0] = '+'; ArenaO[widthO - 1][lengthO - 1] = '+'; ArenaO[0][lengthO - 1] = '+'; ArenaO[widthO - 1][0] = '+'; // These are the corners

	for (int i = 0; i < widthO; i++) // This prints it out
	{
		for (int j = 0; j < lengthO; j++)
			cout << ArenaO[i][j];

		cout << endl;
	}

	// Below is the printing of scores

	gotoXY(5, widthO + 3);
	cout << "P1 Score: " << P1ScoreO;

	gotoXY(lengthO/2 - 13, widthO + 4);
	cout << "Press F1 to return to Menu";			

	gotoXY(lengthO - 17, widthO + 3);
	cout << "P2 Score: " << P2ScoreO;
}

void SetStart(int& xO, int& yO, char ColorO, int PlayerNumO, int lengthO, int widthO)
{
	// All this does is place the initial starting player and set the x y coordinates

	xO = lengthO - 5; yO = widthO / 2;
	gotoXY(xO, yO);
	SetConsoleTextAttribute(console, ColorO);
	cout << PlayerNumO;
	SetConsoleTextAttribute(console, 7);

}
 

int main(int argc, char* argv[])
{
	int length = 79; // The length of the actual arena
	int width = 20; // The size of the actual arena
	int P1Score, P2Score = 0; // The default score
	int PlayerNum = 1; // The default character for the player
	int input = 75; // To start the player moving left
	int Color = 3; // The default light blue color of the player
	int Speed = 150; // The default speed
	int TempSpeed;

	vector <vector <char> > Arena; 
	vector <char> vchar; // The vector of vectors that makes up the arena

	remove_scrollbar(); // Removes scroll bar for an easier viewing experience

	//--------------------------------------------------------------------------------------------------------------------------------------------

	int option;

	do
	{
		system("cls");
		cout << "================================================================================";
		cout << "======                                                                    ======";
		cout << "==                                  CPrawn                                    ==";
		cout << "======                                                                    ======";
		cout << "================================================================================";
		cout << "\n";
		cout << "\n";
	
		cout << "      =============\n";
		cout << "      =  OPTIONS  =\n";
		cout << "      =============\n";
		cout << "      \n";
		cout << "      1. Change Player Color\n";
		cout << "      2. Change Player Number\n";
		cout << "      3. Change Game Speed\n";
		cout << "      4. Change Arena Size\n";
		cout << "      5. Start Battle!\n";
		cout << "      6. Exit\n";

		do
		{
			if (cin.fail()) // Checker
			{
				cin.clear();
				cin.get();
			}
			cin >> option;

		} while (cin.fail() || option > 6 || option < 1);

		//--------------------------------------------------------------------------------------------------------------------------------------------

		if (option == 1) // Shows available console colors and allows you to pick 
		{ 
			do 
			{
				system("cls");

				cout << "Pick a Color: (1-256) ";

				for (int i = 1; i < 255; i++)
				{
					SetConsoleTextAttribute(console, i);
					cout << i << " ";
				}

				cout << "\n\n";

				SetConsoleTextAttribute(console, 7);

				if (cin.fail())
				{
					cin.clear();
					cin.get();
				}

				cin >> Color;
			}	while (cin.fail() || Color > 256 || Color < 1);

			option = 0;

			//--------------------------------------------------------------------------------------------------------------------------------------------
		}
		else if (option == 2) // Allows you to pick your Player's character
		{ 
			do 
			{
				system("cls");

				cout << "Pick a Number (0-9): \n";

				if (cin.fail())
				{
					cin.clear();
					cin.get();
				}

				cin >> PlayerNum;
			}	while (cin.fail() || PlayerNum > 9 || PlayerNum < 0);

			option = 0;

			//--------------------------------------------------------------------------------------------------------------------------------------------
		}
		else if (option == 3)	// Let's you pick the speed you want the character to move
		{						// The variable changed changes how long the system sleeps in the while loop
			do 
			{
				system("cls");

				cout << "Pick a speed: (1-3)\n";
				SetConsoleTextAttribute(console, 4);
				cout << "1: Slow\n";
				SetConsoleTextAttribute(console, 6);
				cout << "2: Medium\n";
				SetConsoleTextAttribute(console, 2);
				cout << "3: Fast\n";
				SetConsoleTextAttribute(console, 7);

				if (cin.fail())
				{
					cin.clear();
					cin.get();
				}

				cin >> TempSpeed;

				if (TempSpeed == 1)
					Speed = 200;
				else if (TempSpeed == 2)
					Speed = 150;
				else if (TempSpeed == 3)
					Speed = 100;
			}	while (cin.fail() || TempSpeed > 3 || TempSpeed < 1);

			option = 0;

			//--------------------------------------------------------------------------------------------------------------------------------------------
		}
		else if (option == 5)
		{
			system("cls");
			SetConsoleTextAttribute(console, 7);

			MakeArena(Arena, vchar, length, width, 0, 0);
			int x, y;
			SetStart(x, y, Color, PlayerNum, length, width);			

			//--------------------------------------------------------------------------------------------------------------------------------------------

			while(1)
			{	
				int tempinput = input;
				
					if (_kbhit()) // This is affected through the conio.h file, kbhit detects a keyboard press
						input = _getch(); // This then reads the buffer to see if anything was entered and extracts it

				// 75 is left, 72 is up, 80 is down, and 77 is right on the keyboard
				if (input != 72 && input != 75 && input != 77 && input != 80 && input != 59)
				{
					input = tempinput;
					continue;
				}

				if (input == 59) // 59 is F1 and breaks the loop taking you to the main menu
				{
					Arena.clear();
					input = 75;
					P2Score = 0;
					P1Score = 0;
					break;
				}
				else if (input == 72) // The x y coordinate changes depending on the direction
					y -= 1;
				else if (input == 75)
					x -= 1;
				else if (input == 77)
					x += 1;
				else if (input == 80)
					y += 1;

				//--------------------------------------------------------------------------------------------------------------------------------------------

				if (Arena[y][x] != ' ') // If the spot is not empty
				{
					if ((width - x) < 8) // This was put in place the the text did not wrap
					{
						gotoXY(x-6, y);
						cout << "P1 Died";
						Beep(250,500);
					}
					else
					{
						gotoXY(x, y);
						cout << "P1 Died";
						Beep(250,500);
					}

					P2Score += 1;

					char PauseWorkaround = _getch(); // This is a pause work around lol

					system("cls");

					Arena.clear();
					MakeArena(Arena, vchar, length, width, P1Score, P2Score);
					SetStart(x, y, Color, PlayerNum, length, width);

					input = 75;
					continue;
				}
				else // The place was empty, carries on as usual
				{
					gotoXY(x, y);
					SetConsoleTextAttribute(console, Color);
					cout << PlayerNum;
					SetConsoleTextAttribute(console, 7);
				}

				if (input == 72) // Places an X on the vector coord where your character was so you can't run into your own wall
					Arena[y+1][x] = 'X';
				else if (input == 75)
					Arena[y][x+1] = 'X';
				else if (input == 77)
					Arena[y][x-1] = 'X';
				else if (input == 80)
					Arena[y-1][x] = 'X';

				Sleep(Speed); // Gives it the Tron speed and effect

				//--------------------------------------------------------------------------------------------------------------------------------------------
			}
		}
	} while (option != 6); // Repeat until they quit

	system("PAUSE");
}