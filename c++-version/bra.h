#ifndef BRA_H
#define BRA_H


#include <random>
#include <math.h>

using namespace std;


mt19937 random_engine(random_device{}());
uniform_real_distribution<float> rndfloat (0.0000001, 1.0);


int BRA (int n_options, float beta) {
    int i = (int) log(rndfloat(random_engine)) / log( 1.0 - beta );
    int idx = i % n_options;
    return idx;
}


#endif