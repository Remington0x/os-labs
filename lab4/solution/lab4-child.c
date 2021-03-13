#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "stringops.h"

int main(int argc, char *argv[]) {

    FILE *f = fopen(argv[0], "w");

    if (f == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int area = shm_open(argv[2], O_RDWR | O_CREAT, S_IRWXU);
    int mutex_ = shm_open(argv[1], O_RDWR | O_CREAT, S_IRWXU);
    int size_data = shm_open(argv[3], O_RDWR | O_CREAT, S_IRWXU);


    if (area == -1 || mutex_ == -1) {
        printf("Error shm_open\n");
        return 2;
    }

    float d = 0;


    char* data = (char*) mmap(NULL, getpagesize(),  PROT_READ | PROT_WRITE, MAP_SHARED, area, 0);
    int* data_s = (int*) mmap(NULL, sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED, size_data, 0);
    pthread_mutex_t *mutex1 = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex_, 0);

    if (data == MAP_FAILED || mutex1 == MAP_FAILED || data_s == MAP_FAILED) {
        printf("CHILD: Error mmap\n");
        return -3;
    }


    char str[255];
    int i;
    while (1 == 1) {
        if (*data_s == 0) continue;
        //printf("CHILD: Hurray! Got some data. Locking mutex...\n");
        if (pthread_mutex_lock(mutex1) != 0) {
            printf("CHILD: Error locking mutex\n");
            return 8;
        }
        //printf("CHILD: Success! Mutex locked.\n");

        if (*data_s == -1) break;

        for (i = 0; i < *data_s; ++i) {
            str[i] = data[i];
        }
        str[i] = '\0';

        //printf("CHILD: got string %s\n", str);

        *data_s = 0;

        if (pthread_mutex_unlock(mutex1) != 0) {
            printf("CHILD: Error unlocking mutex\n");
            return 11;
        }

        str_invert(str);
        i = get_length(str);
        str[i] = '\n';
        fprintf(f, "%s", str);
        fflush(f);
    }


    if (shm_unlink(argv[1]) != 0) {
        perror("Error shm_unlink\n");
        return -10;
    }

    if (shm_unlink(argv[2]) != 0) {
        perror("Error shm_unlink\n");
        return -10;
    }

    if (shm_unlink(argv[3]) != 0) {
        perror("Error shm_unlink\n");
        return -10;
    }

    fclose(f);

    return 0;
}
