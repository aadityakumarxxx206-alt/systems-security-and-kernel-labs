
#include <iostream>
using namespace std;

int add_values(int a, int b) {
    int result = a + b;
    return result;
}

int subtract_values(int a, int b) {
    int result = a - b;
    return result;
}

int main() {
    int a = 42;
    int b = 17;

    int sum = add_values(a, b);
    int difference = subtract_values(a, b);

    int bit_and = a & 15;
    int bit_or  = a | 3;
    int bit_xor = a ^ 7;
    int shifted_left = a << 1;
    int shifted_right = a >> 1;

    int comparison = 0;

    if (difference > 0) {
        comparison = 1;
    } else {
        comparison = -1;
    }

    int zero_test = (a & 0);

    if (zero_test == 0) {
        comparison = comparison + 10;
    }

    cout << "a = " << a << '\n';
    cout << "b = " << b << '\n';
    cout << "sum = " << sum << '\n';
    cout << "difference = " << difference << '\n';
    cout << "AND = " << bit_and << '\n';
    cout << "OR = " << bit_or << '\n';
    cout << "XOR = " << bit_xor << '\n';
    cout << "Left shift = " << shifted_left << '\n';
    cout << "Right shift = " << shifted_right << '\n';
    cout << "Comparison result = " << comparison << '\n';

    return 0;
}
