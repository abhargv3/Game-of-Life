#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <stdio.h>

#define _XOPEN_SOURCE_EXTENDED 1

int mod(int a, int b);

void free_array2d(int **arr, int rows);
void fprint_array2d(FILE *stream, int **arr, int rows, int cols);
int **malloc_array2d(int rows, int cols);
void update_array2d(int **arr, int rows, int cols);

void read_data(FILE *file, int ***arr, int *rows, int *cols);

void print_board(int **arr, int rows, int cols);

#endif /* GAMEOFLIFE_H */
