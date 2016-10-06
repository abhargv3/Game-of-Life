#include <ncursesw/curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game_of_life.h"

int main(int argc, const char *argv[])
{ // Time argument 
  int run_time = -1;
  char in_path[40] = "tests/";
  char out_path[40] = "output/";

  // Check input
  if(argc == 2)
  {
    run_time = -1;
  }
  else if (argc == 3)
  {
    run_time = atoi(argv[2]);
  }
  else
  {
    return 0;
  }

  // Open input file
  strcat(in_path, argv[1]);
  FILE *in_file = fopen(in_path, "r");
  if (in_file == NULL)
  {
    fprintf(stderr, "Can't open input file.\n");
    return 0;
  }

  // Read data from file
  int **arr_data;
  int rows = 0, cols = 0;
  read_data(in_file, &arr_data, &rows, &cols);

  // Close file
  fclose(in_file);
  
  // Run game infintely
  if (run_time == -1)
  {
    while (true)
    {
      // Initialize stdscr
      setlocale(LC_ALL, "");
      initscr();
      curs_set(0);
      start_color();

      // Handle quitting
      timeout(-1);
      noecho();
      nodelay(stdscr, true);

      // Printing board
      print_board(arr_data, rows, cols);

      // Update the array and terminal
      update_array2d(arr_data, rows, cols);

      //Check for user input
      int ch = getch();
      if (ch == 'q')
      {
        // Terminate stdscr
        endwin();

        break;
      }

      // Delay for 100 ms
      int milli = 100;
      usleep(milli * 1000);
    }
  }
  else if (run_time > 0)
  {
    // Initialize output file
    strcat(out_path, argv[1]);
    FILE *out_file = fopen(out_path, "w");

    // Print initial array
    fprint_array2d(out_file, arr_data, rows, cols);
    fprintf(out_file, "\n");
    if (out_file == NULL)
    {
      fprintf(stderr, "Can't open output file.\n");
      return 0;
    }

    for (int i = 0; i < run_time; i++)
    {
      update_array2d(arr_data, rows, cols);
    }

    // Print updated array
    fprint_array2d(out_file, arr_data, rows, cols);

    //Close file
    fclose(out_file);
  }

  // Free memory
  free_array2d(arr_data, rows);

  return 0;
}
