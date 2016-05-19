/**
 * @file StringChange.c
 * @author  Dan Kufra
 * @version 1.0
 * @date 23 July 2015
 * @brief Program receives a user inputted string and convert is
 * @section DESCRIPTION
 * This program converts a string as follows:
 * If a character is a lowercase letter, convert to uppercase.
 * If a character is an uppercase letter, convert to lowercase.
 * If a character is between 5 and 9, convert to 8.
 * If a character is between 0 and 4 convert to 0.
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>

// -------------------------- const definitions -------------------------
#define NUM_CHANGE_ONE '8'
#define NUM_CHANGE_TWO '0'
#define LOVER_NUM '0'
#define MIDDLE_NUM '5'
#define UPPER_NUM '9'
#define BIG_UPPER_LETTER 'Z'
#define SMALL_UPPER_LETTER 'z'
#define SMALL_LOWER_LETTER 'a'
#define BIG_LOWER_LETTER 'A'
#define NULL_BYTE '\0'
// Set a macro for maximum string input length supported
#define MAX_STRING_LENGTH 51
//the ASCII value for changing between upper and lower case letters.
#define ASCII_CHANGE 32

// ------------------------------ functions -----------------------------
/** 
 * @brief convertString function receives a string and converts it
 * @param inputString String user inputted and we wish to convert
 */
void convertString(char* inputString)
{
	int i = 0;
	char newString[MAX_STRING_LENGTH] = {NULL_BYTE};
	
	while (inputString[i] != NULL_BYTE)
	{
		char character = inputString[i];
		if (character >= BIG_LOWER_LETTER && character <= BIG_UPPER_LETTER)
		{
			character += ASCII_CHANGE;
		}
		else if (character >= SMALL_LOWER_LETTER && character <= SMALL_UPPER_LETTER)
		{
			character -= ASCII_CHANGE;
		}
		else if (character >= MIDDLE_NUM && character <= UPPER_NUM)
		{
			character = NUM_CHANGE_ONE;
		}
		else if (character < MIDDLE_NUM && character >= LOVER_NUM)
		{
			character = NUM_CHANGE_TWO;
		}
		newString[i] = character;
		i++;
	}
	printf("\"%s\" -> \"%s\"\n", inputString, newString);

}


/**
 * @brief Main function, asks the user for a string input and calls 
 * convertString and converts it
 */
int main()
{
	char inputString[MAX_STRING_LENGTH];
	printf("Please enter a string: ");
	scanf("%[^\n]s", inputString);
	convertString(inputString);
	
	return 0;
}
