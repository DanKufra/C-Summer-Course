/********************************************************************************
 * @file MyString.c
 * @author  Dan Kufra
 * @version 1.0
 * @date 13.08.2015
 *
 * @brief The SLabC Standard Strings library.
 *
 * @section LICENSE
 * This program is not a free software;
 *
 * @section DESCRIPTION
 * The LabC Standard Strings library.
 *
 * This library provides a string type with extra capabilities.
 *
 * The library provides the following features:
 *  - basic string operations
 *  - conversion to other types (ints, C strings)
 *
 * Error handling
 * ~~~~~~~~~~~~~~
 * Most functions may fail due to failure to allocate dynamic memory. When
 * this happens the functions will return an appropriate failure value. If this
 * happens, then the state of the other outputs of the function is undefined.
 ********************************************************************************/
/* Answers to implementation details:
 *  Struct implementation:  
 *      I decided to create a struct that holds a pointer to a character array,
 *      an unsigned long representing the length of the string in that array.
 *      And an unsigned long representing the actual amount of memory allocated
 *      to that array. 
 *      I decided to do it this way because while we need to know the length of
 *      the string, we also need the actual size since we do not always want to
 *      allocate more memory. For example, a MyStruct with string length of 8
 *      that is copied to a MyStruct of string length of 7 will not always need
 *      more space. Because it may already have more allocated.
 *      This is because we are in a space rich world and sometimes the "cost" of
 *      rempoving space is more costly than keeping more space to begin with.
 *
 *
 *  Memory allocation:
 *      I allocated memory wherever needed. Mostly for new structs and new c strings.
 *      And I saved memory by using the implementation described previously and setting
 *      a bench mark of 16 bytes to realloc (as described in the doc string of reSizeMyString
 *
 *
 ********************************************************************************/

// ------------------------------ includes ------------------------------
#include "MyString.h"

// -------------------------- constant definitions -------------------------
/*
 * @def MIN
 * @brief Macro function that returns the minimum of two given ints
 */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
/*
 * @def NULL_BYTE
 * @brief A macro representing a null byte
 */
#define NULL_BYTE '\0'
/*
 * @def EMPTY
 * @brief represents an empty array, or zero size
 */
#define EMPTY 0
/*
 * @def START_SIZE
 * @brief Default memory size of an empty MyString
 */
#define START_SIZE 16
/*
 * @def FIRST_INDEX
 * @brief First index in an array
 */
#define FIRST_INDEX 0
/*
 * @def POSITIVE
 * @brief Positive integer
 */
#define POSITIVE 1
/*
 * @def NEGATIVE
 * @brief Negative integer
 */
#define NEGATIVE -1
/*
 * @def MINUS
 * @brief ASCII value of '-'
 */
#define MINUS 45
/*
 * @def NINE
 * @brief ASCII value of '9'
 */
#define NINE 57
/*
 * @def ZERO
 * @brief ASCII value of '0'
 */
#define ZERO 48
/*
 * @def BASE
 * @brief Base double for logorithmic math use
 */
#define BASE 10.0
/*
 * @def CHAR_TO_INT
 * @brief ASCII value needed to add to turn an int into char representation of that int
 */
#define CHAR_TO_INT 48
/*
 * @def SMALLER_THAN_ZERO
 * @brief Numbers smaller than this are negative
 */
#define SMALLER_THAN_ZERO 0
/*
 * @def SAME
 * @brief Value representing equality for comparators
 */
#define SAME 0
/*
 * @def BIGGER
 * @brief Value representing the first element is bigger in a comparator
 */
#define BIGGER 1
/*
 * @def SMALLER
 * @brief Value representing the first element is smaller in a comparator
 */
#define SMALLER -1
/*
 * @def EQUAL
 * @brief Value representing equality for equal functions
 */
#define EQUAL 1
/*
 * @def EQUAL
 * @brief Value representing unequality for equal functions
 */
#define UNEQUAL 0
/*
 * @def STRING_LENGTH
 * @brief Value representing string to be printed in length
 */
#define STRING_LENGTH "MyString length"
/*
 * @def STRING_SIZE
 * @brief Value representing string to printed in size
 */
#define STRING_SIZE "MyString size"

/**
 * @brief MyString represents a manipulable string.
 *        Holds a pointer to a string.
 *        Holds an unsigned long representing the length of that string
 *        Holds and unsigned long representing the actual memory allocated to the string.
 */
struct _MyString
{
    char *stringArray;
    unsigned long stringSize;
    unsigned long realSize;
};

/**
 * @brief Allocates a new MyString and sets its value to "" (the empty string).
 * 			It is the caller's responsibility to free the returned MyString.
 *          Complexity is O(1) + whatever malloc is.
 * RETURN VALUE:
 * @return a pointer to the new string, or NULL if the allocation failed.
 */
static MyString *buildMyString(unsigned long memory)
{
    // creates a new pointer to a MyString struct the size of a MyString struct
    MyString *stringPointer = malloc(sizeof(MyString));
    // allocates enough memory for the array (length of the string) and sets to zero
    stringPointer -> stringArray = malloc(memory);
    // sets the string size and real size to be the length of the string
    stringPointer -> stringSize = memory;
    stringPointer -> realSize = memory;
    return stringPointer;
}

/**
 * @brief Getter for the real size of a MyString
 *        Time complexity is O(1).
 * @param MyString we wish to get the real size of.
 */
static unsigned long myStringRealSize(const MyString * str1)
{
    return str1 -> realSize;
}

/**
 * @brief Function that deals with the resizing of a stringArray.
 *        Checks whether the string needs resizing (either it has too little room, or too much).
 *        The bench mark I set for too much room is 16 bytes. This is because the benchmark placed
 *        by C's string library sets that as the length of a "word" where word is not an actual
 *        word, but rather a memory size value. This is so we reallocate memory only when it is
 *        "worth" it to do so and not for every individual byte.
 *        Time complexity is O(1).
 * @param MyString we wish to maybe resize.
 * @param size we wish to maybe resize it to.
 * RETURN VALUE:
 * @return MYSTRING_SUCCESS on success MYSTRING_ERROR on error
 */
static MyStringRetVal reSizeStringArray(MyString *str, const unsigned long newSize)
{
    // check whether we even need to resize. Either current size is too small or too big
    if (myStringRealSize(str) < newSize || myStringRealSize(str) - newSize > 16)
    {
        // set a temp pointer to stringArray and attempt to realloc it to the new size
        char *temp = str -> stringArray;
        temp = realloc(temp, newSize);
        // if it fails return an error (but we keep the pointer to stringArray)
        if(temp == NULL)
        {
            return MYSTRING_ERROR;
        }
        // otherwise switch the pointers, set temp to NULL, change size and return success
        else
        {
            str -> stringArray = temp;
            temp = NULL;
            str -> realSize = newSize;
        }
    }
    return MYSTRING_SUCCESS;
}

/**
 * @brief finds the length of a c string.
 * @param pointer to a cString.
 * RETURN VALUE:
 * @return length of cString
 */
static unsigned long getCStringLength(const char *cString)
{
    unsigned long i = EMPTY;
    // iterate over the c string until we reach the null byte at the end
    while (*(cString + i) != NULL_BYTE)
    {
        i++;
    }
    return i;
}

/**
 * @brief Function receives an int and finds the number of digits in the int
 *        Time complexity is O(log digitCount). Where digitCount is the amount of digits.
 * @param n the int we want to get the length of.
 * RETURN VALUE:
 * @return (int) digitLength is the amount of digits in the int.
 */
