#include <iostream>
using namespace std;

void helperFunction() {
    cout << "Inside Helper Function (CALL & RET worked!)" << endl;
}

int main() {
    int a = 10;
    int b = 20;

    cout << "--- 1. CMP & CPU Flags Demonstration ---" << endl;
    
    if (a < b) {
        cout << "a is less than b (Signed/Unsigned Comparison matched)" << endl;
    }

    cout << "\n--- 2. CALL & RET Demonstration ---" << endl;
    helperFunction();

    return 0;
}