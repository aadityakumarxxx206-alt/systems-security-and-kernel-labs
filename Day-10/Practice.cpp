#include <iostream>

void myTestFunction() {
    int local_var = 7; 
    
    if (local_var == 7) {
        std::cout << "Equal to 7" << std::endl;
    } else {
        std::cout << "Not Equal" << std::endl;
    }
}

int main() {
    myTestFunction();
    return 0;
}