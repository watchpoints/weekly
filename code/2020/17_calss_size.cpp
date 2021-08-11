#include <string>
#include <iostream>

using namespace std;

class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};

int main(int argc, char** argv) {

cout << "x="<<sizeof(X)<<endl;
cout << "y="<<sizeof(Y)<<endl;
cout << "z="<<sizeof(Z)<<endl;
cout << "a=="<<sizeof(A)<<endl;

}
