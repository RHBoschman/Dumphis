#include <iostream>
#include <cstdlib>

int main(void) {
    std::cout << "Open webpage";
    if (system("start index.html") != 0) {
        std::cout << "\nError while opening webpage";
    }
    return 0;
}