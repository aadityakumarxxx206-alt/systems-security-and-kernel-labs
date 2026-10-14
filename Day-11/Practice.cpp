#include <iostream>

// A simple function for CALL and RET practical
int checkValueFunction(int val) {
    // Here RET (Return) will work when the function finishes
    return val * 2; 
}

int main() {
    std::cout << "--- Assembly Concepts Practical ---" << std::endl;

    // 1. CMP (Compare) and JNE (Jump if Not Equal) practical
    int local_var = 7; // Works like [rbp-4]
    
    // Suppose we are doing CMP: cmp dword ptr [rbp-4], 7
    if (local_var == 7) {
        // Here internal subtraction 7 - 7 = 0 happens, making ZF = 1
        std::cout << "CMP Result: Match found! (Zero Flag ZF = 1)" << std::endl;
    }

    // JNE logic (Jump if Not Equal, runs when ZF = 0)
    if (local_var != 7) {
        std::cout << "JNE Jump Taken: No Match!" << std::endl;
    } else {
        std::cout << "JNE Jump Skipped because ZF = 1." << std::endl;
    }

    // 2. Unconditional JMP practical (Skipping intermediate code)
    goto skip_block; // Works like Assembly's JMP instruction

    std::cout << "THIS WILL NOT PRINT because JMP skipped it!" << std::endl;

skip_block:
    std::cout << "Unconditional JMP executed: Skipped the middle block." << std::endl;

    // 3. TEST Instruction practical (Register Zero Check)
    int rdx = 0; // Suppose this is RDX register
    // In assembly it is 'test rdx, rdx' and if rdx == 0, ZF = 1 and JE works
    if (rdx == 0) {
        std::cout << "TEST RDX, RDX: RDX is 0, ZF = 1, JE (Jump if Equal) triggered." << std::endl;
    } else {
        std::cout << "TEST RDX, RDX: RDX is NOT 0, ZF = 0." << std::endl;
    }

    // 4. CMP + JG (Signed Comparison) practical
    int rax = 10;
    // In assembly it is cmp rax, 0Bh (11) followed by JG (Jump if Greater)
    if (rax > 11) {
        std::cout << "CMP + JG: RAX is greater than 11 (Signed condition met)." << std::endl;
    } else {
        std::cout << "CMP + JG: RAX is NOT greater than 11 (ZF = 0, SF = OF conditions)." << std::endl;
    }

    // 5. CALL and RET practical
    std::cout << "Calling function (CALL instruction will save return address on stack)..." << std::endl;
    int result = checkValueFunction(local_var); // CALL
    std::cout << "Function returned result: " << result << " (RET instruction brought us back)." << std::endl;

    return 0;
}