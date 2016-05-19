/**
 * @file LineSeparator.h
 * @author  Dan Kufra
 * @version 1.0
 * @date 04 August 2015
 *
 * @brief Header for a program reads from a given file and creates a linear separator from the test 
 *  data in the file.
 *
 * @section DESCRIPTION
 *
 * Program reads from a text file. First line is the dimension of our vectors, second line is
 * is the amount of vectors in our test data.
 * Based on those vectors we create a linear separator. Then we test the other vectors and
 * classify them according to that separators.
 */

#ifndef __Ex2__LineSeparator__
#define __Ex2__LineSeparator__

#include <stdio.h>

/*
 * @def MAX_LINE
 * @brief Int representing the maximum characters in a line in our file.
 */
#define MAX_LINE 150
/*
 * @def ARG_COUNT
 * @brief Int representing the expected argument count our program receives.
 */
#define ARG_COUNT 2
/*
 * @def MIN_DIMENSION
 * @brief Int representing the minimum dimension of our vectors.
 */
#define MIN_DIMENSION 1
/*
 * @def FILE_NAME_INDEX
 * @brief Int representing index of the file name in our args.
 */
#define FILE_NAME_INDEX 1
/*
 * @def POSITIVE_CLASSIFIER
 * @brief Int representing a positive classifier of a vector in relation to the separator.
 */
#define POSITIVE_CLASSIFIER 1
/*
 * @def Negative_CLASSIFIER
 * @brief Int representing a negative classifier of a vector in relation to the separator.
 */
#define NEGATIVE_CLASSIFIER -1
/*
 * @def ERROR
 * @brief Int that represents an error to be returned.
 */
#define ERROR -1


/**
 * @brief Calls the functions necessary to create the separators and print the classifiers. Also
 *        handles the reading from the file.
 * @param File *fileStream: an open File we would like to read from
 */
void makePerceptron(FILE *fileStream);

/**
 * @brief A struture representing a vector and its classifier. Holds an array of double
 *        for the vector and a double as a classifier.
 */
struct Vector;
/**
 * @def Defines the name of struct Vector as Vector.
 */
typedef struct Vector Vector;

#endif /* defined(__Ex2__LineSeparator__) */
