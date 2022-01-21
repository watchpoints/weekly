#include <iostream>
using namespace std;
class A{
public:
    ~A(){
        cout<<"Destructor"<<endl;
    }
};
void fun(A & ra){
}
int main(){   
    A a;
    fun(a);
    return 0;
}
