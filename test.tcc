#include "stdio.h"

double pow(double, double);
double foo(int);
int strcmp(const char *s1, const char *s2);

char table[][64] = {
        "delete",
        "add",
        "change"
};

double c99_export wrapped(int arg) {
    printf("%s\n", table[1]);
    if (strcmp("add", table[1]) == 0) {
       printf("same....\n");
    }
    double val = foo(arg);
    int x = 100;
    double y = x*1.0/23;
    printf("%lf\n", y);
    printf("pow(2,2) = %lf\n", pow(2,2));
    printf("Inside wrapped function %lf\n", val);
    return val;
}

