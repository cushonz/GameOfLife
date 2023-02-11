# Project Title: Matrix Randomizer

A multi-threaded program that randomizes a matrix in C++. The program takes in an input n, which represents the dimension of the square matrix. It then generates a random matrix with values of either 0 or 1, and uses multithreading to check the value of one randomly selected location in the matrix. If the sum of the number of 1s surrounding the location is greater than the sum of the number of 0s, the value of the selected location becomes 1. If the sum of the number of 0s is greater, the value of the selected location becomes 0. This process continues until either all the values in the matrix are 1, or all the values are 0.

## Codebase Overview

The codebase includes the following files:

- `main.cpp`: Main file for the program

## Building and Running the Program

The program was written in C++ and can be built and run on any platform with a C++ compiler installed. To build the program, use the following command in the terminal:

To run the program, use the following command:

    g++ -pthread main.cpp -o matrix

    ./matrix

## Dependencies

This program requires the following dependencies to run:

- pthread library

## Key Features

- The program uses multithreading to increase the efficiency of randomizing the matrix.
- The program implements a helper function, `evaluation`, to determine if the random location in the matrix is 1 or 0.
- The program implements a helper function, `adjust`, to count the number of 1s and 0s in the surrounding area of the selected location in the matrix.
- The program implements a helper function, `printMatrix`, to print the matrix in a clean and readable format.

## Program Functionality

1. The program takes in an input n from the user, which represents the dimension of the square matrix.
2. The program generates a random matrix of size n x n with values of either 0 or 1.
3. The program creates a supervisor thread to continuously check if all the values in the matrix are either 0 or 1.
4. The program creates multiple worker threads to randomly select a location in the matrix and adjust its value based on the number of 1s and 0s in the surrounding area.
5. The program continues the process of adjusting the values until all the values in the matrix are either 0 or 1.

## Program Output

The program outputs the matrix after each adjustment made by the worker threads. The program also outputs the position of the selected location in the matrix before and after each adjustment.

## Conclusion

This program serves as a demonstration of multithreading and matrix randomization in C++. It can serve as a starting point for further developments and improvements in these areas.
