#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

int main() {
    char file1[255], file2[255];
    printf("Enter names of file1 and file2\n");
    scanf("%s", file1);
    scanf("%s", file2);

    FILE *f1 = fopen(file1, "w");
    FILE *f2 = fopen(file2, "w");

    if (f1 == NULL || f2 == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    char area_n[] = "area";
    char mutex1_n[] = "mutex1";
    char size_data_n[] = "size_data";

    int area = shm_open(area_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);
    int mutex1 = shm_open(mutex1_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);
 	int size_data = shm_open(size_data_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);

    if (area == -1 || mutex1 == -1) {
 		printf("Error accessing memory in shm_open\n");
 		return 2;
 	}

    if (ftruncate(area, getpagesize()) == -1) {
 		printf("ftruncate error\n");
 		return 3;
 	}

 	if (ftruncate(size_data, sizeof(int)) == -1) {
 		printf("ftruncate error\n");
 		return 3;
 	}

 	if (ftruncate(mutex1, sizeof(pthread_mutex_t)) == -1) {
        printf("ftruncate error\n");
        return 3;
    }

    return 0;




















}
