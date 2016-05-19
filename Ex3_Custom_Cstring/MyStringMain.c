#include "MyString.h"
#include <stdlib.h>

/*
 * @def MESSAGE
 * @brief Message to be added between our two MyStrings in file
 */
#define MESSAGE " is smaller than "
/*
 * @def NULL_BYTE
 * @brief Null byte character
 */
#define NULL_BYTE '\0'
/*
 * @def MAX_SIZE
 * @brief Max size of string to be received
 */
#define MAX_SIZE 500
/*
 * @def PROMPT
 * @brief Message to be printed to user before string inputs
 */
#define PROMPT "Please enter string\n"
/*
 * @def NEW_LINE
 * @brief new line character
 */
#define NEW_LINE "\n"
/*
 * @def FILE_NAME
 * @brief Output file name
 */
#define FILE_NAME "test.out"

/**
 * @brief Writes the appropriate strings to the file.
 */
void stringWriter(const MyString* string1, const MyString* string2, FILE *stream)
{
    myStringWrite(string1, stream);
    fputs(MESSAGE, stream);
    myStringWrite(string2, stream);
    fputs(NEW_LINE, stream);
}

/**
 * @brief Compares the two strings and sends appropriate parameters to stringWriter
 */
void compareAndWriteToFile(const char* str1, const char* str2, FILE *stream)
{
    // allocate two my strings and set them from our c strings
    MyString *myStr1 = myStringAlloc();
    MyString *myStr2 = myStringAlloc();
    myStringSetFromCString(myStr1, str1);
    myStringSetFromCString(myStr2, str2);
    // compare the two and send to stringWriter appropriately
    int comparison = myStringCompare(myStr1, myStr2);
    if(comparison <= 0)
    {
        stringWriter(myStr1, myStr2, stream);
    }
    else
    {
        stringWriter(myStr2, myStr1, stream);
    }
    // flush the stream and free our MyStrings
    fflush(stream);
    myStringFree(myStr1);
    myStringFree(myStr2);
}

/**
 * @brief program receives two strings and outputs the comparison of them to a file called test.out
 *
 */
int main()
{
    // initialize two char arrays the size of maximum size
    char str1[MAX_SIZE] = {NULL_BYTE};
    char str2[MAX_SIZE] = {NULL_BYTE};
    // prompt the user and scan their input
    printf(PROMPT);
    scanf("%s", str1);
    printf(PROMPT);
    scanf(" %[^\n]s", str2);
    // open a file called "test.out" and send it and the strings to compareAndWriteToFile
    FILE *testFile = fopen(FILE_NAME, "w");
    compareAndWriteToFile(str1, str2, testFile);
    fclose(testFile);
    return 0;
}

