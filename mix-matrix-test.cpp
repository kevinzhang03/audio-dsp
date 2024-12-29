#include "mix-matrix.h"

#include <iostream>
#include <string>
#include <array>
#include <cstdlib>
#include <ctime>

int main() {
    const bool RAND = 1;
    const int MIX = 0; // 0 for no mix, 1 for householder, 2 for hadamard
    const size_t N = 16;
    using Array = std::array<double, N>;
    
    // init array all 0.00
    Array array;
    for (size_t i = 0; i < N; ++i) {
        array[i] = 0.0;
    }

    srand(time(nullptr));

    if (RAND) {
        // randomly populate the array with values from 0 to 1, with two decimal points
        for (size_t i = 0; i < N; ++i) {
            array[i] = (rand() % 100) / 100.0;
        }
    } else {
        // place 1.00 in first element and rest 0.00
        array[0] = 1.0;
    }

    if (MIX == 1) {
        Householder<double, N>::inPlace(array.data());
    } else if (MIX == 2) {
        Hadamard<double, N>::inPlace(array.data());
    }

    // print the array
    std::cout << "array:" << std::endl << "[ ";
    for (size_t i = 0; i < N; ++i) {
        std::cout << array[i];
        if (i < N - 1) std::cout << ", ";
    }
    std::cout << " ]" << std::endl;

    return 0;
}