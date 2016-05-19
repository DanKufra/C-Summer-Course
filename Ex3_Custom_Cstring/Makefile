CFLAGS= -Wextra -Wall -Wvla -g -lm
CC = c99

#Compiles the test seperately.
compiledTests: MyString.c MyString.h
	$(CC) $(CFLAGS) MyString.c -o compiledTests

#Compiles the tests if necessary, otherwise just runs the executable.
tests: compiledTests
	compiledTests

#Compiles myStringMain if necessary, otherwise just runs the executable.
main: myStringMain
	myStringMain

#Compiles myStringMain
myStringMain: MyStringMain.c libmyString.a
	$(CC) $(CFLAGS) -static -DNDEBUG -c MyStringMain.c
	$(CC) $(CFLAGS) MyStringMain.o -L. -lmyString -o myStringMain

#Creates the libmyString (static library)
myString: MyString.c MyString.h
	$(CC) $(CFLAGS) -DNDEBUG -c MyString.c
	ar rcs libmyString.a MyString.o

clean:
	rm -f libmyString.a
	rm -f compiledTests
	rm -f MyStringMain

.PHONY: main tests
