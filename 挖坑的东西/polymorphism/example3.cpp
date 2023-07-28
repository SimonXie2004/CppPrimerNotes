#include<iostream>
using namespace std;
class Base{
public:
    virtual void foo() {cout<<"Base"<<endl;}
};
class Derived : public Base{
public:
    virtual void foo() {cout<<"Derived"<<endl;}
};
int main(){
    Base *p = new Base;
    p->foo();//Base
    delete p;
    p = new Derived;
    p->foo();//Derived
    delete p;
}