// Copyright 2012 www.mpitutorial.com
// Program yang menghitung rata-rata dari array secara paralel menggunakan Scatter dan Gather.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

void printToFile(int* arr, int n, double elapsed_time) {
  FILE * fstream;
  fstream = fopen("../out/output","w");

  fprintf(fstream, "elapsed time: %f\n", elapsed_time);
  for (int i = 0; i < n; i++) {
    fprintf(fstream, "%d\n", arr[i]);
  }

  fclose(fstream);
}

void rng(int* arr, int n) {
  int seed = 13516154;
  srand(seed);
  for (long i = 0; i < n; i++) {
    arr[i] = (int)rand();
  }
}

int main(int argc, char** argv) {
  int n_element = atoi(argv[1]);
  int arrlel[9] = {22, 21, 11, 39, 99, 3230, 323, 110, 32123};
  int id, comm_size;
  int* arr = (int*) malloc(n_element * sizeof(int));

  rng(arr, n_element);
  double start, end;

  // start time
  start = MPI_Wtime();

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  int n_element_per = n_element / comm_size;

  // print initial array
  // if (id == 0) {
  //   printf("initial array:\n");
  //   for (int i = 0; i < n_element; i++) {
  //     printf("%d\n", arr[i]);
  //   }
  // }

  for (int idx = 0; idx < 32; idx++) {
    /* GENERATE FLAGS */
    int* flags = NULL;

    if (id == 0) {
      flags = (int*) malloc(n_element * sizeof(int));
      assert(flags != NULL);
    }

    int* local_arr = (int*) malloc(n_element_per * sizeof(int));
    assert(local_arr != NULL);

    MPI_Scatter(arr, n_element_per, MPI_INT, local_arr,
      n_element_per, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_flags = (int*) malloc(n_element_per * sizeof(int));
    assert(local_flags != NULL);

    for (int i = 0; i < n_element_per; i++) {
      // printf("local arr %d: %d", i, local_arr[i]);
      if ((local_arr[i] >> idx) & 1 == 1) {
        local_flags[i] = 1;
      } else {
        local_flags[i] = 0;
      }
    }

    MPI_Gather(local_flags, n_element_per, MPI_INT, flags, n_element_per, MPI_INT, 0,
    MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    free(local_arr);
    free(local_flags);
    /* FINISH GENERATING FLAGS */

    if (id == 0) {
      for (int i = 0; i < n_element; i++) {
        // printf("flag %d %d", i, flags[i]);
      }

    }

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
      for (int i = 0; i < n_element; i++) {
        // printf("i down %d", i_down[i]);
      }
      // printf("\n");
    }
    /* FINISH GENERATING INDEX DOWN */

    // /* GENERATE INDEX UP */
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
      for (int i = 0; i < n_element; i++) {
        // printf("i up %d", i_up[i]);
      }
      // printf("\n");
    }
    // /* FINISH GENERATING INDEX UP */
    MPI_Barrier(MPI_COMM_WORLD);

    // /* GENERATE SHOULD INDEX */
    int* should_index = NULL;

    if (id == 0) {
      should_index = (int*) malloc(n_element * sizeof(int));
      assert(should_index != NULL);
    }

    local_flags = (int*) malloc(n_element_per * sizeof(int));
    assert(local_flags != NULL);

    int* local_i_down = (int*) malloc(n_element_per * sizeof(int));
    assert(local_i_down != NULL);

    int* local_i_up = (int*) malloc(n_element_per * sizeof(int));
    assert(local_i_up != NULL);

    MPI_Scatter(flags, n_element_per, MPI_INT, local_flags,
      n_element_per, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(i_down, n_element_per, MPI_INT, local_i_down,
      n_element_per, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(i_up, n_element_per, MPI_INT, local_i_up,
      n_element_per, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_should_index = (int*) malloc(n_element_per * sizeof(int));
    assert(local_should_index != NULL);

    for (int i = 0; i < n_element_per; i++) {
      if (local_flags[i] == 0) {
        local_should_index[i] = local_i_down[i];
      } else {
        local_should_index[i] = local_i_up[i];
      }
      // printf("\nlocal_should_index %d: %d\n", i, local_should_index[i]);
    }

    MPI_Gather(local_should_index, n_element_per, MPI_INT, should_index, n_element_per, MPI_INT, 0,
    MPI_COMM_WORLD);

    free(local_flags);
    free(local_should_index);
    free(local_i_down);
    free(local_i_up);
    // /* FINISH GENERATING SHOULD INDEX */

    // /* PERMUTE */
    int* should_arr = NULL;

    if (id == 0) {
      should_arr = (int*) malloc(n_element * sizeof(int));
      assert(should_arr);
      for (int i = 0; i < n_element; i++) {
        // printf("should index: %d\n", should_index[i]);
        should_arr[should_index[i]] = arr[i];
      }

      for (int i = 0; i < n_element; i++) {
        arr[i] = should_arr[i];
        // printf("progress: %d\n", arr[i]);
      }
      // printf("\n");
    }

    // /* FINISH PERMUTE */
    // /* FREE MALLOCS */
    free(flags);
    free(i_down);
    free(i_up);
    free(should_index);
    }
    // MPI_Bcast(arr, n_element, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  end = MPI_Wtime();

  double elapsed_time = (end - start) * 1000000;

  if (id == 0) {
    printf("elapsed time: %f\n", elapsed_time);
    printToFile(arr, n_element, elapsed_time);
  }
}
