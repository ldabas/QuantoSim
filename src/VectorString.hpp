//
// Created by Lakshit Dabas on 2/24/20.
//

#ifndef FINALPROD_VECTORSTRING_H
#define FINALPROD_VECTORSTRING_H

#include "Base.hpp"
#include "Gates.hpp"

#include <vector>

namespace QuantoSim {
    class VectorString {
    public:
        VectorString(uint n, const vector<unsigned int> qIDs) : n(n), qIDs(qIDs),
                                                                              amps(vector<complex<double> >(pow(2, n))) {
            amps.at(0) = 1.0;
            pnc = stateTP(n);
            probs = getProbabilities();
            random_device rd;
            gen = mt19937(rd());
        }
        unsigned int nthCleared(const unsigned int n, const unsigned int t) {
            /* It finds the nth integer whose
             * tth bit is zero, where t starts at 0.
             * Comment: For this function to work correctly,
             * n must be greater than t.
             * Comment: The search starts by binary number 0x01.
             */
            // In binary, holder will be all ones up to position t (not included)
            unsigned int holder = (1<<t)-1;
            // In binary, inverter will be all zeros up to position t (not included)
            unsigned int inverter = ~holder;
            // n & holder: get first t bits of n
            // (n & inverter) << 1: get the rest of bits of n and shift left by one
            // (n & holder)+2*(n&inverter)
            return (n & holder) | ((n & inverter) << 1u);
        }
        unsigned int numberOfQubits() { return n; }
        VectorString joinStateWith(VectorString other) {
            // Refactor in a clean manner
            vector<complex<double> > newAmp(getSize() + other.getSize());
            for (unsigned int i = 0; i < getSize(); i++)
                for (unsigned int j = 0; j < other.getSize(); j++)
                    newAmp.at(i * other.getSize() + j) =
                            amps.at(i) * other.amps.at(j); //VERIFY THE INDEX
            vector<uint> newQIDs;
            newQIDs.insert(newQIDs.end(), qIDs.begin(), qIDs.end());
            newQIDs.insert(newQIDs.end(), other.qubitIDs().begin(), other.qubitIDs().end());
            VectorString combined (n+other.n, newQIDs);
            combined.setAmplitudes(newAmp);
            return combined;
        }
        string measure() {
            string m = pnc.at(d(gen));
            return m;
        }
        int getSize() {
            return pow(2,numberOfQubits());
        }
        vector<unsigned int> qubitIDs() { return qIDs; }
        void singleGate(unsigned int qID, Gate &gate){
            // Refactor
            unsigned int qIndex = find(qIDs.begin(), qIDs.end(), qID)-qIDs.begin();
            for(unsigned int i = 0; i < pow(2,n-1); i++) {
                unsigned int cState = nthCleared(i, qIndex);
                unsigned int tState = cState | (1u << qIndex);
                complex<double> cAmp = amps.at(cState);
                complex<double> tAmp = amps.at(tState);
                amps.at(cState) = gate.at(0)*cAmp + gate.at(1)*tAmp;
                amps.at(tState) = gate.at(2)*cAmp + gate.at(3)*tAmp;
            }
            probs = getProbabilities(); // Remember to update probs each time amps is updated
        }
        void setAmplitudes(const vector<complex<double> > &newAmplitudes) {
            amps = newAmplitudes;
            probs = getProbabilities(); // Remember to update probs each time amps is updated
        }
        void applyCnGate(unsigned int control, unsigned int qID, Gate &gate) {
            // Refactor
            unsigned int qIndex = find(qIDs.begin(), qIDs.end(), qID)-qIDs.begin();
            for(int i = 0; i < pow(2,n-1); i++) {
                unsigned int cState = nthCleared(i, qIndex);
                unsigned int tState = cState | (1u << qIndex);
                complex<double> cAmp = amps.at(cState);
                complex<double> tAmp = amps.at(tState);
                if (((1u << control) & cState) > 0)
                    amps.at(cState) = gate.at(0)*cAmp + gate.at(1)*tAmp;
                if(((1u << control) & tState) > 0)
                    amps.at(tState) = gate.at(2)*cAmp + gate.at(3)*tAmp;
            }
            probs = getProbabilities(); // Remember to update probs each time amps is updated
        }
        vector<double> getProbabilities() {
            vector<double> probs((unsigned int) pow(2,n));
            transform(amps.begin(), amps.end(), probs.begin(), [](complex<double> &d) { return
                    norm(d); });
            d = discrete_distribution<unsigned int>(probs.begin(), probs.end());
            return probs;
        }
        friend ostream &operator<<(ostream &os, const VectorString &vs);
    private:
        unsigned int n;
        vector<unsigned int> qIDs;
        vector<complex<double> > amps;
        mt19937 gen;
        vector<string> pnc;
        vector<double> probs;
        discrete_distribution<unsigned int> d;
    };
    ostream &operator<<(ostream &os, const VectorString &vs) {
        for (auto &amp : vs.amps) {
            cout << amp;
        }
        return os;
    }
}

#endif //FINALPROD_VECTORSTRING_H
