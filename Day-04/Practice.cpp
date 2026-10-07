#include <iostream>

int main() {
    int a = 10;
    int b = 20;
    unsigned int x = 50;
    unsigned int y = 30;

    // 1. Equal Comparison (CMP + ZF + JE/JNE)
    if (a == 10) {
        std::cout << "Equal Check Passed\n";
    }

    // 2. Signed Comparison (CMP + SF/OF + JG/JL)
    if (a < b) { // 10 < 20
        std::cout << "Signed: A is less than B\n";
    }

    // 3. Unsigned Comparison (CMP + CF + JA/JB)
    if (x > y) { // 50 > 30
        std::cout << "Unsigned: X is Above Y\n";
    }

    // 4. Unconditional Jump (JMP)
    goto end_label;
    std::cout << "Skipped Code\n";

end_label:
    std::cout << "Final Destination\n";

    return 0;
}