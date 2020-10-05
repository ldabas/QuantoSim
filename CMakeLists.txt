cmake_minimum_required(VERSION 3.16)
project(QuantoSim2)
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_STANDARD 17)

# OpenACC Parellization
set(USE_OPENACC ON) # Enable OpenACC
set(USE_OPENACC_GPU OFF) 
set(ACC_ROOT_DIR /opt/pgi/linux86-64-llvm/2019) # Root dir, where bin and REDIST folders are

if(${USE_OPENACC})
    set(CMAKE_CXX_COMPILER ${ACC_ROOT_DIR}/bin/pgc++)
endif()

add_library(QuantoSim STATIC src/Base.hpp src/Gates.hpp src/QRegister.hpp
        src/VectorString.hpp src/Simulation.hpp)
set_target_properties(QuantoSim PROPERTIES LINKER_LANGUAGE CXX)
add_executable(TestSimulation tests/test_simulation.cpp)
target_include_directories(TestSimulation PUBLIC src)
if(${USE_OPENACC})
    if(${USE_OPENACC_GPU})
        target_compile_options(TestSimulation PRIVATE -fast -acc -ta=tesla -O3) # Could add -Minfo for debugging
        target_link_libraries(TestSimulation QuantoSim accg accg2 cudadevice)
    else()
        target_compile_options(TestSimulation PRIVATE -fast -acc -ta=multicore -O3) # Could add -Minfo for debugging
        target_link_libraries(TestSimulation  accg accg2 )
    endif()
    target_link_directories(TestSimulation PUBLIC ${ACC_ROOT_DIR}/REDIST)
else()
    target_link_libraries(TestSimulation QuantoSim)
endif()
add_executable(TestQuantumFourierTransform tests/test_quantum_fourier_transform.cpp)
target_include_directories(TestQuantumFourierTransform PUBLIC src)
target_link_libraries(TestQuantumFourierTransform QuantoSim)