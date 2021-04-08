/*
Интеграл от синуса методом трапеций

Пи через формулу Валлиса
*/

#include "declaration.h"
#include <math.h>

float SinIntegral(float a, float b, float e) {
    float res = 0;
    float x1, x2;
    float n = (b - a) / e;


    for (int i = 0; i < n; ++i) {
        x1 = a + i * e;
        x2 = a + (i + 1) * e;

        res += 0.5 * (sin(x1) + sin(x2));
    }

    res *= e;

    return res;
}


float Pi(int k) {
    if (k < 1) {
        printf("ERROR: Pi(): k must be greater than 0\n");
    }
    float res = 1;

    for (int i = 1; i <= k; ++i) {
        res *= (float)(4 * i * i) / (4 * i * i - 1);
    }

    res *= 2;

    return res;
}
