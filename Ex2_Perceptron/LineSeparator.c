/**
 * @file LineSeparator.c
 * @author  Dan Kufra
 * @version 1.0
 * @date 04 August 2015
 *
 * @brief Program reads from a given file and creates a linear separator from the test data in the
 *  file.
 *
 * @section DESCRIPTION
 *
 * Program reads from a text file. First line is the dimension of our vectors, second line is
 * is the amount of vectors in our test data.
 * Based on those vectors we create a linear separator. Then we test the other vectors and
 * classify them according to that separators.
 */

// ------------------------------ includes ------------------------------
#include "LineSeparator.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// -------------------------- constant definitions -------------------------
/*
 * @def MAX_DOUBLE_ARRAY_SIZE
 * @brief This is the maximum size an array of doubles can hold
 */
#define MAX_DOUBLE_ARRAY_SIZE 74
/*
 * @def EPSILON
 * @brief This is the accuracy around 0 that is demanded for the dot products
 */
#define EPSILON 0.00001
/*
 * @def DELIMITER
 * @brief A comma that separates between doubles
 */
#define DELIMITER ","
/*
 * @def START_VALUE
 * @brief Value that dot products and the separator starts with
 */
#define START_VALUE 0
/*
 * @def BAD_ARG_COUNT
 * @brief String that is printed when wrong amount of args are given
 */
#define BAD_ARG_COUNT "Wrong amount of arguments given."
/*
 * @def BAD_FILE
 * @brief String that is printed when bad file is given
 */
#define BAD_FILE "File cannot be opened properly."
/*
 * @def EOF_ERROR
 * @brief String that is printed when a file is ended badly.
 */
#define EOF_ERROR "ERROR: EOF not reached at end of file."


// ------------------------------ functions -----------------------------

/**
 * @brief A struture representing a vector and its classifier. Holds an array of double
 *        for the vector and a double as a classifier.
 */
struct Vector
{
    double vector[MAX_DOUBLE_ARRAY_SIZE];
    double classifier;
    
};

/**
 * @brief updates our Vector struct's paramaters based on the line and dimension it is given.
 * @param char *line: a line from a file that holds a vector and maybe it's classifier.
 *        int dimension: the dimension of our vectors.
 *        Vector *tempVector: a pointer to the struct we are updating.
 */
static void updateVector(char *line, const int dimension, Vector *tempVector)
{
    // declare a char * that will hold the character we separated
    char *point = NULL;
    // go over each point and set our struct's vector to be those points
    
    for (int i = 0; i < dimension; i++)
    {
        if (i == START_VALUE)
        {
            point = strtok(line, DELIMITER);
        }
        else
        {
            point = strtok(NULL, DELIMITER);
        }
        double converted_point = atof(point);
        tempVector -> vector[i] = converted_point;
    }
    // split once more, if it is NULL it means it is not test data, else update classifier
    point = strtok(NULL, DELIMITER);
    if(point != NULL)
    {
        double classifier = atof(point);
        tempVector -> classifier = classifier;
    }
}


/**
 * @brief Calculates the dot product between our separator and a given vector.
 * @param double *separator: A pointer to our separator vector.
 *        double *coordinate: A pointer to the vector we will multiply it by.
 *        int dimension: the dimension of our vectors.
 * @return int either 1 or -1 depending on the dot product.
 */
static int dotProduct(const double *separator, const double *coordinate, const int dimension)
{
    // initialize our dot_total to 0, and calculate it
    double dot_total = START_VALUE;
    for (int i = 0; i < dimension; i++)
    {
        dot_total += separator[i] * coordinate[i];
    }
    // return the proper sign of the dot product
    if (dot_total > EPSILON)
    {
        return POSITIVE_CLASSIFIER;
    }
    return NEGATIVE_CLASSIFIER;
}


/**
 * @brief updates our separator based on the line and dimension it is given.
 * @param char *line: a line from a file that holds a vector and maybe it's classifier.
 *        double *separator: a pointer to the separator
 *        int dimension: the dimension of our vectors.
 */
static void updateSeparator(char *line, double *separator, const int dimension)
{
    //create a temporary struct that will be released from stack after calculations
    Vector cur_point;
    // update the vector and/or classifier
    updateVector(line, dimension, &cur_point);
    // calculate dot product between this vector and our separator
    int product = dotProduct(separator, cur_point.vector, dimension);
    // if the signs are different then update the classifier
    if (product != cur_point.classifier)
    {
        for (int i = 0; i < dimension; i++)
        {
            separator[i] += cur_point.classifier * cur_point.vector[i];
        }
    }
    
}


/**
 * @brief Calls the functions necessary to create the separators and print the classifiers. Also
 *        handles the reading from the file.
 * @param File *fileStream: an open File we would like to read from
 */
void makePerceptron(FILE *fileStream)
{
    
    // create a buffer, read the first two lines, and convert them to proper ints using atoi
    char buffer[MAX_LINE];
    fgets(buffer, MAX_LINE, fileStream);
    int dimension = atoi(buffer);

    assert(dimension > MIN_DIMENSION);
    fgets(buffer, MAX_LINE, fileStream);
    int num_test = atoi(buffer);
    
    // initialize our separator vector to be 0
    double separator[MAX_DOUBLE_ARRAY_SIZE];
    for (int i = 0; i < dimension; i++)
    {
        separator[i] = START_VALUE;
    }
    // go over the test data and update our separator
    for (int i = 0; i < num_test; i++)
    {
        char line[MAX_LINE];
        fgets(line, MAX_LINE, fileStream);
        updateSeparator(line, separator, dimension);
    }
    while(fgets(buffer, MAX_LINE, fileStream) != NULL)
    {
        Vector temp;
        updateVector(buffer, dimension, &temp);
        printf("%d\n", dotProduct(separator, temp.vector, dimension));
    }
    if(!feof(fileStream))
    {
        printf("%s\n", EOF_ERROR);
    }
}


/**
 * @brief main function which receives the file as an argument and opens the file. Then calls
 *        makePerceptron with that file.
 * @param int argc: number of arguments passed
 *        char *argv[]: name of the file we would like to open.
 * @return int -1 if an error occurred, 0 if all went well.
 */
int main(const int argc, const char *argv[])
{
    // Check whether the argument count is correct
    if (argc != ARG_COUNT)
    {
        printf("%s\n", BAD_ARG_COUNT);
        return ERROR;
    }
    // open the file and check whether it is a valid file
    FILE *data_file = NULL;
    data_file = fopen(argv[FILE_NAME_INDEX], "r");
    if(data_file == NULL)
    {
        printf("%s\n", BAD_FILE);
        return ERROR;
    }
    // call makePerceptron on the file
    makePerceptron(data_file);
    fclose(data_file);
    return 0;
}
































