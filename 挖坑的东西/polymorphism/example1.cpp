#include <iostream>
using std::cout, std::endl;
class Base{
public:
    Base() {}
private:
    Base(const Base&);
    Base& operator=(const Base&);
};
class A : public Base{
private:
    int val;
};
class B {
private:
    int val;
    Base b;
};
int main()
{
    A a;
    B b;
    //A a1=a;
    //B b1=b;
    cout<<sizeof(a)<<endl;
    cout<<sizeof(b)<<endl;
    return 0;
}