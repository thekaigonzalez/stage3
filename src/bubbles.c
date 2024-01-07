// $Id: bubbles.c
// ew bubble sort

#include <stdio.h>
#include <stdlib.h>

void
bubblesort (int *arr, int start, int end)
{
  int i;

  for (i = start; i < end - 1; i++)
    {
      printf ("%d > %d\n", arr[i + 1], arr[i]);

      if (arr[i + 1] > arr[i])
        {
          int tmp = arr[i];
          arr[i] = arr[i + 1];
          arr[i + 1] = tmp;
          i++;
        }
    }
}
int
main (void)
{
  int *ints = calloc (5, sizeof (int));

  ints[0] = 4;
  ints[1] = 3;
  ints[2] = 1;
  ints[3] = 2;
  ints[4] = 5;

  bubblesort (ints, 0, 4);

  for (int i = 0; i < 5; ++i)
    {
      printf ("%d ", ints[i]);
    }

  printf ("\n");

  free (ints);
}