static int getLengthOfInt(int n)
{
    // if it is negative just multiply by -1 to get positive
    if(n < SMALLER_THAN_ZERO)
    {
        n *= NEGATIVE;
    }
    // find the length by flooring the result of value/10 to the power of each iteration
    double digitLength = EMPTY;
    double value = floor(n / pow(BASE, digitLength)) ;
    while (value != EMPTY)
    {
        digitLength ++;
        value = floor(n / pow(BASE, digitLength));
    }
    return (int) digitLength;
}

/**
 * @brief default comparator that compares two chars with a logical comparison.
 *        Time complexity is O(1).
 * @param char1 first char to compare
 * @param char2 second char to compare
 * RETURN VALUE:
 * @return MYSTR_ERROR_CODE if cannot compare, 0 if equal, 1 if char1 is bigger, -1 if char2 is.
 */
static int defCompare(const char *char1, const char *char2)
{
    if(char1 == NULL || char2 == NULL)
    {
        return MYSTR_ERROR_CODE ;
    }
    if(*char1 == *char2)
    {
        return SAME;
    }
    
    else if(*char1 > *char2)
    {
        return BIGGER;
    }
    else
    {
        return SMALLER;
    }
}

/**
 * @brief Equality checker compares two chars.
 *        Time complexity is O(n) where n is size entered.
 * @param Size we want to compare up till
 * @param MyString to check
 * @param second MyString to check
 * @param comparator function
 * RETURN VALUE:
 * @return 0 if equal, -1 or 1 otherwise
 */
static int checkEquality(const unsigned long size, const MyString *str1, const MyString *str2,
                         int (*foo)(const char *compareChar1, const char *compareChar2))
{
    // go over the strings character by character
    for(unsigned long i = 0; i < size; i++)
    {
        char *char1 = (str1 -> stringArray + i);
        char *char2 = (str2 -> stringArray + i);
        //save the result we receive from our comparator
        int tempResult = foo(char1, char2);
        // if they don't match stop and return the result
        if (tempResult != SAME)
        {
            return tempResult;
        }
    }
    return SAME;
}

/**
 * @brief casts the const void* it receives to be pointers to pointers to MyString and then
 *        returns our myStringCompare function with those two pointers to MyStrings.
 *        Time complexity is O(1)
 * @param arr
 * @param len
 *
 * RETURN VALUE: none
 */
static int compareCaster(const void* str1, const void* str2)
{
    // cast str1 and str2 to be two pointers to pointers to MyString and return our compare with it
    MyString** newStr1 = (MyString**) str1;
    MyString** newStr2 = (MyString**) str2;
    return myStringCompare(*newStr1, *newStr2);
}

/**
 * @brief default equality checker compares two chars with a logical comparison.
 *        Time complexity is O(1).
 * @param char1 first char to compare
 * @param char2 second char to compare
 * RETURN VALUE:
 * @return 0 if equal, 1 otherwise
 */
static int logicalEqual(const char *char1, const char *char2)
{
    if (*char1 == *char2)
    {
        return 0;
    }
    return 1;
}


/**
 * Complexity is O(1). Because buildMyString is O(1) and the other actions are also O(1).
 */
MyString * myStringAlloc()
{
    // allocate a new myString of size 16 because we would like to save cost of reallocating later
    MyString *newString = buildMyString(START_SIZE);
    memset(newString -> stringArray, EMPTY, START_SIZE);
    // set the first (and only) index to be a null byte, and it's string size to be 0
    newString -> stringArray[FIRST_INDEX] = NULL_BYTE;
    newString -> stringSize = EMPTY;
    return newString;
}

/**
 * @brief Frees the memory and resources allocated to str.
 *        Time complexity is O(1).
 * @param str the MyString to free.
 * If str is NULL, no operation is performed.
 * It is the user's responsibility to set the pointer to NULL after calling this function
 */
void myStringFree(MyString *str)
{
    // check that str is not null
    if (str != NULL)
    {
        //if it isn't then free the array it's pointer points to
        free(str -> stringArray);
        str -> stringArray = NULL;
        // also free the struct itself
        free(str);
    }
}

/**
 * @brief Allocates a new MyString with the same value as str. It is the caller's
 * 		  responsibility to free the returned MyString.
 *        Time complexity is O(n) where n is the length of str.
 * @param str the MyString to clone.
 * RETURN VALUE:
 *   @return a pointer to the new string, or NULL if the allocation failed.
 */
MyString * myStringClone(const MyString *str)
{
    // declare a MyString do be initialized later
    MyString *newString = NULL;
    unsigned long size = myStringLen(str);
    // if the MyString we are cloning is an "empty" string, then use myStringAlloc to initialize
    if (size == EMPTY)
    {
        newString = myStringAlloc();
        if(newString == NULL)
        {
            return NULL;
        }
    }
    // otherwise allocate a new MyString the size needed and use myStringSetFromMyString to clone
    else
    {
        newString = buildMyString(myStringLen(str));
        if (newString == NULL)
        {
            return NULL;
        }
        myStringSetFromMyString(newString, str);
    }
    return newString;
}

/**
 * @brief Sets the value of str to the value of other.
 *        Time complexity is O(n) where n is the length of other's string.
 * @param str the MyString to set
 * @param other the MyString to set from
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
    // if either string is null then return an error
    if(str == NULL || other == NULL)
    {
        return MYSTRING_ERROR;
    }
    // get the length we want and update our string length
    unsigned long StringLength = myStringLen(other);
    str -> stringSize = StringLength;
    // if the string we are setting from is empty, change the length match the actual space we want
    if (StringLength == 0)
    {
        StringLength = 16;
    }
    // if the length of str is smaller than the length of other then reallocate enough memory
    if (reSizeStringArray(str, StringLength) == MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }
    if(myStringLen(other) == 0)
    {
        StringLength = 1;
    }
    // use memcpy to copy the values from other to str
    memcpy(str -> stringArray, other -> stringArray, StringLength);
    // return success
    return MYSTRING_SUCCESS;
}

/**
 * @brief gets the length of a MyString.
 * @param The MyString we wish to check.
 * @return the length of the string in str1.
 */
unsigned long myStringLen(const MyString *str1)
{
    if(str1 == NULL)
    {
        return (unsigned long) MYSTR_ERROR_CODE;
    }
    return str1 -> stringSize;
}


/**
 * @brief Sets the value of str to the value of the given C string.
 * 			The given C string must be terminated by the null character.
 * 			Checking will not use a string without the null character.
 *          Time complexity is O(n) where n is the length of cString. This is the cost of getting
 *          cString's length and the cost of memcpy from cString to str.
 * @param str the MyString to set.
 * @param cString the C string to set from.
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromCString(MyString *str, const char * cString)
{
    // find the length of cString and check whether the strings are null
    if (str == NULL || cString == NULL)
    {
        return MYSTRING_ERROR;
    }
    // if no errors then proceed
    else
    {
        unsigned long cStringLength = getCStringLength(cString);
        // check whether cString is an empty string
        if (cStringLength == EMPTY)
        {
            // create a temp struct using myStringAlloc() and copy it, then free the struct
            MyString *tempStruct = myStringAlloc();
            myStringSetFromMyString(str, tempStruct);
            myStringFree(tempStruct);
            tempStruct = NULL;
        }
        // resize the array as needed and copy the cString into it without null byte
        else
        {
            if (reSizeStringArray(str, cStringLength) == MYSTRING_ERROR)
            {
                return MYSTRING_ERROR;
            }
            // use memcpy to copy cString to str
            memcpy(str ->stringArray, cString, cStringLength);
        }
        // resize stringArray for str and return success
        str -> stringSize = cStringLength;
        return MYSTRING_SUCCESS;
    }
}

/**
 * @brief Returns the value of str as a C string, terminated with the
 * 	null character. It is the caller's responsibility to free the returned
 * 	string by calling free().
 *  Time complexity is O(n) where n is the length of str + 1. This is the cost of memcpy from str
 *  to newCString.
 * @param str the MyString
 * RETURN VALUE:
 *  @return the new string, or NULL if the allocation failed.
 */
