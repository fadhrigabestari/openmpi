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

// get specific bit at index = idx
int* generateFlag(int* arr, int n, int idx) {
  int* flag = (int*) malloc(n * sizeof(int));

  for (int i = 0; i < n; i++) {
    if ((arr[i] >> idx) & 1 == 1) {
      flag[i] = 1;
    } else {
      flag[i] = 0;
    }
  }
  return flag;
}



// create I-down array
int* generateIDown(int* flag, int n) {
  int* iDown = (int*) malloc(n * sizeof(int));
  int val = 0;

  iDown[0] = val;
  for (int i = 1; i < n; i++) {
    if (flag[i - 1] == 0) {
      val++;
    }
    iDown[i] = val;
  }
  return iDown;
}

// create I-up array
int* generateIUp(int* flag, int n) {
  int* iUp = (int*) malloc(n * sizeof(int));
  int val = n - 1;

  iUp[n - 1] = val;
  for (int i = n - 2; i >= 0; i--) {
    if (flag[i + 1] == 1) {
      val--;
    }
    iUp[i] = val;
  }
  return iUp;
}

int* generateShouldIndex(int* flag, int* iDown, int* iUp, int n) {
  int* shouldIndex = (int*) malloc(n * sizeof(int));

  for (int i = 0; i < n; i++) {
    if (flag[i] == 0) {
      shouldIndex[i] = iDown[i];
    } else {
      shouldIndex[i] = iUp[i];
    }
  }
  return shouldIndex;
}

// permute
void permute(int* arr, int* flag, int* iDown, int* iUp, int n) {
  int* shouldArr = (int*) malloc(n * sizeof(int));

  int* shouldIndex = generateShouldIndex(flag, iDown, iUp, n);

  for (int i = 0; i < n; i++) {
    shouldArr[shouldIndex[i]] = arr[i];
  }

  for (int i = 0; i < n; i++) {
    arr[i] = shouldArr[i];
  }
}

void split(int* arr, int n, int idx) {
  int* flag = generateFlag(arr, n, idx);
  int* iDown = generateIDown(flag, n);
  int* iUp = generateIUp(flag, n);

  permute(arr, flag, iDown, iUp, n);
}

void radixSort(int* arr, int n) {
  for (int i = 0; i < 32; i++) {
    split(arr, n, i);
  }
}

int main(int argc, char** argv) {
  int n_element = atoi(argv[1]);
  int id, comm_size;
  int* arr = (int*) malloc(n_element * sizeof(int));
  int* arr_serial = (int*) malloc(n_element * sizeof(int));

  rng(arr, n_element);
  rng(arr_serial, n_element);

  double start, end;
  double start_serial, end_serial;
  
  // start serial
  start_serial = MPI_Wtime();
  radixSort(arr, n_element);
  end_serial = MPI_Wtime();
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

  if (id == 0) {
    double elapsed_time = (end - start) * 1000000;
    double elapsed_time_serial = (end_serial - start_serial) * 1000000;

    printf("elapsed time serial: %f\n", elapsed_time_serial);
    printf("elapsed time parallel: %f\n", elapsed_time);
    printToFile(arr, n_element, elapsed_time);
  }
}
