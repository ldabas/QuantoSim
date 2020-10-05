#include <iostream>
#include <random>
#include <QuantoSim.hpp>
#include <map>

using namespace std;

int main() {
    cout << "[QUANTOSIM] Creating Quantum register..." << endl;
    QuantoSim::QRegister qr(4); // Create quantum register with 4 qubits
    //cout << qr.getVectorString(0) << endl; // Print first qubit amplitudes
    qr.applySingleGate(0, QuantoSim::Gates::H); // Apply Haddamar to first qubit
    //qr.applySingleGate(0, QuantoSim::Gates::Rm(10)); // Apply R Gate to first qubit
    qr.applyCnGate(1,0, QuantoSim::Gates::X); // Apply not gate to first qubit
    // Show probabilities of the Quantum Register model
    auto qrProbabilities = qr.getProbabilities();
    for(auto m = qrProbabilities.begin(); m != qrProbabilities.end(); m++) {
        cout << "Amplitudes: {" << *m->first << "}, Probability for qubit: {";
        for(double &k : m->second) {
            cout << k;
            if (&k != &m->second.back()) cout << ",";
        }
        cout << "}" << endl;
    }
    // Run simulation
    cout << "[QUANTOSIM] Running simulation..." << endl;
    QuantoSim::Simulation simulation(qr);
    simulation.run(100000000, true);
    return 0;
}