char * myStringToCString(const MyString *str)
{
    if(str == NULL)
    {
        return NULL;
    }
    // set a long to be the size of str's string
    unsigned long size = myStringLen(str);
    // allocate new memory the size of that string + 1 for the null byte
    char *newCString = malloc(size + 1);
    // if the allocation failed, return null
    if (newCString == NULL)
    {
        return NULL;
    }
    // otherwise use memcpy to copy str to newCString and then add the nullbyte
    memcpy(newCString, str -> stringArray, size);
    *(newCString + size) = NULL_BYTE;
    return newCString;
}

/**
 * @brief Returns the value of str as an integer.
 * 	If str cannot be parsed as an integer,
 * 	the return value should be MYSTR_ERROR_CODE
 * 	NOTE: positive and negative integers should be supported.
 *  Time complexity is O(n) where n is the length of str. This is because we go over every char
 *  to calculate it's int value.
 * @param str the MyString
 * @return an integer
 */
int myStringToInt(const MyString *str)
{
    if(str == NULL || myStringLen(str) == EMPTY)
    {
        return MYSTR_ERROR_CODE;
    }
    // set our result to 0 and the sign to 1 to start and set a pointer to the array
    int result = EMPTY;
    int sign = POSITIVE;
    char *toCheck = str -> stringArray;
    // go over the array and check that it is a proper int, if it is negative change sign
    for(unsigned long i = 0; i < myStringLen(str); i++)
    {
        // if it is a number then add to our result
        if (*(toCheck + i) <= NINE && *(toCheck + i) >= ZERO)
        {
            result = BASE * result + (*(toCheck + i) - CHAR_TO_INT);
        }
        // otherwise except when we are at the first index and it is a MINUS return an error
        else
        {
            if (i == FIRST_INDEX && *toCheck == MINUS)
            {
                sign = NEGATIVE;
            }
            else
            {
                return MYSTR_ERROR_CODE;
            }
        }
    }
    // return our result times the sign to make it positive or negative
    return result * sign;
}

/**
 * @brief Sets the value of str to the value of the integer n.
 *        (i.e. if n=7 than str should contain ‘7’)
 *        You are not allowed to use itoa (or a similar functions) here but
 *        must code your own conversion function.
 *        Time complexity is O(n) where n is the amount of digits in the int. This is because
 *        the dominant action is going over the digits one by one after finding them.
 * @param str the MyString to set.
 * @param n the int to set from.
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
    // check that str is not null, if it is return an error
    if (str == NULL)
    {
        return MYSTRING_ERROR;
    }
    // find the length of the int (including it's sign)
    unsigned long stringLength = getLengthOfInt(n);
    if (n <= SMALLER_THAN_ZERO)
    {
        stringLength ++;
    }
    // if our length was not enough, reallocate memory
    reSizeStringArray(str, stringLength);
    // update the struct's size
    str -> stringSize = stringLength;
    // initialize a pointer to our array
    char *toCheck = str -> stringArray;
    // if the sign is negative then set the first char to be '-', if not by digits and update
    unsigned long i = FIRST_INDEX;
    unsigned long digitCounter = stringLength;
    if (n < 0)
    {
        *(toCheck) =  MINUS;
        i++;
        n *= NEGATIVE;
        digitCounter--;
    }
    for (; i < stringLength; i++)
    {
        long divider = (n / pow(BASE, digitCounter - 1));
        *(toCheck + i) =  floor(divider) + CHAR_TO_INT;
        n -= (pow(BASE, digitCounter - 1) * divider);
        digitCounter --;
    }
    return MYSTRING_SUCCESS;
}

/**
 * @brief filter the value of str acording to a filter.
 * 	remove from str all the occurrence of chars that are filtered by filt
 *	(i.e. filr(char)==true)
 *  Time complexity is O(n) where n is the length of str. This is the cost of memcpy of
 *  str in the case where nothing is filtered
 * @param str the MyString to filter
 * @param filt the filter
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure. */
MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
    // check for null string
    if(str == NULL)
    {
        return MYSTRING_ERROR;
    }
    // set amount found to 0 and allocate temporary array
    long amountFound = EMPTY;
    char *tempArray = malloc(myStringLen(str));
    // go over original string
    for (unsigned long i = 0; i < myStringLen(str); i++)
    {
        // each time we find a filter match raise amount found
        if (filt((str ->stringArray + i)) == true)
        {
            amountFound ++;
        }
        // each time we find they don't match then insert into temp array
        else
        {
            *(tempArray + i - amountFound) = *(str -> stringArray + i);
        }
    }
    // Set our stringSize to change
    str -> stringSize -= amountFound;
    // copy the tempArray into stringArray
    memcpy(str -> stringArray, tempArray, myStringLen(str));
    free(tempArray);
    tempArray = NULL;
    return MYSTRING_SUCCESS;
}

