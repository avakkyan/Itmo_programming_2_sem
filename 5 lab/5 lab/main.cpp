#include <iostream>
#include "stack.cpp"
using namespace std;


int main() {
    int a[] {1, 2, 3, -1, -2};
    cout << maximum_in_array<int, 5>(a) << endl;
    double b[] {1., 2.5, 3.1, 3.2,-1., -2.};
    cout << maximum_in_array<double, 6>(b) << endl;
    string c[] {"a", "b", "cc", "c", "d"};
    cout << maximum_in_array<string, 5>(c) << endl;

    MyStack <int, 3> myStack;
    myStack.push(4);
    myStack.push(5);
    myStack.push(6);
    //myStack.push(7); //exception 1
    myStack.take();
    myStack.take();
    myStack.take();
    myStack.take(); // exception 2

    return 0;
}

