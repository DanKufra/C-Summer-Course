/**
 * @file NimGame.c
 * @author  Dan Kufra
 * @version 1.0
 * @date 23 July 2015
 *
 * @brief Program runs a version of the Nim game between 2 human players.
 *
 * @section DESCRIPTION
 * Rules of the game:
 *
 * Users choose amount of balls in each box. If the amount chosen is negative exit program.
 * Player one and two then choose to room X balls in turn from either box.
 * First player to empty a box loses.
 * X can't be 0 or lower. Or larger than 3. Or larger than amount of box. 
 * Turn is replayed if it is.
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>

// -------------------------- const definitions -------------------------
#define MAX_BALLS 3
#define EMPTY 0
#define GAME_TERMINATED -1
#define NULL_BYTE '\0'
#define LINE_DIVIDER 15
const int FIRST = 1, SECOND = 2;

// global variables of balls in each box.
int gBallsOne = NULL_BYTE, gBallsTwo = NULL_BYTE;

// ------------------------------ functions -----------------------------

/**
 * @brief ballInBoxPrint prints a prompt for the user at begining of game.
 * @param int boxNum the number of the box
 */
void ballInBoxPrint(int boxNum)
{
	printf("How many balls in box %d?\n", boxNum);
}



/**
 * @brief checkNegativitiy decides validity of ball amount and prints messages accordingly
 * @param int numBalls number of balls chosen by user.
 * @return 0 if the initialization is valid, -1 (GAME_TERMINATED) if the input was invalid
 */
int checkNegativity(int numBalls)
{
	if (numBalls <= EMPTY)
	{
		printf("Number of balls in box must be positive.\n");
		return GAME_TERMINATED;
	}
	return 0;
}


/**
 * @brief printLineDivider prints a line divider of 15 "-" */
void printLineDivider()
{
	int i = 0;
	for (; i < LINE_DIVIDER; i++)
	{
		printf("-");
	}
	printf("\n");

}

/**
 * @brief printBoxLine prints the amount of balls in the given box as "o"s.
 */
void printBoxLine(int ballsInBox)
{
	int i = 0;
	for(; i < ballsInBox; i++)
	{
		printf("o");
	}
	printf("\n");
}

/**
 * @brief gameBoardPrint prints the game board at the start of each turn.
 */
void gameBoardPrint()
{
	printLineDivider();
	printf("Box 1: ");
	printBoxLine(gBallsOne);
	printf("Box 2: ");
	printBoxLine(gBallsTwo);
	printLineDivider();

}

/**
 * @brief initGame initializes a game by prompting the user to fill 
 * the boxes,handles edge cases.
 * @return 0 if the initialization is valid, -1 (GAME_TERMINATED) if the input was invalid
 */
int initGame()
{
	// prompts user for first box entry, if valid prompts for second.
	ballInBoxPrint(FIRST);
	scanf("%d", &gBallsOne);
	if (checkNegativity(gBallsOne) != GAME_TERMINATED)
	{
		ballInBoxPrint(SECOND);
		scanf("%d", &gBallsTwo);
		return checkNegativity(gBallsTwo);
	}

	return GAME_TERMINATED;
}


/**
 * @brief runGame runs the game
 */
void runGame()
{
	// set the current player to start as the First player, the game boxes aren't empty
	int	player = FIRST;
	while(gBallsOne != EMPTY && gBallsTwo != EMPTY)
	{
		int ballNum = EMPTY, choice = EMPTY;
		gameBoardPrint();

		do
		{
			// prompt the player to choose a box, and continue asking while he chooses wrongly.
			printf("Player %d, it's your turn.\n", player);
			printf("Player %d, choose a box (1 or 2):\n", player);
			scanf("%d", &choice);
		} while (choice != FIRST && choice != SECOND);
		
		// prompt him to pick the amount of balls, continue doing so till he choose correctly
		while(ballNum == EMPTY)
		{
			printf("Player %d, how many balls do you want to"
				   " take from box %d?\n", player, choice);
			scanf("%d", &ballNum);
			if (ballNum <= EMPTY)
			{
				printf("Number of balls to take must be positive.\n");
				ballNum = EMPTY;
			}
			else if (ballNum > MAX_BALLS)
			{
				printf("Cannot take more than %d balls at a time.\n", MAX_BALLS);
				ballNum = EMPTY;
			}
			else if((ballNum > gBallsOne && choice == FIRST) ||
					(ballNum > gBallsTwo && choice == SECOND))
			{
				printf("Cannot take more balls than what's in the box.\n");
				ballNum = EMPTY;
			}
			else
			{
				// remove balls from the correct box
				switch (choice)
				{
					case 1:
						gBallsOne -= ballNum;
						break;
					case 2:
						gBallsTwo -= ballNum;
					default:
						break;
				}
				// switch the current player
				if (player == FIRST)
				{
					player = SECOND;
				}
				else
				{
					player = FIRST;
				}
			}
		}
		
	}
	gameBoardPrint();
	printf("Player %d wins the game.\n", player);
	
}

/**
 * @brief main calls the initialization function and if the game is not 
 * terminated calls runGame in order to run the game.
 */
int main()
{
	if (initGame() != GAME_TERMINATED)
	{
		runGame();
	}
	
	return 0;
}