/**
 * @brief Compare str1 and str2.
 * Time complexity is O(min(length str1, length str2)).
 * @param str1
 * @param str2
 * RETURN VALUE:
 * @return an integral value indicating the relationship between the strings:
 * 	A zero value indicates that the strings are equal.
 * 	A value greater than zero indicates that the first character that does not 
 *  match has a greater ASCII value in str1 than in str2;
 * 	And a value less than zero indicates the opposite.
 * 	If strings cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringCompare(const MyString *str1, const MyString *str2)
{
    // returns a call to the custom compare with the default comparator we made
    return myStringCustomCompare(str1, str2, defCompare);
}

/**
 * @brief Compares str1 and str2.
 *        Time complexity is O(n) where n is min(length str1 , length str2) in worst case these are
 *        equal.
 * @param str1
 * @param str2
 * @param comparator
 * RETURN VALUE:
 * @return an integral value indicating the relationship between the strings:
 * 	A zero value indicates that the strings are equal according to the custom comparator 
 *  (3rd parameter).
 * 	A value greater than zero indicates that the first MyString is bigger according to 
 *  the comparator. And a value less than zero indicates the opposite.
 * 	If strings cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringCustomCompare(const MyString *str1, const MyString *str2,
                          int (*foo)(const char *compareChar1, const char *compareChar2))
{
    if(str1 == NULL || str2 == NULL)
    {
        return MYSTR_ERROR_CODE;
    }
    // save the sizes and find the minimum size
    long size1 = myStringLen(str1);
    long size2 = myStringLen(str2);
    long minSize = MIN(size1, size2);
    // check the equality up to that size
    int result = checkEquality(minSize, str1, str2, foo);
    // if already here we find an imbalance return it
    if(result != SAME)
    {
        return result;
    }
    // otherwise according to their sizes return proper int
    else
    {
        if(size1 == size2)
        {
            return SAME;
        }
        else if(size1 > size2)
        {
            return BIGGER;
        }
        else
        {
            return SMALLER;
        }
    }
}


/**
 * @brief Check if str1 is equal to str2.
 *        Time complexity is O(n) where n is the length of str1 and and str2 in worst case.
 *        In best case it is O(1) because if the strings are not the same length they are for sure
 *        not equal.
 * @param str1
 * @param str2
 * @param comparator
 * RETURN VALUE:
 * @return an integral value indicating the equality of the strings using a 
 *  custom comparator (3rd parameter):
 * 	A zero value indicates that the strings are not equal.
 * 	A greater than zero value indicates that the strings are equal.
 * 	If string cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringCustomEqual(const MyString *str1, const MyString *str2,
                        int (*foo)(const char *char1, const char * char2))
{
    if(str1 == NULL || str2 == NULL)
    {
        return MYSTR_ERROR_CODE;
    }
    // save their sizes. If the sizes don't match they are definitely not equal
    long size1 = myStringLen(str1);
    long size2 = myStringLen(str2);
    if(size1 != size2)
    {
        return UNEQUAL;
    }
    // otherwise use checkEquality to get whether they are equal according to foo
    else
    {
        int result = checkEquality(size1, str1, str2, foo);
        if(result == SAME)
        {
            return EQUAL;
        }
        else
        {
            return UNEQUAL;
        }
    }
}


/**
 * @brief Check if str1 is equal to str2.
 *        Time complexity is that of myStringCustomEqual which is O(n) in worst case and O(1) in
 *        best. Where n is the length of str1 and str2,
 * @param str1
 * @param str2
 * RETURN VALUE:
 * @return an integral value indicating the equality of the strings 
 *         (logical equality - that they are composed of the very same characters):
 * 	A zero value indicates that the strings are not equal.
 * 	A greater than zero value indicates that the strings are equal.
 * 	If strings cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringEqual(const MyString *str1, const MyString *str2)
{
    // uses myStringCustomEqual with a default comparator to decide whether they are equal
    return myStringCustomEqual(str1, str2, logicalEqual);
}

/**
 * @brief Getter for the total amount of memory used by a MyString.
 *        Time complexity is O(1)
 * @param the Mystring we wish to check.
 * @return the amount of memory (all the memory that used by the MyString 
 *         object itself and its allocations), in bytes, allocated to str1.
 */
unsigned long myStringMemUsage(const MyString *str1)
{
    if(str1 == NULL)
    {
        return (unsigned long) MYSTR_ERROR_CODE;
    }
    return sizeof(str1 -> stringSize) + sizeof(str1 -> realSize) +
    sizeof(str1 ->stringArray) + myStringRealSize(str1);
}

/**
 * @brief Writes the content of str to stream. (like fputs())
 *        Time complexity is that of myStringToCString which is O(n) where n is length of str.
 * @param MyString we wish to write to file.
 * @param open file stream.
 * RETURNS:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
    // allocate a new c string from the MyString
    char *newCString = myStringToCString(str);
    // if it isn't NULL and we are able to write it in then continue
    if (newCString == NULL || fputs(newCString, stream) == EOF)
    {
        return MYSTRING_ERROR;
    }
    // flush it to the stream so we can read it right away if needed
    fflush(stream);
    // free the string we allocated and set it the pointer to it to NULL
    free(newCString);
    newCString = NULL;
    return MYSTRING_SUCCESS;
}

/**
 * @brief Sets result to be the concatenation of str1 and str2.
 * 	result should be initially allocated by the caller.
 * 	result shouldn't be the same struct as str1 or str2.
 *  Time complexity is O(m + k) where m is the length of str1 and k is the length of str2.
 * @param str1
 * @param str2
 * @param result
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{
    // check whether any struct is null
    if (str1 == NULL || str2 == NULL || result == NULL)
    {
        return MYSTRING_ERROR;
    }
    // check that result is not equal to str1 or str2
    if(result == str1 || result == str2)
    {
        return MYSTRING_ERROR;
    }
    // save the total size needed for result, and reallocate as needed
    unsigned long totalSize = myStringLen(str1) + myStringLen(str2);
    if(reSizeStringArray(result, totalSize) == MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }
    result -> stringSize = totalSize;
    // copy str1 and str2 into the proper spots in result
    memcpy(result -> stringArray, str1 -> stringArray, myStringLen(str1));
    memcpy(result -> stringArray + myStringLen(str1), str2 -> stringArray, myStringLen(str2));
    return MYSTRING_SUCCESS;
}

/**
 * @brief Appends a copy of the source MyString src to the destination MyString dst.
 *        Time complexity is O(m + k) where m is the length of dest and k is the length of src.
 * @param dest the destination
 * @param src the MyString to append
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringCat(MyString * dest, const MyString * src)
{
    // allocate a new empty struct
    MyString *tempStruct = myStringAlloc();
    // copy dest and src to temp_struct using myStringCatTo
    if (myStringCatTo(dest, src, tempStruct) == MYSTRING_ERROR)
    {
        return MYSTRING_ERROR;
    }
    // set our dest to be what we got in tempStruct and free temptStruct
    myStringSetFromMyString(dest, tempStruct);
    myStringFree(tempStruct);
    return MYSTRING_SUCCESS;
}


/**
 * @brief sort an array of MyString pointers
 *        In this sort we use qSort to sort an array of MyString pointers using a comparator.
 *        However the comparator itself also uses a comparator to compare between individual chars
 *        in each MyString. Therefore the time complexity of this function is O(n*log(n)*k) where
 *        n is the length of our MyString pointer array and k is (in the worst case where all the
 *        MyString lengths are equal) the length of each MyString. This is an average run time
 *        due to the implementation of quicksort. In worst case it is O(n^2 * k), but we learned in
 *        data structures that this is unlikely.
 * @param arr
 * @param len
 * @param comparator custom comparator
 *
 * RETURN VALUE:none
 */
void myStringCustomSort(MyString **arr, int len, int (*foo)(const void* str1, const void* str2))
{
    // use the standard qsort function to sort based on the comparator received
    qsort(arr, len, sizeof(MyString *), foo);
}

/**
 * @brief sorts an array of MyString pointers according to the default 
 *        comparison (like in myStringCompare).
 *        We will sort using myStringCustomSort and using a casting function as a comparator which
 *        will deal with qsort only accepting void pointers in a comparator.
 *        Time complexity is that of myStringCustomSort.
 * @param arr
 * @param len
 *
 * RETURN VALUE: none
 */
void myStringSort(MyString **arr, int len)
{
    // call our custom sort function with the casting helper function as a comparator
    myStringCustomSort(arr, len, compareCaster);
}



#ifndef NDEBUG
static void printImproperError(const char *func, const int line)
{
    printf("Error returned improperly in %s at line %d.\n", func, line);
    printf("Expected: MYSTRING_SUCCESS");
    printf("Actual: MYSTRING_ERROR");
}
static void printComparisonHelper(const char *str, const char* func,
                                  const int expected, const int actual)
{
    printf("Wrong value for %s between two equal MyStrings in %s.\n", str, func );
    printf("Expected: %d\n", expected);
    printf("Actual: %d\n", actual);
}

