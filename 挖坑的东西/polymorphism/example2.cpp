#include<iostream>
using namespace std;
class Base{
public:
    Base() : val_Base(0) {cout<<"**Base default construction**"<<endl;}
    //Base(int x): val_Base(x) {cout<<"**Base construction**"<<endl;}
    int val_Base;
};
class A : public Base{
public:
    A() : val_A(0) {cout<<"**A default construction**"<<endl;}
    int val_A;
};
int main()
{
    A a;
    return 0;
}