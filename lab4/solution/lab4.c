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

    char area1_n[] = "area1";
    char mutex1_n[] = "mutex1";
    char size_data1_n[] = "size_data1";

    int area1 = shm_open(area1_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);
    int mutex1_ = shm_open(mutex1_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);
 	int size_data1 = shm_open(size_data1_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);

    char area2_n[] = "area2";
    char mutex2_n[] = "mutex2";
    char size_data2_n[] = "size_data2";

    int area2 = shm_open(area2_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);
    int mutex2_ = shm_open(mutex2_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);
 	int size_data2 = shm_open(size_data2_n, O_RDWR | O_CREAT, S_IRWXO | S_IRWXU);

    if (area1 == -1 || mutex1_ == -1 || area2 == -1 || mutex2_ == -1) {
 		printf("Error accessing memory in shm_open\n");
 		return 2;
 	}

    if (ftruncate(area1, getpagesize()) == -1 || ftruncate(area2, getpagesize()) == -1) {
 		printf("ftruncate error\n");
 		return 3;
 	}

 	if (ftruncate(size_data1, sizeof(int)) == -1 || ftruncate(size_data2, sizeof(int)) == -1) {
 		printf("ftruncate error\n");
 		return 3;
 	}

 	if (ftruncate(mutex1_, sizeof(pthread_mutex_t)) == -1 || ftruncate(mutex2_, sizeof(pthread_mutex_t)) == -1) {
        printf("ftruncate error\n");
        return 3;
    }

    char* data1 = (char*) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, area1, 0);
    int* data_s1 = (int*) mmap(NULL, sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED, size_data1, 0);
    pthread_mutex_t* mutex1 = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex1_, 0);

    char* data2 = (char*) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, area2, 0);
    int* data_s2 = (int*) mmap(NULL, sizeof(int),  PROT_READ | PROT_WRITE, MAP_SHARED, size_data2, 0);
    pthread_mutex_t* mutex2 = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex2_, 0);

    if (data1 == MAP_FAILED || mutex1 == MAP_FAILED || data_s1 == MAP_FAILED || data2 == MAP_FAILED || mutex2 == MAP_FAILED || data_s2 == MAP_FAILED) {
        printf("mmap error\n");
        return 4;
    }

    pthread_mutexattr_t mutex1_at, mutex2_at;
    if (pthread_mutexattr_init(&mutex1_at) != 0 || pthread_mutexattr_init(&mutex2_at) != 0) {
        printf("Error mutex attributes init\n");
        return 5;
    }

    if (pthread_mutexattr_setpshared(&mutex1_at, PTHREAD_PROCESS_SHARED) != 0 || pthread_mutexattr_setpshared(&mutex2_at, PTHREAD_PROCESS_SHARED) != 0) {
        printf("Error mutex scope change\n");
        return 6;
    }

    if (pthread_mutex_init(mutex1, &mutex1_at) != 0 || pthread_mutex_init(mutex2, &mutex2_at) != 0) {
        printf("Error mutex init\n");
        return 7;
    }

    if (pthread_mutex_lock(mutex1) != 0 || pthread_mutex_lock(mutex2) != 0) {
        printf("Error mutex lock\n");
        return 8;
    }

    pid_t id1 = fork();

    if (id1 == -1) {
        printf("Error creating process\n");
        return 9;
    }

    if (id1 = 0) {
        //this is child
        if (pthread_mutex_lock(mutex1) != 0) {
        	printf("Error mutex lock\n");
       		return 8;
	   	}

 		char* argv[] = {file1, mutex1_n, area1_n, size_data1_n, (char *) NULL};
	    if (execv("child.out", argv) == -1) {
			printf("Error exexv\n");
			printf("%s\n", strerror(errno));
	    	return 9;
	    }

	    if (pthread_mutex_unlock(mutex1) != 0) {
			printf("Error mutex unlock\n");
			return 10;
		}

    } else
    if (id1 != 0) {
        //this is parent
        
    }



















    return 0;
}
