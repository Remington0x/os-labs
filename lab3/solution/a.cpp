/*
окно -- целое положительное нечетное число
предполагается, что окно не может быть больше размера матрицы
*/

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <ctime>

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
        std::cout << "malloc error in int median()\n";
    }
    //std::cout << "allocd arr int median\n";

    //std::cout << "Elem is: " << matrix[elem_i][elem_j] << std::endl;

    int arr_i = 0;

    int minus_i = elem_i - window / 2;
    int minus_j = elem_j - window / 2;
    int plus_i = elem_i + window / 2;
    int plus_j = elem_j + window / 2;

    //std::cout << "minus_i = " << minus_i << std::endl;
    //std::cout << "minus_j = " << minus_j << std::endl;
    //std::cout << "plus_i = " << plus_i << std::endl;
    //std::cout << "plus_j = " << plus_j << std::endl;

    int i_out;
    int j_out;

    int out_items;

    if (minus_i < 0) {  //up
        //std::cout << "up\n";
        i_out = -minus_i;
        if (minus_j < 0) { //left up
            //std::cout << "Its left up\n";
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
            //std::cout << "Its up\n";
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
            //std::cout << "Its right up\n";
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
        //std::cout << "right\n";
        j_out = plus_j - size_x + 1;
        if (plus_i < size_y) { //only right
            //std::cout << "Its right\n";
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
            //std::cout << "Its right down\n";
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
        //std::cout << "down\n";
        i_out = plus_i - size_y + 1;

        if (minus_j >= 0) {  //only down
            //std::cout << "Its only down\n";
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
            //std::cout << "Its left down\n";
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
        //std::cout << "Its left\n";
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
        //std::cout << "Ono vnutri!\n";
        for (int i = elem_i - window / 2; i <= elem_i + window / 2; ++i) {
            for (int j = elem_j - window / 2; j <= elem_j + window / 2; ++j) {
                arr[arr_i] = matrix[i][j];
                ++arr_i;
            }
        }
    }

    //on this point we have unsorted array of integer arr[]
    // std::cout << "arr is:\n";
    // for (int i = 0; i < window * window; ++i) {
    //     std::cout << arr[i] << ' ';
    // }
    // std::cout << std::endl;

    sort(arr, window * window);

    // std::cout << "sorted arr is:\n";
    // for (int i = 0; i < window * window; ++i) {
    //     std::cout << arr[i] << ' ';
    // }
    // std::cout << std::endl;

    int med = arr[window * window / 2];

    // std::cout << "med is: " << med << std::endl;

    free(arr);

    //std::cout << "freed arr in median\n";

    //return med;
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

    std::ifstream fin;
    fin.open("in.txt");

    //matrix input
    fin >> size_y;
    fin >> size_x;

    int ** matrix = (int**)malloc(sizeof(int*) * size_y);
    if (matrix == NULL) {
        std::cout << "matrix malloc error in main()\n";
        return 1;
    }

    for (int i = 0; i < size_y; ++i) {
        matrix[i] = (int*)malloc(sizeof(int) * size_x);
        if (matrix[i] == NULL) {
            std::cout << "matrix malloc error in main()\n";
            return 1;
        }
    }

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_x; ++j) {
            fin >> matrix[i][j];
        }
    }
    //echo
    // std::cout << "Matrix is:\n";
    // for (int i = 0; i < size_y; ++i) {
    //     for (int j = 0; j < size_x; ++j) {
    //         std::cout << matrix[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }

    int window;
    std::cout << "Enter window size\n";
    std::cin >> window;

    int k;
    std::cout << "Enter K\n";
    std::cin >> k;
    //Соображения такие: в цикле от 0 до кол-во элементов
    //cthreads -- кол-во потоков
    int ** matrix2 = (int**)malloc(sizeof(int*) * size_y);
    if (matrix2 == NULL) {
        std::cout << "matrix malloc error in main()\n";
        return 1;
    }

    for (int i = 0; i < size_y; ++i) {
        matrix2[i] = (int*)malloc(sizeof(int) * size_x);
        if (matrix2[i] == NULL) {
            std::cout << "matrix malloc error in main()\n";
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
                    std::cout << "Error creating thread in main()\n";
                    return 2;
                }
            }
            //wait for all the threads to finish
            //put all the results in 2nd matrix
            for (int m = 0; m < cthreads; ++m) {
                status = pthread_join(threads[m], NULL);

                if (status != 0) {
                    std::cout << "Error joining thread in main()\n";
                    return 3;
                }

                matrix2[(j + m) / size_x][(j + m) % size_x] = a[m].med;
            }
        }

        if (j < size_x * size_y) {
            for (int m = j; m < size_x * size_y; ++m) {
                //start thread
                a[m].elem_i = (j + m) / size_x;
                a[m].elem_j = (j + m) % size_x;
                status = pthread_create(&threads[m], NULL, median, (void*) &a[m]);
                if (status != 0) {
                    std::cout << "Error creating thread in main()\n";
                    return 2;
                }
            }
            //wait for all the threads to finish
            //put all the results in 2nd matrix
            for (int m = 0; m < cthreads; ++m) {
                status = pthread_join(threads[m], NULL);

                if (status != 0) {
                    std::cout << "Error joining thread in main()\n";
                    return 3;
                }

                matrix2[(j + m) / size_x][(j + m) % size_x] = a[m].med;
            }
        }

        swap_p(&matrix, &matrix2);
        //put 2nd matrix into 1st
    }

    time = clock() - time;

    std::cout << "Multi-thread ticks = " << time << " (" << (float)time / CLOCKS_PER_SEC << " sec)\n";

    std::ofstream fout("out.txt");

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_x; ++j) {
            fout << matrix[i][j] << ' ';
        }
        fout << '\n';
    }


    for (int i = 0; i < size_y; ++i) {
        free(matrix[i]);
        //std::cout << "freed matrix[" << i << "]\n";
    }
    free(matrix);

    for (int i = 0; i < size_y; ++i) {
        free(matrix2[i]);
        //std::cout << "freed matrix[" << i << "]\n";
    }
    free(matrix2);
    free(threads);
    free(a);
    fin.close();
    fout.close();

    return 0;
}
