#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "stringops.h"

int main(void) {
    char str[STRLENGTH];
    strnull(str);
    int n;
    char c;
    int read_ret;
    while ((read_ret = read(fileno(stdin), &str, STRLENGTH * sizeof(char))) > 0) {
        //printf("read returned %d\n", read_ret);
        //fflush(stdout);
        str_invert(str);
        n = get_length(str);
        str[n] = '\n';
        printf("%s", str);
        fflush(stdout);
    }
    
    return 0;
}
