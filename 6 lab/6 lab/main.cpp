#include <iostream>
#include <vector>
#include "string"
#include "sample.cpp"
using namespace std;

struct CPoint{
    int x,y;
};

int main() {
    CPoint a{1,4};
    CPoint b{3,8};
    CPoint c{5,5};
    vector <CPoint> v;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    cout << "All of x are > 0: " << all_of(v.begin(),v.end(),[](const CPoint& point){return point.x > 0;})<<endl;

    vector <int> v1;
    v1.push_back(1);
    v1.push_back(56);
    v1.push_back(23);
    v1.push_back(65);
    cout << "Is Sorted: " << is_sorted(v1.begin(), v1.end(), [](int n1, int n2){return n1 <= n2;}) << endl;

    vector <string> v2;
    v2.push_back("aabbaa");
    v2.push_back("Karina");
    v2.push_back("poop");
    for (int i=0; i<v2.size(); i++){
        int j=i + 1;
        cout << j << ')'<< is_palindrome(v2[i].begin(),v2[i].end())<<endl;
    }
    return 0;
}
