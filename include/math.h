#ifndef MATH_H
#define MATH_H

int pow(int b, int p) {
    int ret = 1;
    for(int i = 0; i < p; i++) {
        ret *= b;
    }
    return ret;
}

#endif