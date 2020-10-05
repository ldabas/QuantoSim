//
// Created by Lakshit Dabas on 2/23/20.
//

#ifndef FINALPROD_BASE_H
#define FINALPROD_BASE_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <map>

using namespace std;

const double pi = M_PI;
const complex<double> im(0.0, 1.0);
typedef const vector<complex<double> > Gate;
typedef map<string, unsigned int> AbsoluteFrequency;


vector<string> product(vector<string> vector1, vector<string> vector2){
    vector<string> prod(vector1.size()*vector2.size());
    transform(vector1.begin(), vector1.end(), vector2.begin(), prod.begin(),
              [](auto l, auto r) { return l+r;});
    return prod;
}

vector<string> stateTP(unsigned int n) {
    vector<string> base = {"0", "1"};
    vector<string> newBase= {"0","1"};
    for(unsigned int i = 0; i < n-1; i++) newBase = product(newBase, base);
    return newBase;
}


#endif //FINALPROD_BASE_H
