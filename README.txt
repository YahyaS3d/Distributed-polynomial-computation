Distributed polynomial computation
Authored by Yahya Saad  :) 

==Description==
this program performs operations on polynomials. The programs are designed to work together and demonstrate interprocess communication using shared memory and semaphores
the second part of the program support also threads and the usage of semaphores together 
consumer producer problem: The consumer-producer problem is a classical synchronization problem where one or more producers produce data or items, and one or more consumers consume or process the items. this project demonstrate the basic structure and synchronization mechanisms required to solve this problem in the context of polynomial operations.


==Program Files==
1. Ex3q1.c: This file contains a program that reads user input, parses polynomial expressions, and performs addition, subtraction, or multiplication operations on the polynomials. The results are printed to the console.

2. Ex3q2a.c: This file contains a program that acts as a writer process. It reads user input, performs polynomial operations, and writes the results to a shared buffer using shared memory. It uses a semaphore for synchronization to ensure safe access to the shared buffer.

3. Ex3q2b.c: This file contains a program that acts as a reader process. It accesses the shared buffer, retrieves polynomial results, and calculates the polynomial value for a specific input. It uses the shared memory and semaphore created by the writer process.

4. Ex3q3a.c: This file contains an updated version of the writer process from `Ex3q2a.c`. It includes additional error handling and input validation.

5. Ex3q3b.c: This file contains an updated version of the reader process from `Ex3q2b.c`. It includes additional error handling and output formatting.

README.txt - description for the program 

makefile : to build and compile the programs 

==How to compile?==
## Usage

To compile and run the programs, follow these steps:
 To use this Makefile, save it in the same directory as the program files with the corresponding names (Ex3q1.c, Ex3q2a.c, Ex3q2b.c, Ex3q3a.c, Ex3q3b.c). Then open a terminal, navigate to the directory containing the Makefile, and run the following commands:
 make 

 This will compile all the programs and generate the executable files. To run a specific program, you can use the following commands:
 For Exercise 3 Question 1:
 ./ex3q1
For Exercise 3 Question 2 (Writer):

./ex3q2a
For Exercise 3 Question 2 (Reader):

./ex3q2b
For Exercise 3 Question 3 (Writer):

./ex3q3a
For Exercise 3 Question 3 (Reader):

./ex3q3b
Make sure to run the reader program (ex3q2b and ex3q3b) after running the corresponding writer program (ex3q2a and ex3q3a) to ensure proper communication through shared memory.

==requirements== 
Note: The Makefile assumes that you are using Linux or a similar environment that supports POSIX threads and shared memory. The flags -lrt and -lpthread are used to link the necessary libraries for these features.


==Input:==
2 polynomials and 1 operation in this formate 
(degree: coefficient) operation (degree: coefficient)
the operation can be ADD/MUL/SUB 
example input: 
(2:3,2,1) ADD (3:2,4,0,-1)
the program will simulate the operation (add) to polynomial 2x^3 + 4x^2 -1 with 3x^2 +2x^1 +1 

==Limitations==
The programs have the following limitations:

The maximum degree of the polynomials is 10 (MAX_DEGREE).
The maximum length of the input string is 128 characters (MAX_INPUT_LENGTH).
The shared buffer size is limited to 10 polynomial results (BUFFER_SIZE).
The programs process a fixed number of results from the buffer (5 in this case), as specified by the totalCount variable in Ex3q2b.c.

==Output:==
The programs output the results of the polynomial operations and the calculated polynomial values.
for the previous example (2:3,2,1) ADD (3:2,4,0,-1)
the output is 2x^3 + 7x^2 +2x the result fit the operation between 2 polynomials 
the second part of the program - 
Program Ex3q2a (writer) prints the result of each polynomial operation and writes the results to the shared buffer.
Program Ex3q2b (reader) retrieves the polynomial results from the shared buffer, prints the results, and calculates the polynomial value for a specific input.