static void printCalculatorHelper(const char* str, const char* func,
                                  const unsigned long expected, const unsigned long actual)
{
    printf("%s was not calculated properly in %s.\n", str, func);
    printf("Expected: %lu\n", expected);
    printf("Actual: %lu\n", actual);
}
/**
 * @brief Tester for myStringAlloc()
 *
 * RETURN VALUE: none
 */
static void testMyStringAlloc()
{
    printf("Start test for myStringAlloc\n");
    MyString *testString = myStringAlloc();
    // tests the length of an "empty" struct
    if(testString -> stringSize != EMPTY)
    {
        printCalculatorHelper(STRING_LENGTH, __func__, EMPTY, testString -> stringSize);
    }
    // tests the size of an empty "struct"
    if(testString -> realSize != 16)
    {
        printCalculatorHelper(STRING_SIZE, __func__, START_SIZE, testString -> realSize);
    }
    char testChar = testString -> stringArray[0];
    // tests for NULL_BYTE in first index of empty struct
    if (testChar != NULL_BYTE)
    {
        printf("Null byte not added to empty MyString in myStringAlloc.");
        printf("Expected: %c\n", NULL_BYTE);
        printf("Actual: %c\n", testChar);
    }
    myStringFree(testString);
    printf("End test for myStringAlloc\n");
}

/**
 * @brief Tester for myStringLen()
 *
 * RETURN VALUE: none
 */
static void testMyStringLen()
{
    printf("Start test for myStringLen\n");
    MyString *testStruct = myStringAlloc();
    // checks that length is returned properly for empty struct
    if(myStringLen(testStruct) != 0)
    {
        printCalculatorHelper(STRING_LENGTH, __func__, EMPTY, testStruct -> stringSize);
    }
    myStringFree(testStruct);
    printf("End test for myStringLen\n");
}

/**
 * @brief Tester for myStringClone()
 *
 * RETURN VALUE: none
 */
static void testMyStringClone()
{
    printf("Start test for myStringClone\n");
    MyString *testString1 = myStringAlloc();
    MyString *testString2 = myStringClone(testString1);
    // checks length of cloned struct
    if(myStringLen(testString2) != EMPTY)
    {
        printCalculatorHelper(STRING_LENGTH, __func__, EMPTY, myStringLen(testString2));

    }
    // checks length of cloned struct
    if(myStringLen(testString2) != myStringLen(testString1))
    {
        printCalculatorHelper(STRING_LENGTH, __func__,
                              myStringLen(testString1), myStringLen(testString2));
    }
    char testChar = testString2 -> stringArray[0];
    // checks struct was cloned properly
    if (testChar != NULL_BYTE)
    {
        printf("Null byte not added to empty MyString in myStringClone.\n");
        printf("Expected: %c\n", NULL_BYTE);
        printf("Actual: %c\n", testChar);
    }
    myStringFree(testString1);
    myStringFree(testString2);
    printf("End test for myStringClone\n");
}

/**
 * @brief Tester for myStringSetFromMyString()
 *
 * RETURN VALUE: none
 */
static void testMyStringSetFromMyString()
{
    printf("Start test for myStringSetFromMyString\n");
    MyString *startStruct = myStringAlloc();
    char *startString = "Testing this nown";
    myStringSetFromCString(startStruct, startString);
    MyString *destStruct = myStringAlloc();
    // checks setting struct from other struct doesn't return an error
    if(myStringSetFromMyString(destStruct, startStruct) == MYSTRING_ERROR)
    {
        printImproperError(__func__, __LINE__);
    }
    // checks length of copied struct
    if(myStringLen(destStruct) != 17)
    {
        printCalculatorHelper(STRING_LENGTH, __func__, 17, myStringLen(destStruct));
    }
    // checks size of copied struct
    if(destStruct -> realSize != 17)
    {
        printCalculatorHelper(STRING_SIZE, __func__, 17, myStringRealSize(destStruct));

    }
    char testChar = destStruct-> stringArray[START_SIZE];
    // checks struct was copied properly
    if (testChar != 'n')
    {
        printf("Improper character in destStruct in myStringSetFromMyString.\n");
        printf("Expected: %c\n", 'n');
        printf("Actual: %c\n", testChar);
    }
    myStringFree(startStruct);
    myStringFree(destStruct);
    printf("End test for myStringSetFromMyString\n");
}

/**
 * @brief Tester for myStringSetFromCString()
 *
 * RETURN VALUE: none
 */
static void testMyStringSetFromCString()
{
    printf("Start test for MyStringSetFromCString\n");
    char *testCString = "Testing this now";
    MyString *testStruct = myStringAlloc();
    // checks that setting a MyString from c string doesn't return an error
    if(myStringSetFromCString(testStruct, testCString) == MYSTRING_ERROR)
    {
        printImproperError(__func__, __LINE__);
    }
    // checks that length of setting a MyString from c string is correct
    if(myStringLen(testStruct) != START_SIZE)
    {
        printCalculatorHelper(STRING_LENGTH, __func__, START_SIZE, myStringLen(testStruct));
    }
    // checks that size of setting a MyString from c string is correct
    if(testStruct -> realSize != START_SIZE)
    {
        printCalculatorHelper(STRING_SIZE, __func__, START_SIZE, myStringRealSize(testStruct));
    }
    char testChar = testStruct-> stringArray[FIRST_INDEX];
    // check that MyString was set properly
    if (testChar != 'T')
    {
        printf("Improper character in test_struct in myStringSetFromCString.\n");
        printf("Expected: %c\n", 'T');
        printf("Actual: %c\n", testChar);
    }
    char *testEmptyCString = "";
    // check edge case of copying empty string
    if(myStringSetFromCString(testStruct, testEmptyCString) == MYSTRING_ERROR)
    {
        printImproperError(__func__, __LINE__);
    }
    if(myStringLen(testStruct) != EMPTY)
    {
        printCalculatorHelper(STRING_LENGTH, __func__, EMPTY, myStringLen(testStruct));
    }
    if(testStruct -> realSize != START_SIZE)
    {
        printCalculatorHelper(STRING_SIZE, __func__, START_SIZE, myStringRealSize(testStruct));
    }
    char testChar2 = testStruct-> stringArray[FIRST_INDEX];
    if (testChar2 != NULL_BYTE)
    {
        printf("Improper null byte for empty string in myStringSetFromCString.\n");
        printf("Expected: %c\n", NULL_BYTE);
        printf("Actual: %c\n", testChar);
    }
    myStringFree(testStruct);
    printf("End test for MyStringSetFromCString\n");
}

/**
 * @brief Tester for myStringFree()
 *
 * RETURN VALUE: none
 */
static void testMyStringFree()
{
    printf("Start test for myStringFree\n");
    /*
     There is no reliable way to tell if a pointer has been freed. The freed memory could be 
     occupied by other irrelevant data and you'll get wrong result.
     And indeed there is no standard way to check if a pointer is freed. 
     That said, glibc does have functions (mcheck, mprobe) to find the malloc status of a pointer 
     for heap consistency checking, and one of them is to see if a pointer is freed.
     However, these functions are mainly used for debugging only, and they are not thread-safe. 
     So I just made sure I have paired malloc/free and used valgrind to double check.
     */
    printf("End test for myStringFree\n");
}

/**
 * @brief Tester for myStringToCString()
 *
 * RETURN VALUE: none
 */
