//
// Created by Lakshit Dabas on 2/25/20.
//

#ifndef FINALPROD_QREGISTER_H
#define FINALPROD_QREGISTER_H

#include "Base.hpp"
#include "VectorString.hpp"
#include "Gates.hpp"

namespace QuantoSim {
    class QRegister {
    public:
        QRegister(unsigned int n) : n(n) {
            for(unsigned int i = 0; i < n; i++)
                vectors.push_back(VectorString(1, {i}));
        }
        unsigned int numberOfQubits() {
            return n;
        }
        void applySingleGate(unsigned int qID, Gate &gate) {
            vectors[getID(qID)].singleGate(qID, gate);
        }
        void applyCnGate(unsigned int control, unsigned int qID, Gate &gate) {
            if (control) vectors[getID(qID)].applyCnGate(control, qID, gate);
        }
        unsigned int combineTwoQubits(unsigned int q1ID, unsigned int q2ID) {
            // TODO: Refactor
            auto q1Index = getID(q1ID);
            auto q2Index = getID(q2ID);
            // if already combined, do nothing
            if(q1Index == q2Index) return q1Index;
            VectorString combinedState = vectors[q1Index].joinStateWith(vectors[q2Index]);
            unsigned int indexOfCombinedState = min(q1Index, q2Index);
            vectors.at(indexOfCombinedState) = combinedState;
            vectors.erase(vectors.begin() + max(q1Index, q2Index));
            return indexOfCombinedState;
        }
        void combineAllQubits() {
            for(int i = 1; i < n; i++) combineTwoQubits(i-1, i);
        }
        unsigned int getID(unsigned int qID) {
            for(uint i = 0; i < vectors.size(); i++) {
                vector<uint> qubitIDs = vectors[i].qubitIDs();
                if (find(qubitIDs.begin(),qubitIDs.end(), qID)!=qubitIDs.end()) return i;
            }
            throw exception(); // Should not arrive here
        }
        string measure() {
            // TODO: Refactor
            string result(n, '-');
            for(auto &v: vectors) {
                auto decomposed = v.measure();
                auto qIDs = v.qubitIDs();
                for(int i = 0; i < decomposed.length(); i++) {
                    auto qID = qIDs.at(i);
                    result.at(qID) = decomposed.at(i);
                }
            }
            return result;
        }
        map<VectorString *, vector<double> > getProbabilities() {
            // This function returns a map of VectorString states
            // and probabilities
            map<VectorString *, vector<double> > result;
            for (auto &v : vectors)
                result.insert({&v,v.getProbabilities()});
            return result;
        }
        string ketMeasure() { return "|" + measure() + ">"; }
        vector<unsigned int> allQubits() {
            vector<unsigned int> result(n);
            iota(result.begin(), result.end(), 0); //0, 1, ..., n-1
            return result;
        }
        vector<VectorString> getVectorStrings() { return vectors; }
        VectorString getVectorString(const unsigned int qID) {
            unsigned int qubitIndex = getID(qID);
            return vectors.at(qubitIndex);
        }
        // Fourier functions: This code needs reviewing

        //FUCK FUCK FUCK FUCK FUCK FUCK FUCK
        void QuantumFourierTransform(vector<unsigned int> qIDs) {
            for(int qID = 0; qID < qIDs.size(); qID++) {
                vectors[getID(qID)].singleGate(qID, Gates::H);
                for(int otherqID = qID+1; otherqID < qIDs.size(); otherqID++) {
                    applyCnGate(getID(otherqID), getID(qID), Gates::Rm(otherqID-qID+1));
                }
            }
        }
        void InverseQuantumFourierTransform(vector<unsigned int> qIDs) {
            for(int qID = qIDs.size() - 1; qID >= 0; qID--) {
                for(int otherqID = qIDs.size() - 1; otherqID > qID; otherqID--) {
                    applyCnGate(getID(otherqID), getID(qID), Gates::Rm(-(otherqID-qID+1)));
                }
                applySingleGate(getID(qID), Gates::H);
            }
        }

                //Entangling function
        void EntangleQubits(unsigned int qID1, unsigned int qID2) {
            applySingleGate(qID1, Gates::H);
            applyCnGate(qID1,qID2,Gates::X);
        }
    private:
        unsigned int n;
        vector<VectorString> vectors;
    };
}



#endif //FINALPROD_QREGISTER_H
