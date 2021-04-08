/*
Интеграл от синуса методом прямоугольников

Пи через ряд Лейбница
*/

#include "declaration.h"

float SinIntegral(float a, float b, float e) {
    float res = 0;
    float n = (b - a) / e;

    for (int i = 0; i < n; ++i) {
        res += sin(a + e * (i + 0.5));
    }

    res *= e;

    return res;
}

float Pi(int k) {
    float res = 0;

    for (int i = 0; i < k; ++i) {
        res += (pow(-1, i)) / (2 * i + 1);
    }

    res *= 4;

    return res;
}