static void testMyStringToCString()
{
    printf("Start test for myStringToCString\n");
    char *testCString = "Testing this now";
    MyString *testStruct = myStringAlloc();
    myStringSetFromCString(testStruct, testCString);
    char *newCString = myStringToCString(testStruct);
    char testChar = newCString[START_SIZE];
    // check that string was copied with a null byte at end to make it a cstring
    if(testChar != NULL_BYTE)
    {
        printf("Improper null byte at end of c string in myStringToCString.\n");
        printf("Expected: %c\n", NULL_BYTE);
        printf("Actual: %c\n", testChar);
    }
    MyString *testStruct2 = NULL;
    char *newCString2 = myStringToCString(testStruct2);
    // check that c string was copied properly
    if(newCString2 != NULL)
    {
        printf("No null in cString2 in myStringToCString.\n");
        printf("Expected: NULL");
        printf("Actual: %s\n", newCString2);
    }
    myStringFree(testStruct);
    free(newCString);
    free(newCString2);
    printf("End test for MyStringToCString\n");
}

/**
 * @brief Tester for myStringToInt()
 *
 * RETURN VALUE: none
 */
static void testMyStringToInt()
{
    printf("Start test for myStringToInt\n");
    char *correctInt = "1234";
    char *incorrectInt = "12a4";
    char *negativeInt = "-1234";
    char *emptyString = "";
    char *complexInt = "1234567";
    char *complexNegative = "-1234567";
    char *badNegative = "12-34";
    MyString *testStruct = myStringAlloc();
    myStringSetFromCString(testStruct, correctInt);
    int testInt = myStringToInt(testStruct);
    // test that correct int was copied properly
    if(testInt != 1234)
    {
        printf("Wrong value for int for correctInt test in myStringToInt.\n");
        printf("Expected: %d\n", 1234);
        printf("Actual: %d\n", testInt);
    }
    myStringSetFromCString(testStruct, incorrectInt);
    testInt = myStringToInt(testStruct);
    // check that error code is returned for improper int
    if(testInt != MYSTR_ERROR_CODE)
    {
        printImproperError(__func__, __LINE__);
    }
    myStringSetFromCString(testStruct, negativeInt);
    testInt = myStringToInt(testStruct);
    // test that negative int was copied properly
    if(testInt != -1234)
    {
        printf("Wrong value for int for negativeInt test in myStringToInt.\n");
        printf("Expected: %d\n", -1234);
        printf("Actual: %d\n", testInt);
    }
    myStringSetFromCString(testStruct, emptyString);
    testInt = myStringToInt(testStruct);
    // check that error code is returned for improper int
    if(testInt != MYSTR_ERROR_CODE)
    {
        printImproperError(__func__, __LINE__);

    }
    myStringSetFromCString(testStruct, complexInt);
    testInt = myStringToInt(testStruct);
    // test that complex int was copied properly
    if(testInt != 1234567)
    {
        printf("Wrong value for int for complexInt test in myStringToInt.\n");
        printf("Expected: %d\n", 1234567);
        printf("Actual: %d\n", testInt);
    }
    myStringSetFromCString(testStruct, complexNegative);
    testInt = myStringToInt(testStruct);
    // test that complex negative int was copied properly
    if(testInt != -1234567)
    {
        printf("Wrong value for int for complexNegative test in myStringToInt.\n");
        printf("Expected: %d\n", -1234567);
        printf("Actual: %d\n", testInt);
    }
    myStringSetFromCString(testStruct, badNegative);
    testInt = myStringToInt(testStruct);
    // check that error code is returned for improper int
    if(testInt != MYSTR_ERROR_CODE)
    {
        printImproperError(__func__, __LINE__);
    }
    myStringFree(testStruct);
    printf("End test for myStringToInt\n");
}

/**
 * @brief Tester for myStringCustomEqual()
 *
 * RETURN VALUE: none
 */
static void testMyStringCustomEqual()
{
    printf("Start test for myStringCustomEqual\n");
    char *string1 = "Testing";
    char *string2 = "TestingWrong";
    MyString *struct1 = myStringAlloc();
    MyString *struct2 = myStringAlloc();
    MyString *struct3 = myStringAlloc();
    myStringSetFromCString(struct1, string1);
    myStringSetFromCString(struct2, string1);
    myStringSetFromCString(struct3, string2);
    // check that equal recognizes that MyStrings are equal
    if (myStringCustomEqual(struct1, struct2, logicalEqual) == UNEQUAL)
    {
        printComparisonHelper("equality test", __func__, EQUAL,
                              UNEQUAL);
    }
    // check that equal recognizes that MyStrings are unequal
    if (myStringCustomEqual(struct1, struct3, logicalEqual) == EQUAL)
    {
        printComparisonHelper("equality test", __func__, UNEQUAL ,
                              EQUAL);
    }
    myStringFree(struct1);
    myStringFree(struct2);
    myStringFree(struct3);
    printf("End test for myStringCustomEqual\n");
}

/**
 * @brief Tester for myStringEqual()
 *
 * RETURN VALUE: none
 */
static void testMyStringEqual()
{
    printf("Start test for myStringEqual\n");
    char *equalString = "equal string";
    char *unequalString = "unequal string";
    MyString *struct1 = myStringAlloc();
    MyString *struct2 = myStringAlloc();
    myStringSetFromCString(struct1, equalString);
    myStringSetFromCString(struct2, equalString);
    // check that equal recognizes that MyStrings are equal
    if (myStringEqual(struct1, struct2) == UNEQUAL)
    {
        printComparisonHelper("equality test", __func__, EQUAL,
                              UNEQUAL);
    }
    myStringSetFromCString(struct2, unequalString);
    // check that equal recognizes that MyStrings are unequal
    if (myStringEqual(struct1, struct2) == EQUAL)
    {
        printComparisonHelper("equality test", __func__, UNEQUAL,
                              EQUAL);
    }
    myStringFree(struct1);
    myStringFree(struct2);
    printf("End test for myStringCustomEqual\n");
}


/**
 * @brief Tester for myStringSetFromInt()
 *
 * RETURN VALUE: none
 */
static void testMyStringSetFromInt()
{
    printf("Start test for myStringSetFromInt\n");

    char *correctIntString = "1234";
    char *negativeIntString = "-1234";
    char *complexIntString = "1234567";
    char *complexNegativeString = "-1234567";
    int correctInt = 1234;
    int negativeInt = -1234;
    int complexInt = 1234567;
    int complexNegative = -1234567;
    MyString *testStructInt = myStringAlloc();
    MyString *testStructString = myStringAlloc();
    myStringSetFromInt(testStructInt, correctInt);
    myStringSetFromCString(testStructString, correctIntString);
    // test that int was copied properly into MyString
    if(myStringEqual(testStructInt, testStructString) == UNEQUAL)
    {
        printComparisonHelper("int", __func__, EQUAL, UNEQUAL);
    }
    myStringSetFromInt(testStructInt, negativeInt);
    myStringSetFromCString(testStructString, negativeIntString);
    // test that negative int was copied properly into MyString
    if(myStringEqual(testStructInt, testStructString) == UNEQUAL)
    {
        printComparisonHelper("negative int", __func__, EQUAL, UNEQUAL);

    }
    myStringSetFromInt(testStructInt, complexInt);
    myStringSetFromCString(testStructString, complexIntString);
    // test that complex int was copied properly into MyString
    if(myStringEqual(testStructInt, testStructString) == UNEQUAL)
    {
        printComparisonHelper("complex int", __func__, EQUAL, UNEQUAL);

    }
    myStringSetFromInt(testStructInt, complexNegative);
    myStringSetFromCString(testStructString, complexNegativeString);
    // test that complex negative int was copied properly into MyString
    if(myStringEqual(testStructInt, testStructString) == UNEQUAL)
    {
        printComparisonHelper("complex negative int", __func__, EQUAL, UNEQUAL);

    }
    myStringFree(testStructInt);
    myStringFree(testStructString);
    printf("End test for myStringSetFromInt\n");
}

