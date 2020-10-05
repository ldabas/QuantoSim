//
// Created by Lakshit Dabas on 2/23/20.
//

#ifndef FINALPROD_GATES_H
#define FINALPROD_GATES_H

#include "Base.hpp"

using namespace std;

namespace QuantoSim {
    namespace Gates {
        Gate H = {
                1/sqrt(2), 1/sqrt(2),
                1/sqrt(2), -1/sqrt(2)};
        Gate X = {
                0,1,
                1,0};
        Gate Z = {
                1,0,
                0,-1};
        Gate Y = {
                0,-im,
                im,0};
        Gate T = {
                1,0,
                0,exp(im*pi/4.0)};
        Gate Rm(unsigned int m) {
            complex<double> omega = exp(2*M_PI*im/pow(2, m));
            Gate R({
                1,0,
                0,omega});
            return R;
        }
    }
};


#endif //FINALPROD_GATES_H
