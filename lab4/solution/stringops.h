#ifndef _STRINGOPS_H_
#define _STRINGOPS_H_

#include <stdio.h>

#define FILENAME_LENGTH 255
#define STRLENGTH 40000

void strnull(char *str) {
    for (int i = 0; i < FILENAME_LENGTH; ++i) {
        str[i] = '\0';
    }
}

int is_exit(char *str) { //returns 1 if exit
    char *ex = "exit";
    int flag = 1; // 1 if exit
    for (int i = 0; i < 4; ++i) {
        if (str[i] != ex[i]) {
            flag = 0;
        }
    }
    if (flag == 0) {
        return 0;
    } else {
        return 1;
    }
}

int get_length(char *str) {
    int counter = 0;
    while (str[counter] != '\0') {
        ++counter;
    }

    return counter;
}

void str_invert(char *str) {
    int len = get_length(str);
    char c;
    for (int i = 0; i < len / 2; ++i) {
         c = str[i];
         str[i] = str[len - i - 1];
         str[len - i - 1] = c;
    }
}

#endif