/**
 * @brief Helper for testMyStringFilter()
 *        Simple filter for test purposes
 * @param char we wish to check
 * RETURN VALUE:
 * @return true if char is equal to a, false otherwise
 */
static bool testMyStringFilterHelper(const char *char1)
{
    if(*char1 == 'a')
    {
        return true;
    }
    return false;
}
/**
 * @brief Tester for myStringFilter()
 *
 * RETURN VALUE: none
 */
static void testMyStringFilter()
{
    printf("Start test for myStringSetFilter\n");
    char *testString = "abacadae";
    char *testString2 = "bcde";
    MyString *testStruct = myStringAlloc();
    MyString *testStruct2 = myStringAlloc();
    myStringSetFromCString(testStruct, testString);
    myStringSetFromCString(testStruct2, testString2);
    // test that filter does not return error
    if (myStringFilter(testStruct, testMyStringFilterHelper) == MYSTRING_ERROR)
    {
        printImproperError(__func__, __LINE__);
    }
    // test that two strings are equal after filter
    if(myStringEqual(testStruct2, testStruct) == UNEQUAL)
    {
        printComparisonHelper("equality test", __func__, EQUAL, UNEQUAL);
    }
    char *testString3 = "";
    myStringSetFromCString(testStruct, testString3);
    // test that filter does not return error
    if (myStringFilter(testStruct, testMyStringFilterHelper) == MYSTRING_ERROR)
    {
        printImproperError(__func__, __LINE__);
    }
    myStringFree(testStruct);
    myStringFree(testStruct2);
    printf("End test for myStringSetFilter\n");
}

/**
 * @brief Tester for myStringCompare()
 *
 * RETURN VALUE: none
 */
static void testMyStringCompare()
{
    
    printf("Start test for myStringCompare\n");
    char *string1 = "bcfd";
    char *string2 = "bcde";
    char *string3 = "bcd";
    char *string4 = "cd";
    MyString *equalStruct1 = myStringAlloc();
    myStringSetFromCString(equalStruct1, string1);
    MyString *equalStruct2 = myStringClone(equalStruct1);
    MyString *unequalStruct = myStringAlloc();
    myStringSetFromCString(unequalStruct, string2);
    MyString *shorterStruct = myStringAlloc();
    myStringSetFromCString(shorterStruct, string3);
    MyString *shortestStruct = myStringAlloc();
    myStringSetFromCString(shortestStruct, string4);
    // Checks that two equal structs are compared properly
    if(myStringCompare(equalStruct1, equalStruct2) == BIGGER ||
       myStringCompare(equalStruct1, equalStruct2) == SMALLER )
    {
        printComparisonHelper("comparison test", __func__, SAME,
                              myStringCompare(equalStruct1, equalStruct2) == BIGGER);
    }
    // Checks that two unequal structs are compared properly
    if(myStringCompare(equalStruct1, unequalStruct) <= SAME)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: -1\n");
        printf("Actual: %d\n", myStringCompare(equalStruct1, unequalStruct));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCompare(unequalStruct, equalStruct1) >= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: 1\n");
        printf("Actual: %d\n", myStringCompare(unequalStruct, equalStruct1));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCompare(equalStruct1, shorterStruct) <= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: 1\n");
        printf("Actual: %d\n", myStringCompare(equalStruct1, shorterStruct));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCompare(shorterStruct, shortestStruct) >= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: -1\n");
        printf("Actual: %d\n", myStringCompare(shorterStruct, shortestStruct));
    }
    
    myStringFree(equalStruct1);
    myStringFree(equalStruct2);
    myStringFree(unequalStruct);
    myStringFree(shorterStruct);
    myStringFree(shortestStruct);
    printf("End test for myStringCompare\n");
}

/**
 * @brief Tester for myStringCustomCompare()
 *
 * RETURN VALUE: none
 */
static void testMyStringCustomCompare()
{
    printf("Start test for myStringCustomCompare\n");
    char *string1 = "abcd";
    char *string2 = "abce";
    char *string3 = "abc";
    char *string4 = "ad";
    MyString *equalStruct1 = myStringAlloc();
    myStringSetFromCString(equalStruct1, string1);
    MyString *equalStruct2 = myStringClone(equalStruct1);
    MyString *unequalStruct = myStringAlloc();
    myStringSetFromCString(unequalStruct, string2);
    MyString *shorterStruct = myStringAlloc();
    myStringSetFromCString(shorterStruct, string3);
    MyString *shortestStruct = myStringAlloc();
    myStringSetFromCString(shortestStruct, string4);
    // Checks that two equal structs are compared properly
    if(myStringCustomCompare(equalStruct1, equalStruct2, defCompare) !=0)
    {
        printf("Wrong value returned after comparison of 2 equal MyStrings.\n");
        printf("Expected: 0\n");
        printf("Actual: %d\n", myStringCustomCompare(equalStruct1, equalStruct2, defCompare));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCustomCompare(equalStruct1, unequalStruct, defCompare) >= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: -1\n");
        printf("Actual: %d\n", myStringCustomCompare(equalStruct1, unequalStruct, defCompare));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCustomCompare(unequalStruct, equalStruct1, defCompare) <= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: 1\n");
        printf("Actual: %d\n", myStringCustomCompare(unequalStruct, equalStruct1, defCompare));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCustomCompare(equalStruct1, shorterStruct, defCompare) <= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: 1\n");
        printf("Actual: %d\n", myStringCustomCompare(equalStruct1, shorterStruct, defCompare));
    }
    // Checks that two unequal structs are compared properly
    if(myStringCustomCompare(shorterStruct, shortestStruct, defCompare) >= 0)
    {
        printf("Wrong value returned after comparison of 2 unequal MyStrings.\n");
        printf("Expected: -1\n");
        printf("Actual: %d\n", myStringCustomCompare(shorterStruct, shortestStruct, defCompare));
    }
    
    myStringFree(equalStruct1);
    myStringFree(equalStruct2);
    myStringFree(unequalStruct);
    myStringFree(shorterStruct);
    myStringFree(shortestStruct);
    printf("End test for myStringCustomCompare\n");
}

/**
 * @brief Tester for myStringCatTo()
 *
 * RETURN VALUE: none
 */
static void testMyStringCatTo()
{
    printf("Start test for myStringCatTo\n");
    char *string1 = "first";
    char *string2 = "morecharacters";
    char *string3 = "firstmorecharacters";
    MyString *str1 = myStringAlloc();
    MyString *str2 = myStringAlloc();
    MyString *str3 = myStringAlloc();
    myStringSetFromCString(str1, string1);
    myStringSetFromCString(str2, string2);
    myStringCatTo(str1, str2, str3);
    // checks that the length matches after catTo
    if(myStringLen(str3) != myStringLen(str1) + myStringLen(str2))
    {
        printCalculatorHelper(STRING_LENGTH, __func__, 19, myStringLen(str3));
    }
    myStringSetFromCString(str1, string3);
    // Checks that two equal structs are compared properly
    if(myStringEqual(str3, str1) == 0)
    {
        printf("String does not match expected after myStringCatTo\n");
        printf("Expected: firstmorecharacters\n");
        printf("Actual: %s\n", str3 -> stringArray);
    }
    myStringFree(str1);
    myStringFree(str2);
    myStringFree(str3);
    printf("End test for myStringCatTo\n");
}

