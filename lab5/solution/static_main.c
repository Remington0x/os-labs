/*
Variant 4
*/
#include "declaration.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int q;
    printf("Enter nuber of function (1 for SinIntegral, 2 for Pi)\n");
    while (scanf("%d", &q) > 0) {
        if (q == 1) {
            float a, b, e;
            printf("Enter a, b, e\n");
            if (scanf("%f%f%f", &a, &b, &e) != 3) {
                printf("Error reading float\n");
                return 1;
            }

            float res = SinIntegral(a, b, e);
            printf("Integral = %f\n", res);
        } else if (q == 2) {
            int k;
            printf("Enter number of series terms\n");
            if (scanf("%d", &k) != 1) {
                printf("Error reading int\n");
                return 2;
            }
            float res = Pi(k);
            printf("Pi = %f\n", res);
        }
    }
}
