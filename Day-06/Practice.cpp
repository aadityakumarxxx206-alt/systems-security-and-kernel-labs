#include <stdio.h>
#include <stdint.h>

// 1. Structure definition (base + offset)
struct Student {
    uint8_t id;         // Byte (1 byte)
    uint16_t code;      // Word (2 bytes)
    int32_t score;      // Dword (4 bytes)
    int64_t total;      // Qword (8 bytes)
};

// 2. Calling Convention & Instruction Testing Function
// 4 Arguments: 1st->RCX, 2nd->RDX, 3rd->R8, 4th->R9
int64_t process_data(uint8_t small_val, int8_t signed_val, struct Student* student_ptr, int64_t extra_val) {
    // Local stack variables ([RBP - offset])
    int32_t local_var1 = 10;
    int32_t local_var2 = 20;

    // Zero Extension (MOVZX) test
    uint32_t z_ext = (uint32_t)small_val;

    // Sign Extension (MOVSX) test
    int32_t s_ext = (int32_t)signed_val;

    // Pointer access & Pointer Chain ([base + offset])
    int32_t s_score = student_ptr->score;

    // Return value in RAX
    return z_ext + s_ext + s_score + extra_val + local_var1 + local_var2;
}

int main() {
    struct Student s1;
    s1.id = 0x05;
    s1.code = 0x1234;
    s1.score = 100;
    s1.total = 5000;

    // Function call with 4 arguments
    int64_t result = process_data(s1.id, -5, &s1, 200);

    printf("Result: %lld\n", result);
    return 0;
}