/**
 * @brief Tester for myStringCat()
 *
 * RETURN VALUE: none
 */
static void testMyStringCat()
{
    printf("Start test for myStringCat\n");
    char *string1 = "first";
    char *string2 = "morecharacters";
    char *string3 = "firstmorecharacters";
    char *string4 = "firstmorecharactersfirstmorecharacters";
    MyString *str1 = myStringAlloc();
    MyString *str2 = myStringAlloc();
    myStringSetFromCString(str1, string1);
    myStringSetFromCString(str2, string2);
    myStringCat(str1, str2);
    myStringSetFromCString(str2, string3);
    char *cString = myStringToCString(str1);
    // Checks that length of structs is the same
    if(myStringLen(str1) != myStringLen(str2))
    {
        printCalculatorHelper(STRING_LENGTH, __func__, 19, myStringLen(str1));
    }
    // Checks that two equal structs are compared properly
    if(myStringEqual(str1, str2) == 0)
    {
        printf("String does not match expected after myStringCat\n");
        printf("Expected: firstmorecharacters\n");
        printf("Actual: %s\n", cString);
    }
    myStringCat(str1, str1);
    myStringSetFromCString(str2, string4);
    free(cString);
    char *cString2 = myStringToCString(str1);
    // Checks that two equal structs are compared properly
    if(myStringEqual(str1, str2) == 0)
    {
        printf("String does not match expected after myStringCat with itself\n");
        printf("Expected: firstmorecharactersfirstmorecharacters\n");
        printf("Actual: %s\n", cString2);
    }
    myStringFree(str1);
    myStringFree(str2);
    free(cString2);
    printf("End test for myStringCat\n");
}


/**
 * @brief Tester for myStringMemUsage()
 *
 * RETURN VALUE: none
 */
static void testMyStringMemUsage()
{
    printf("Start test for myStringMemUsage\n");
    MyString *str1 = myStringAlloc();
    unsigned long memory = myStringMemUsage(str1);
    unsigned long expectedSize = (sizeof(unsigned long) * 2) + sizeof(char) * 16 + 8;
    if(memory != expectedSize)
    {
        printf("Memory does not match that expected for empty struct in myStringMemUsage\n");
        printf("Expected: %lu\n", expectedSize);
        printf("Actual: %lu\n", memory);
    }
    
    myStringFree(str1);
    printf("End test for myStringMemUsage\n");
}

/**
 * @brief Tester for myStringSort()
 *
 * RETURN VALUE: none
 */
static void testMyStringSort()
{
    printf("Start test for myStringSort\n");
    char *string1 = "Monkey";
    char *string2 = "Bear";
    char *string3 = "Giraffe";
    char *string4 = "Mongoose";
    char *string5 = "Sloth";
    MyString *str1 = myStringAlloc();
    MyString *str2 = myStringAlloc();
    MyString *str3 = myStringAlloc();
    MyString *str4 = myStringAlloc();
    MyString *str5 = myStringAlloc();
    myStringSetFromCString(str1, string1);
    myStringSetFromCString(str2, string2);
    myStringSetFromCString(str3, string3);
    myStringSetFromCString(str4, string4);
    myStringSetFromCString(str5, string5);
    MyString* array[] = {str1, str2, str3, str4, str5};
    myStringSort(array, 5);
    // check that the array is properly sorted
    for(int i = 0; i < 4; i++)
    {
        if(myStringCompare(array[i], array[i + 1]) != -1)
        {
            printf("Array was not sorted properly in testMyStringSort\n");
            printf("Expected: -1\n");
            printf("Actual: %d\n", myStringCompare(array[i], array[i + 1]));
        }
    }
    myStringFree(str1);
    myStringFree(str2);
    myStringFree(str3);
    myStringFree(str4);
    myStringFree(str5);
    printf("End test for myStringSort\n");
}

/**
 * @brief Tester for myStringCustomSort()
 *
 * RETURN VALUE: none
 */
static void testMyStringCustomSort()
{
    printf("Start test for myStringCustomSort\n");
    
    char *string1 = "bird";
    char *string2 = "mouse";
    char *string3 = "cat";
    char *string4 = "Dog";
    char *string5 = "elephant";
    MyString *str1 = myStringAlloc();
    MyString *str2 = myStringAlloc();
    MyString *str3 = myStringAlloc();
    MyString *str4 = myStringAlloc();
    MyString *str5 = myStringAlloc();
    myStringSetFromCString(str1, string1);
    myStringSetFromCString(str2, string2);
    myStringSetFromCString(str3, string3);
    myStringSetFromCString(str4, string4);
    myStringSetFromCString(str5, string5);
    MyString* array[] = {str1, str2, str3, str4, str5};
    myStringCustomSort(array, 5, compareCaster);
    // checks that the array is properly sorted
    for(int i = 0; i < 4; i++)
    {
        if(myStringCompare(array[i], array[i + 1]) != -1)
        {
            printf("Array was not sorted properly in testMyStringCustomSort\n");
            printf("Expected: -1\n");
            printf("Actual: %d\n", myStringCompare(array[i], array[i + 1]));
        }
    }
    myStringFree(str1);
    myStringFree(str2);
    myStringFree(str3);
    myStringFree(str4);
    myStringFree(str5);

    
    printf("End test for myStringCustomSort\n");
}

/**
 * @brief Tester for myStringWrite()
 *
 * RETURN VALUE: none
 */
static void testMyStringWrite()
{
    printf("Start test for myStringWrite\n");
    FILE* newFile = fopen("newFile", "w");
    char *stringWrite = "Write this to file";
    MyString *str1 = myStringAlloc();
    myStringSetFromCString(str1, stringWrite);
    myStringWrite(str1, newFile);
    char buffer[19];
    fclose(newFile);
    newFile = fopen("newFile", "r");
    fgets(buffer, (int) myStringLen(str1) + 1, newFile);
    MyString *str2 = myStringAlloc();
    myStringSetFromCString(str2, buffer);
    if(myStringEqual(str1, str2) == 0)
    {
        printf("Array was not sorted properly in testMyStringWrite\n");
        printf("Expected: 1\n");
        printf("Actual: 0\n");
    }
    fclose(newFile);
    myStringFree(str1);
    myStringFree(str2);
    printf("End test for myStringWrite\n");
}
#endif

/**
 * @brief Calls our tests if in NDEBUG mode
 * RETURN VALUE:
 * @int 0 when program is done
 */
#ifndef NDEBUG
int main()
{

    testMyStringAlloc();
    testMyStringLen();
    testMyStringClone();
    testMyStringSetFromMyString();
    testMyStringSetFromCString();
    testMyStringToCString();
    testMyStringToInt();
    testMyStringCustomEqual();
    testMyStringEqual();
    testMyStringSetFromInt();
    testMyStringFilter();
    testMyStringCustomCompare();
    testMyStringCompare();
    testMyStringCatTo();
    testMyStringCat();
    testMyStringMemUsage();
    testMyStringSort();
    testMyStringCustomSort();
    testMyStringWrite();
    testMyStringFree();
    return 0;
}
#endif



