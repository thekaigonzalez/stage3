// $Id: quicksort.c
// quicksort.c - Implementation of the
// QuickSort algorithm for sorting an
// array of integers.

#include <stdio.h>
#include <stdlib.h>

int partition (int *, int, int);

// sorts *arr by constantly swapping
// sub-lists with the pivot.
void
quicksort (int *arr, int start, int end)
{
  // if the end is less than or equal to
  // the start we've finished.
  if (end <= start)
    return;

  // the pivot is the point in which two
  // sides of an array are generated,
  // the lesser and greater side, both
  // of which are also recursively
  // sorted in order to ensure that the
  // entire array ends up having the
  // same sorting operations done on
  // them, making it a fully sorted array.
  int pivot = partition (arr, start, end);

  // recursively sorting the lesser side
  // of the pivot
  quicksort (arr, start, pivot - 1);

  // and the greater side
  quicksort (arr, pivot + 1, end);
}

// partitions *arr into two sub-arrays
// in order to do the
// "conquer-and-divide" mechanisms
int
partition (int *arr, int start, int end)
{
  // the last element is the pivot in
  // this algorithm, but it can be
  // anything inside the array.
  // 1 5 7 3 2 6
  //           ^~~ pivot
  int pivot = arr[end];
  // i is the pointer BEHIND j, and it's
  // what J will swap with if the array
  // at the J index is less than the pivot.
  int i = start - 1;

  // j is the current number in this partition.
  int j;
  for (j = start; j <= end - 1; ++j)
    {
      // if the array at j is less than
      // the pivot, increment i and swap
      // them both.
      if (arr[j] < pivot)
        {
          // move i forward
          i++;
          // swap array at i and the
          // array at j
          int tmp = arr[i];
          arr[i] = arr[j];
          arr[j] = tmp;
        }
    }
  i++; // increment i to it's final place.

  // swap arr[i] with arr[end]
  int tmp = arr[i];
  arr[i] = arr[end];
  arr[end] = tmp;

  // NOTE THAT THESE FUNCTIONS DO REPEAT
  // EACH OTHER, IT IS IMPORTANT THAT
  // CHECKS ARE IN PLACE TO STOP WHERE NECESSARY!
  return i;
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
  
  quicksort (ints, 0, 4);

  for (int i = 0; i < 5; ++i)
    {
      printf ("%d ", ints[i]);
    }

  printf ("\n");

  free (ints);
}
