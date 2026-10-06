#include <iostream>

int addNumbers(int a, int b) {
    int result = a + b; 
    return result;
}

int main() {
    int x = 10;
    int y = 20;
    
    int sum = addNumbers(x, y); 
    
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}