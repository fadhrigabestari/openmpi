// Copyright 2012 www.mpitutorial.com
// Program yang menghitung rata-rata dari array secara paralel menggunakan Scatter dan Gather.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

void rng(int* arr, int n) {
  int seed = 13516154;
  srand(seed);
  for (long i = 0; i < n; i++) {
    arr[i] = (int)rand();
  }
}

float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}

int* generate_flags(int* arr, int n_element, int n_element_per, int idx, int id, int comm_size) {
  int* flags = NULL;

  if (id == 0) {
    flags = (int*) malloc(n_element * sizeof(int));
    assert(flags != NULL);
  }

  int *local_arr = (int*) malloc(n_element_per * sizeof(int));
  assert(local_arr != NULL);

  MPI_Scatter(arr, n_element_per, MPI_INT, local_arr,
    n_element_per, MPI_INT, 0, MPI_COMM_WORLD);

  int* local_flags = (int*) malloc(n_element_per * sizeof(int));
  assert(local_flags != NULL);

  for (int i = 0; i < n_element_per; i++) {
    if ((local_arr[i] >> idx) & 1 == 1) {
      local_flags[i] = 1;
    } else {
      local_flags[i] = 0;
    }
  }

  MPI_Gather(&local_flags, n_element_per, MPI_INT, flags, n_element, MPI_INT, 0,
  MPI_COMM_WORLD);

  if (id == 0) {
    for (int i = 0; i < n_element; i++) {
      printf("%d\n", flags[i]);
    }
  }

  free(local_arr);
  free(local_flags);

  if (id == 0) {
    return flags;
  }
}

int main(int argc, char** argv) {
  int n_element = atoi(argv[1]);
  int* arr = (int*) malloc(n_element * sizeof(int));
  int id, comm_size;

  rng(arr, n_element);
  
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  int n_element_per = comm_size;

  // print initial array
  if (id == 0) {
    for (int i = 0; i < n_element; i++) {
      printf("arr: %d\n", arr[i]);
    }
  }
  
  for (int idx = 0; idx < n_element; idx++) {
    /* GENERATE FLAGS */
    int* flags = NULL;

    if (id == 0) {
      flags = (int*) malloc(n_element * sizeof(int));
      assert(flags != NULL);
    }

    int *local_arr = (int*) malloc(n_element_per * sizeof(int));
    assert(local_arr != NULL);

    MPI_Scatter(arr, n_element_per, MPI_INT, local_arr,
      n_element_per, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_flags = (int*) malloc(n_element_per * sizeof(int));
    assert(local_flags != NULL);

    for (int i = 0; i < n_element_per; i++) {
      if ((local_arr[i] >> idx) & 1 == 1) {
        local_flags[i] = 1;
      } else {
        local_flags[i] = 0;
      }
    }

    MPI_Gather(local_flags, n_element_per, MPI_INT, flags, n_element_per, MPI_INT, 0,
    MPI_COMM_WORLD);

    free(local_arr);
    free(local_flags);
    /* FINISH GENERATING FLAGS */

    /* GENERATE INDEX DOWN */
    int* i_down = NULL;
    if (id == 0) {
      i_down = (int*) malloc(n_element * sizeof(int));
      assert(i_down != NULL);
      int val = 0;

      i_down[0] = val;
      for (int i = 1; i < n_element; i++) {
        if (flags[i - 1] == 0) {
          val++;
        }
        i_down[i] = val;
      }
    }

    /* GENERATE INDEX UP */
    int* i_up = NULL;
    if (id == 0) {
      i_up = (int*) malloc(n_element * sizeof(int));
      assert(i_up != NULL);
      int val = n_element - 1;

      i_up[n_element - 1] = val;
      for (int i = n_element - 2; i >= 0; i--) {
        if (flags[i + 1] == 1) {
          val--;
        }
        i_up[i] = val;
      }
    }
    /* FREE MALLOCS */
    free(flags);
    free(i_down);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
