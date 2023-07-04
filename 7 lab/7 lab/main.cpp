#include <iostream>
#include "iterator.cpp"
#include <algorithm>
using namespace std;

int main(){
    Circle_buf <int> buf(4);


    buf.push_back(4);
    buf.push_back(6);
    buf.push_back(10);
    buf.push_back(3);

    for(auto t:buf)
        cout << t << " ";
    cout << endl;

    sort(buf.begin(), buf.end());
    for(auto t:buf)
        cout << t << " ";
    cout << endl;


    buf.pop_back();

    for(auto t:buf)
        cout << t << " ";
    cout << endl;

    buf.pop_front();
    for(auto t:buf)
        cout << t << " ";
    cout << endl;


    buf.push_back(10);
    for(auto t:buf)
        cout << t << " ";
    cout << endl;

}
