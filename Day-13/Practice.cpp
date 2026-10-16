#include <iostream>
using namespace std;

int main() {
    int health = 100;
    int damage = 25;

    cout << "Initial Health: " << health << endl;

    health = health - damage;

    cout << "Damage Taken: " << damage << endl;
    cout << "Remaining Health: " << health << endl;

    health = health + 10;

    cout << "Health After Recovery: " << health << endl;

    return 0;
}