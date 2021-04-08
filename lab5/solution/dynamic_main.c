/*
Variant 4
*/
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

const char* DYN_LIB_1 = "./libimp1.so";
const char* DYN_LIB_2 = "./libimp2.so";
const char* SI_FUNC_NAME = "SinIntegral";
const char* PI_FUNC_NAME = "Pi";

int main() {
    int dynLibNum = 1;
    void* handle = dlopen(DYN_LIB_1, RTLD_LAZY);
    if (handle == NULL) {
        printf("Error opening dynamic library\n");
        return 1;
    }

    float (*SinIntegral) (float, float, float);
    float (*Pi) (int);
    //Здесь адрес указателя на функции приводится к типу двойного
    //указателя на войд и разыменовывается один раз
    *(void**)(&SinIntegral) = dlsym(handle, SI_FUNC_NAME);
    *(void**)(&Pi) = dlsym(handle, PI_FUNC_NAME);
    char* error = dlerror();
    if (error != NULL) {
        printf("%s", error);
        return 2;
    }
    int q;
    printf("Current implementation is %d\n", dynLibNum);
    while (scanf("%d", &q) > 0) {
        if (q == 0) {
            if (dlclose(handle) != 0) {
                printf("Error closing dynamic library\n");
                return 3;
            }
            if (dynLibNum == 1) {
                handle = dlopen(DYN_LIB_2, RTLD_LAZY);
            } else {
                handle = dlopen(DYN_LIB_1, RTLD_LAZY);
            }
            if (handle == NULL) {
                printf("Error open dynamic library\n");
                return 1;
            }
            *(void**)(&SinIntegral) = dlsym(handle, SI_FUNC_NAME);
            *(void**)(&Pi) = dlsym(handle, PI_FUNC_NAME);
            error = dlerror();
            if (error != NULL) {
                printf("%s", error);
                return 4;
            }
            dynLibNum = (dynLibNum == 1) ? 2 : 1;
            printf("Switched to dynLibNum %d\n", dynLibNum);
        } else if (q == 1) {
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

    if (dlclose(handle) != 0) {
        printf("Error closing dynamic library\n");
        return 3;
    }

    return 0;
}
