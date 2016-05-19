/**
 * @file MyString.c
 * @author  Dan Kufra
 * @version 1.0
 * @date 04 August 2015
 *
 * @brief Program receives two strings and finds the amount of occurences of string2 in string1.
 *
 * @section DESCRIPTION
 * We have two methods of searching: Cyclic and not Cyclic. For not Cyclic we will search string1
 * only until the end of it. For example, "abcabc" will hold one occurance of "bca".
 * Meanwhile for Cyclic we will search for strings that continue back to the begining of string1.
 * For example, "abcabc" will hold 2 occurances of "bca" one from indexes 1-3 and one from indexes
 * 4-5 and then index 0 again.
 */

// ------------------------------ includes ------------------------------
#include "MyString.h"

// -------------------------- const definitions -------------------------
/*
 * @def FIRST_INDEX
 * @brief This is the first letter of the string, meaning the index 0
 */
#define FIRST_INDEX 0

/*
 * @def NULL_BYTE
 * @brief represents the null byte at the end of a string
 */
#define NULL_BYTE '\0'
/*
 * @def NOT_CYCLIC
 * @brief Cyclic run of the program is represented by not 0, so not cyclic is 0.
 */
#define NOT_CYCLIC 0

/*
 * @def EMPTY
 * @brief Represents and Empty string or empty count
 */
#define EMPTY 0


// ------------------------------ functions -----------------------------

/**
 * @brief Counts the number of occurences of str2 in str1.
 * @param const char* str1: String we are searching in.
 *        const char* str2: String we are searching for.
 *        int isCyclic: an int representing whether the search is Cyclic. 0 for notCyclic. Any 
 *                      other int for Cyclic.
 * @return unsigned int : returns the amount of occurences of str2 in str1. Returns -1 if either
 *                        string was NULL.
 */
unsigned int countSubStr(const char* str1, const char* str2, int isCyclic)
{

    // check whether either string is empty of null.
    if (str1 == NULL || str1[FIRST_INDEX] == NULL_BYTE ||
        str2 == NULL || str2[FIRST_INDEX] == NULL_BYTE)
    {
        return EMPTY;
    }
    // initialize our starting variables of the total count, and indexes i and j and first found.
    int total_count = EMPTY;
    int j = FIRST_INDEX;
    int i = FIRST_INDEX;
    int first_found = FIRST_INDEX;
    
    // must do this loop at least once, it stops once the index of str1 we are at is the last index
    do
    {
        // if we haven't iterated to the end of str2 then enter the if
        if (*(str2 + j) != NULL_BYTE)
        {
            // if the values in str1 and str2 at the current place match, then raise i and j
            if (*(str2 + j) == *(str1 + i))
            {
                // if j is still at the begining when this happens then first found will be i
                if(j == FIRST_INDEX)
                {
                    first_found = i;
                }
                j++;
                i++;
            }
            // if they don't then reset j to keep searching from the start of str2
            else
            {
                // if j is already at the first spot then move i one index forward
                if(j == FIRST_INDEX)
                {
                    i++;
                }
                else
                {
                    i = first_found + 1;
                    j = FIRST_INDEX;
                }
            }
        }
        /*
         if we reached the end of str2 then raise our count and set i to be first found + 1 to
         keep checking 
         */
        if (*(str2 + j) == NULL_BYTE)
        {
            total_count ++;
            i = ++first_found ;
            // if we are not at the end of string 1 then reset index of string 2 to start
            if(*(str1 + i) != NULL_BYTE)
            {
                j = FIRST_INDEX;
            }
        }

        
        // if we reached the end of str1 and haven't finished str2 and it is cyclic then reset i
        if ((isCyclic != NOT_CYCLIC) && (*(str1 + i) == NULL_BYTE)
            && (*(str2 + j) != NULL_BYTE) && j != 0)
        {
            i = FIRST_INDEX;
        }
    
    } while(*(str1 + i) != NULL_BYTE);
    return total_count;
}

/**
 * @brief main which does nothing.
 * @return int 0
 */
int main()
{
    return 0;
}


