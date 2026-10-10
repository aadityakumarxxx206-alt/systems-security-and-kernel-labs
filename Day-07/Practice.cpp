#include <iostream>

int addNumbers(int a, int b) {
    int result = a + b;
    return result;       
}

int main() {
    int x = 10;
    int y = 20;

    int sum = addNumbers(x, y);

    if (sum > 25) {
        std::cout << "Sum is greater than 25 (JG / JNE matched)" << std::endl;
    } else {
        std::cout << "Sum is less than or equal to 25 (JL / JE matched)" << std::endl;
    }

    int val = 0;
    while (val < 1) {
        val++;
    }

    return 0;
}