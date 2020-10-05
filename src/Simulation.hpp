#ifndef QUANTOSIM2_SIMULATION_HPP
#define QUANTOSIM2_SIMULATION_HPP

#include "Base.hpp"
#include "QRegister.hpp"

namespace QuantoSim {
    class Simulation {
        // This class contains QRegister simulations
    public:
        Simulation(QRegister &reg) : reg(&reg) {}
        AbsoluteFrequency run(unsigned int count, bool output = true) {
            //Generate random numbers before the simulation
            //randoms = generateRandomNumbers();
            AbsoluteFrequency measurements;
            //Profile to see where the computational resources are being consumed the most
            #pragma acc parallel loop
            for(int i = 0; i < count; i++) {
                string measurement = reg->ketMeasure();
                if (measurements.find(measurement) == measurements.end())
                    measurements.insert({measurement,1});
                else measurements.at(measurement)++;
            }
            if (output) {
                string results;
                cout << "{";
                for (auto &m: measurements) {
                    results += "\"" + m.first + "\": " + to_string(m.second);
                    if (m != *measurements.rbegin()) results += ",";
                }
                cout << results << "}";
            }
            return measurements;
        }
    private:
        /*vector<unsigned int> generateRandomNumbers() {

        }*/
        QRegister *reg;

    };
}

#endif //QUANTOSIM2_SIMULATION_HPP
