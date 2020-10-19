/*
    Variant #20
*/
#include "stringops.h"
#include "unistd.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int CreateChild(int pipe[2], FILE* file);

int main() {
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    char fname1[FILENAME_LENGTH];
    char fname2[FILENAME_LENGTH];

    strnull(fname1);
    strnull(fname2);

    int i_write;
    FILE* file1 = NULL;
    FILE* file2 = NULL;

    printf("Enter name of file 1\n");
    scanf("%s", &fname1);

    printf("Enter name of file 2\n");
    scanf("%s", &fname2);

    file1 = fopen(fname1, "w");
    if (file1 == NULL) {
        printf("fopen error\n");
        return -3;
    }

    file2 = fopen(fname2, "w");
    if (file2 == NULL) {
        printf("fopen error\n");
        return -3;
    }

    pid_t id1 = fork();
    if (id1 == -1) {
        printf("fork error\n");
        return -1;
    } else if (id1 == 0) {
        //it is child
        CreateChild(pipe1, file1);
    } else {
        //it is parent and there is one child
        pid_t id2 = fork();
        if (id2 == -1) {
            printf("fork error\n");
            return -1;
        } else if (id2 == 0) {
            //it is child
            CreateChild(pipe2, file2);
        } else {
            //it is parent and there are two children
            char str[STRLENGTH];
            strnull(str);
            int flag = 0;
            while (1 == 1) {
                printf("Enter string\n");
                scanf("%s", &str);

                if (is_exit(str) == 1) {
                    break;
                }

                //printf("Got string %s, deciding a pipe to use\n", str);
                if (get_length(str) > 10) { //to pipe2
                    //printf("Decided to use pipe2\n");
                    i_write = write(pipe2[1], &str, STRLENGTH * sizeof(char));
                    if (i_write == -1) {
                        printf("write error\n");
                        return -5;
                    }
                } else {
                    //printf("Decided to use pipe1\n");
                    i_write = write(pipe1[1], &str, STRLENGTH * sizeof(char));
                    if (i_write == -1) {
                        printf("write error\n");
                        return -5;
                    }
                }
                //printf("Str throwed to pipe, nulling it\n");
                strnull(str);
            }
            //here exit returned
            close(pipe1[1]);
            close(pipe1[0]);
            close(pipe2[1]);
            close(pipe2[0]);
            fclose(file1);
            fclose(file2);

            printf("Pipes closed\n");
            //kill(id1, SIGKILL);
            //kill(id2, SIGKILL);
        }
    }

    return 0;
}

int CreateChild(int pipe[2], FILE* file) {
    int i_dup, i_exec;
    i_dup = dup2(pipe[0], fileno(stdin));
    if (i_dup == -1) {
        printf("dup2 error\n");
        return -2;
    }
    i_dup = dup2(fileno(file), fileno(stdout));
    if (i_dup == -1) {
        printf("dup2 error\n");
        return -2;
    }
    i_exec = execv("lab2-child.out", NULL);
    if (i_exec == -1) {
        printf("execv error\n");
        return -4;
    }
}
