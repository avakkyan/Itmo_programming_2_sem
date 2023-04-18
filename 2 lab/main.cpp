#include <iostream>
#include <cmath>
#include "Complex.h"
using namespace std;



int main() {
    Complex complex1 {2,5};
    Complex complex2 {4,7};
    Complex complex3;
    int a = 10;
    cout << "Complex numbers: " << endl;
    complex1.print_c();
    complex2.print_c();
    cout << "\n";
    complex2.mult_by_num(complex2, a);
    cout << "Multiplying a complex number by a real number: ";
    complex2.print_c();
    complex3.add_of_comp_num(complex1,complex2);
    cout << "Addiction of two complex numbers: ";
    complex3.print_c();
    complex3.mult_by_2_comp_num(complex1, complex2);
    cout << "Multiplication of two complex numbers: ";
    complex3.print_c();
    cout << "Length of the second number: " << complex2.length_of_comp_num(complex2) << endl;
}
