#include <iostream>
#include "complex.h"
#include "bias.h"
#include "round_m.h"
#include "swap_sw.h"

using namespace std;

int main() {
    // Task 1
    int a, b;
    int x, y;
    cin >> a >> b;
    cin >> x >> y;
    swap_m(&a, &b);
    swap_m(x , y);
    cout << a << ' ' << b << '\n';
    cout << x << ' ' << y << '\n';


    // Task 4
    float num1, num2;
    cin >> num1 >> num2;
    round_m(num1);
    round_m(&num2);
    cout << num1 << ' ' << num2 << '\n';

    // Task 9
    complex_number number1;
    complex_number number2;
    float fst, sec, c;
    cin >> fst >> sec >> c;
    number1.imaginary_part = sec;
    number1.real_part = fst;
    number2.imaginary_part = sec;
    number2.real_part = fst;
    complex(number1, c);
    complex(&number2, &c);
    cout << number1.real_part << " " << number1.imaginary_part << "\n";
    cout << number2.real_part << " " << number2.imaginary_part << "\n";

    //Task 14
    circle circle1;
    circle circle2;
    my_vector vec1;
    my_vector vec2;
    cin >> circle1.center_x >> circle1.center_y >> circle1.radius >> vec1.v_x >> vec1.v_y;
    circle2.center_x = circle1.center_x;
    circle2.center_y = circle1.center_y;
    circle2.radius = circle1.radius;

    vec2.v_y = vec1.v_y;
    vec2.v_x = vec1.v_x;

    bias(circle1, vec1);
    bias(&circle2, &vec2);

    cout << circle1.center_x << " " <<  circle1.center_y << " " << circle1.radius << "\n";
    cout << circle2.center_x << " " <<  circle2.center_y << " " << circle2.radius << "\n";
    return 0;
}