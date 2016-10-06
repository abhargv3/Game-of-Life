#include <ncursesw/curses.h>
#include <stdlib.h>
#include <wchar.h>
#include "game_of_life.h"

enum status
{
  RESPAWN = -1,
  DEAD,
  ALIVE, 
  KILL
};

/*
 * mod
 *   DESCRIPTION: computes a mod b
 *   INPUTS: a -- value to be modded
 *           b -- modding value
 *   OUTPUTS: none
 *   RETURN VALUE: a mod b
 *   SIDE EFFECTS: none
 */
int mod(int a, int b)
{
  int ret = a % b;
  return (ret < 0) ? ret + b : ret;
}

/*
 * free_array2d
 *   DESCRIPTION: frees the memory occupied by a 2d array
 *   INPUTS: arr -- pointer to array to be freed
 *           rows -- number of rows in the array
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void free_array2d(int **arr, int rows)
{
  // Check if arr valid
  if (arr == NULL)
  {
    return;
  }

  // Free inner arrays
  for (int i = 0; i < rows; i++)
  {
    free(arr[i]);
  }

  // Free outer arrays
  free(arr);

  return;
}

/*
 * fprint_array2d
 *   DESCRIPTION: prints the array to the provided file pointer
 *   INPUTS: stream -- file pointer where data will be printed to
 *           arr -- array whose contents will be printed
 *           rows -- number of rows in arr
 *           cols -- number of cols in arr
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void fprint_array2d(FILE *stream, int **arr, int rows, int cols)
{
  // Check if arr valid
  if (arr == NULL)
  {
    return;
  }

  for (int row = 0; row < rows; row++)
  {
    for (int col = 0; col < cols; col++)
    {
      fprintf(stream, "%d ", arr[row][col]);
    }
    fprintf(stream, "\n");
  }
}

/*
 * malloc_array2d
 *   DESCRIPTION: allocates a 2d array of size rows x cols
 *   INPUTS: rows -- number of rows in the 2d array
 *           cols -- number of columns in the 2d array
 *   OUTPUTS: none
 *   RETURN VALUE: pointer to the 2d array
 *   SIDE EFFECTS: none
 */
int **malloc_array2d(int rows, int cols)
{
  // Check if dimensions valid
  if (rows <= 0 || cols <= 0)
  {
    return NULL;
  }
  
  // Malloc outer array
  int **arr = (int **)malloc(rows * sizeof(int *));

  // Malloc inner arrays
  for (int i = 0; i < rows; i++)
  {
    arr[i] = (int *)malloc(cols * sizeof(int));
  }

  // Initialize all cells to 0 
  for (int row = 0; row < rows; row++)
  {
    for (int col = 0; col < cols; col++)
    {
      arr[row][col] = -1;
    }
  }

  // Return array
  return arr;
}

/*
 * update_array2d
 *   DESCRIPTION: updates the 2d array according to the rules specified in 
 *                Conway's Game of Life
 *   INPUTS: arr -- array to be updated
 *           rows -- number of rows in arr
 *           cols -- number of cols in arr
 *   OUTPUTS: none
 *   RETURN VALUES: none
 *   SIDE EFFECTS: none
 */
void update_array2d(int **arr, int rows, int cols)
{
	int i,j;
	for (i=0; i<rows; i++) //We have two loops for each pixel value 
	{
    		for (j = 0; j<cols; j++)
   		 {
      			int z = 0;//This z value is the number of alive or killed recently around the pixel
     			for (int p = i - 1; p <= i + 1; p++)
     			{
        			for (int q = j - 1; q <= j + 1; q++)
        			{
          				int m = mod(p + rows, rows); 
          				int n = mod(q + cols, cols);
          				if ( ((*(*(arr + m) + n) == ALIVE) || (*(*(arr + m) + n) == KILL)) && !(m == i && n == j))
          				{
            					z++;
          				}
       				}
      			}
      
      		if ((*(*(arr + i) + j) == ALIVE) || (*(*(arr + i) + j) == KILL))
      		{
        		*(*(arr + i) + j) = (((z == 2) || (z == 3)) ? ALIVE : KILL);//if 2 or 3 then alive else killed
      		}
      		else
      		{
        		*(*(arr + i) + j) = ((z == 3) ? RESPAWN : DEAD);//if exactly three neighbours then alive else killed
      		}
    		}
  	}
  
  	for (int i = 0; i < rows; i++) //This loop is basically to make alive the respawned and dead the killed. people die when they are killed!
  	{
    		for (int j = 0; j < cols; j++)
    		{
      			if (*(*(arr + i) + j) == KILL) *(*(arr + i) + j) = DEAD;
      			else if (*(*(arr + i) + j) == RESPAWN) *(*(arr + i) + j) = ALIVE;
    		}
  	}
}

/*
 * read_data
 *   DESCRIPTION: reads the data from the file into the array
 *   INPUTS: file -- pointer to file where data is located
 *   OUTPUTS: arr -- pointer to 2d array where data will be read into
 *            rows -- pointer to rows data
 *            cols -- pointer to cols data
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void read_data(FILE *file, int ***arr, int *rows, int *cols)
{
  fscanf(file, "%d %d", rows, cols);
  *arr = malloc_array2d(*rows, *cols);
  int val;
  int a = 0, b = 0;
  while ((val = fgetc(file)) != EOF)//Till we dont reach the end we need to get values
  { 
    if (val == '0')  // if the cell is 0 means it is dead
    {
      *(*(*arr + a) + b) = DEAD; // according to our  ENUM its DEAD

      goto kappa; // well now that we have assigned the cell as dead we can now increment a and b accordingly
    }
    else if (val == '1') //is it alive! yes sir
    {
      *(*(*arr + a) + b) = ALIVE;
      
      goto kappa;
    }
    
    loop:;
  }
  
  goto end;//After assigning everyone one value we just goto end
  
  kappa:
 	 if (b == *cols - 1) 
 	 {
 		   a++; b = 0;
	 }
	  else 
	  {
	   	 b++;
	  }
 goto loop;
  end:;
  
}

/*
 * print_board
 *   DESCRIPTION: prints the array to stdscr
 *   INPUTS: arr -- pointer to array whose data will be printed
 *           rows -- number of rows in arr
 *           cols -- number of cols in arr
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void print_board(int **arr, int rows, int cols)
{
  // Initialize colors
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  // Print board to stdscr
  for (int row = 0; row < rows; row++)
  {
    for (int col = 0; col < cols; col++)
    {
      if (arr[row][col])
      {
        mvaddwstr(row, col, L"\u2588");
        mvchgat(row, col, 1, A_NORMAL, 1, NULL);
      }
      else
      {
        mvaddch(row, col, ' ');
      }
    }
  }

  // Print exit message
  mvaddstr(rows, 0, "To exit, press 'q'.");
  
  // Print stdscr to terminal
  refresh();
}
