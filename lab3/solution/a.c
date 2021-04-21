/*
Инструкция по применению:
$ make
Делает mg и a.out.
mg сгенерирует исходную матрицу для задания.
a.out -- сама программа. Запускается с ключом -t <кол-во потоков>, либо без ключа,
тогда она запустится в однопоточном режиме.

Идея распараллеливания в следующем:
В цикле for запускается нужное количество потоков, потом тоже в цикле for
ожидается окончание их всех, и результат записывается во вторую матрицу, так как
изменение исходной в процессе одного прохода невозможно. Таким образом матрица
обходится до тех пор, пока количество оставшихся элементов не станет меньше
количества потоков. Для оставшихся элементов запускается соответствующее
количество потоков и так же ожидается их завершение и запись результата во
вторую матрицу.

окно -- целое положительное нечетное число
предполагается, что окно не может быть больше размера матрицы
*/

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

typedef struct args_tag {
    int ** matrix;
    int size_x;
    int size_y;
    int elem_i;
    int elem_j;
    int window;
    int med;
} args;

void swap(int * a, int * b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void swap_p(int *** a, int *** b) {
    int ** c = *a;
    *a = *b;
    *b = c;
}

int sort(int * arr, int size) {
    int i = 1;
    int j = 2;
    while (i < size) {
        if (arr[i - 1] < arr[i]) {
            i = j;
            ++j;
        } else {
            swap(&arr[i - 1], &arr[i]);
            i = i - 1;
            if (i == 0) {
                i = j;
                ++j;
            }
        }
    }
    return 0;
}

void *median(void *a) {
    args *arg = (args*)a;
    int ** matrix = arg->matrix;
    int size_x = arg->size_x;
    int size_y = arg->size_y;
    int elem_i = arg->elem_i;
    int elem_j = arg->elem_j;
    int window = arg->window;

    int * arr = (int*)malloc(sizeof(int) * window * window);
    if (arr == NULL) {
        printf("malloc error in int median()\n");
    }

    int arr_i = 0;

    int minus_i = elem_i - window / 2;
    int minus_j = elem_j - window / 2;
    int plus_i = elem_i + window / 2;
    int plus_j = elem_j + window / 2;

    int i_out;
    int j_out;

    int out_items;

    if (minus_i < 0) {  //up
        i_out = -minus_i;
        if (minus_j < 0) { //left up
            j_out = -minus_j;
            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = 0; i < window - i_out; ++i) {
                for (int j = 0; j < window - j_out; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }
        } else
        if (plus_j < size_x) { //only up
            j_out = 0;
            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = elem_i - window / 2 + i_out; i <= elem_i + window / 2; ++i) {
                for (int j = elem_j - window / 2; j <= elem_j + window / 2; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }
        } else
        if (plus_j >= size_x) {    //right up
            j_out = plus_j - size_x + 1;
            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = elem_i - window / 2 + i_out; i <= elem_i + window / 2; ++i) {
                for (int j = elem_j - window / 2; j <= elem_j + window / 2 - j_out; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }
        }
    } else
    if (plus_j >= size_x)  { //right
        j_out = plus_j - size_x + 1;
        if (plus_i < size_y) { //only right
            i_out = 0;
            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = elem_i - window / 2; i <= elem_i + window / 2; ++i) {
                for (int j = elem_j - window / 2; j <= elem_j + window / 2 - j_out; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }
        } else
        if (plus_i >= size_y) {  //right down
            i_out = plus_i - size_y + 1;
            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = elem_i - window / 2; i <= elem_i + window / 2 - i_out; ++i) {
                for (int j = elem_j - window / 2; j <= elem_j + window / 2 - j_out; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }

        }
    } else
    if (plus_i >= size_y) {  //down
        i_out = plus_i - size_y + 1;

        if (minus_j >= 0) {  //only down
            j_out = 0;
            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = elem_i - window / 2; i <= elem_i + window / 2 - i_out; ++i) {
                for (int j = elem_j - window / 2; j <= elem_j + window / 2; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }
        } else
        if (minus_j < 0) {  //left down
            j_out = -minus_j;

            out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

            for (arr_i = 0; arr_i < out_items; ++arr_i) {
                arr[arr_i] = 0;
            }

            for (int i = elem_i - window / 2; i <= elem_i + window / 2 - i_out; ++i) {
                for (int j = elem_j - window / 2 + j_out; j <= elem_j + window / 2; ++j) {
                    arr[arr_i] = matrix[i][j];
                    ++arr_i;
                }
            }

        }
    } else
    if (minus_j < 0) {  //left only
        i_out = 0;
        j_out = -minus_j;

        out_items = i_out * (window - j_out) + j_out * (window - i_out) + i_out * j_out;

        for (arr_i = 0; arr_i < out_items; ++arr_i) {
            arr[arr_i] = 0;
        }

        for (int i = elem_i - window / 2; i <= elem_i + window / 2; ++i) {
            for (int j = elem_j - window / 2 + j_out; j <= elem_j + window / 2; ++j) {
                arr[arr_i] = matrix[i][j];
                ++arr_i;
            }
        }
    } else {    //neechego ne podoshlo
        for (int i = elem_i - window / 2; i <= elem_i + window / 2; ++i) {
            for (int j = elem_j - window / 2; j <= elem_j + window / 2; ++j) {
                arr[arr_i] = matrix[i][j];
                ++arr_i;
            }
        }
    }

    //on this point we have unsorted array of integer arr[]

    sort(arr, window * window);

    int med = arr[window * window / 2];

    free(arr);

    arg->med = med;

    return 0;
}

int main(int argc, char * argv[]) {
    int cthreads = 1;
    if (argc == 2) {
        if (argv[0] == "-t") {
            cthreads = atoi(argv[1]);
        }
    }

    int size_x, size_y;

    FILE* fin;
    fin = fopen("in.txt", "r");
    if (fin == NULL) {
        printf("Error opening file, try to start ./mg first\n");
    }

    //matrix input
    fscanf(fin, "%d", &size_y);
    fscanf(fin, "%d", &size_x);

    int ** matrix = (int**)malloc(sizeof(int*) * size_y);
    if (matrix == NULL) {
        printf("matrix malloc error in main()\n");
        return 1;
    }

    for (int i = 0; i < size_y; ++i) {
        matrix[i] = (int*)malloc(sizeof(int) * size_x);
        if (matrix[i] == NULL) {
            printf("matrix malloc error in main()\n");
            return 1;
        }
    }

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_x; ++j) {
            fscanf(fin, "%d", &matrix[i][j]);
        }
    }

    int window;
    printf("Enter window size. It has to be positive odd integer\n");
    scanf("%d", &window);
    if (window < 0 || (window % 2 == 0)) {
        printf("Wrong window size! Shutting down...\n");
        return -1;
    }

    int k;
    printf("Enter K\n");
    scanf("%d", &k);
    //Соображения такие: в цикле от 0 до кол-во элементов
    //cthreads -- кол-во потоков
    int ** matrix2 = (int**)malloc(sizeof(int*) * size_y);
    if (matrix2 == NULL) {
        printf("matrix malloc error in main()\n");
        return 1;
    }

    for (int i = 0; i < size_y; ++i) {
        matrix2[i] = (int*)malloc(sizeof(int) * size_x);
        if (matrix2[i] == NULL) {
            printf("matrix malloc error in main()\n");
            return 1;
        }
    }

    args * a = (args*)malloc(sizeof(args) * cthreads);
    for (int i = 0; i < cthreads; ++i) {
        a[i].matrix = matrix;
        a[i].size_x = size_x;
        a[i].size_y = size_y;
        a[i].window = window;
    }

    pthread_t * threads = (pthread_t*)malloc(sizeof(pthread_t) * cthreads);
    int status;
    int j;

    clock_t time = clock();

    for (int i = 0; i < k; ++i) {
        for (j = 0; j <= size_x * size_y - cthreads; j += cthreads) {
            for (int m = 0; m < cthreads; ++m) {
                //start thread
                a[m].elem_i = (j + m) / size_x;
                a[m].elem_j = (j + m) % size_x;
                status = pthread_create(&threads[m], NULL, median, (void*) &a[m]);
                if (status != 0) {
                    printf("Error creating thread in main()\n");
                    return 2;
                }
            }
            //wait for all the threads to finish
            //put all the results in 2nd matrix
            for (int m = 0; m < cthreads; ++m) {
                status = pthread_join(threads[m], NULL);

                if (status != 0) {
                    printf("Error joining thread in main()\n");
                    return 3;
                }

                matrix2[(j + m) / size_x][(j + m) % size_x] = a[m].med;
            }
        }
        //Если количество элементов матрицы не кратно количеству потоков
        if (j < size_x * size_y) {
            for (int m = j; m < size_x * size_y; ++m) {
                //start thread
                a[m].elem_i = (j + m) / size_x;
                a[m].elem_j = (j + m) % size_x;
                status = pthread_create(&threads[m], NULL, median, (void*) &a[m]);
                if (status != 0) {
                    printf("Error creating thread in main()\n");
                    return 2;
                }
            }
            //wait for all the threads to finish
            //put all the results in 2nd matrix
            for (int m = 0; m < size_x * size_y - j; ++m) {
                status = pthread_join(threads[m], NULL);

                if (status != 0) {
                    printf("Error joining thread in main()\n");
                    return 3;
                }

                matrix2[(j + m) / size_x][(j + m) % size_x] = a[m].med;
            }
        }

        swap_p(&matrix, &matrix2);
        //put 2nd matrix into 1st
    }

    time = clock() - time;

    printf("Multi-thread ticks = %d, (%f sec)\n", time, ((float)time / CLOCKS_PER_SEC));

    FILE* fout = fopen("out.txt", "w");

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_x; ++j) {
            fprintf(fout, "%d ", matrix[i][j]);
        }
        fprintf(fout, "\n");
    }


    for (int i = 0; i < size_y; ++i) {
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; i < size_y; ++i) {
        free(matrix2[i]);
    }
    free(matrix2);
    free(threads);
    free(a);
    fclose(fin);
    fclose(fout);

    return 0;
}
