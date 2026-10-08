#include <iostream>

// x64 / x86 Architecture Practice Code
int main() {
    // 1. Bitwise Operations & Flag Checks
    int a = 10;                     // Value
    int b = 0;                      // Zero check
    
    // 2. Memory Addressing Formula Vectors
    int arr[5] = {100, 200, 300, 400, 500}; 
    int* ptr = arr;                 // Pointer to memory

    // Logic Execution for Assembly Inspection
    if (b == 0) {                   // Will trigger ZF and JE/JZ
        a = a ^ a;                  // XOR EAX, EAX equivalent (Zero out)
    }

    a = 15;                         // 0x0F
    a = a & 0x0F;                   // AND bitmask
    a = a << 2;                     // SHL (Shift Left)
    a = a >> 1;                     // SHR (Shift Right)

    // Memory Access: [BASE + INDEX * SCALE + OFFSET]
    // [ptr + 2 * 4 + 0] -> arr[2] = 300
    int val = *(ptr + 2);

    return 0;
}