#include <iostream>
#include <random>
#include <QuantoSim.hpp>
#include <map>

using namespace std;

int main() {
    cout << "[QUANTOSIM] Creating Quantum register..." << endl;
    QuantoSim::QRegister qr(4); // Create quantum register with 4 qubits
    qr.applySingleGate(0, QuantoSim::Gates::H); // Apply Haddamar to first qubit
    qr.QuantumFourierTransform({0, 1, 2});
    // Run simulation
    cout << "[QUANTOSIM] Running simulation..." << endl;
    QuantoSim::Simulation simulation(qr);
    simulation.run(10000, true);
    return 0;
}

