#include <iostream>
#include <cmath>
#include "types_of_data.h"
using namespace std;

int main() {
    Triangle triangle(1,1, 2,4, 4,2);
    Triangle triangle1(2,1, 3,4, 5,2);
    Triangle triangle2(1,1, 5,4, 4,2);
    Vector vec{};

    vec.a = 4;
    vec.b = 8;
    cout << (triangle==triangle1) << "\n";
    cout << (triangle!=triangle1) << "\n";
    cout << (triangle>triangle1) << "\n";
    cout << (triangle<triangle1) << "\n";

    cout << "second test part" << "\n";

    cout << (triangle==triangle2) << "\n";
    cout << (triangle!=triangle2) << "\n";
    cout << (triangle<triangle2) << "\n";
    cout << (triangle>triangle2) << "\n";


    return 0;
}


