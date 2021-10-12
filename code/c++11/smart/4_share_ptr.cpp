#include <iostream>
#include <cassert>
#include <memory>
using namespace std;
 
class Child;
typedef std::shared_ptr<Child> childPtr;

class Child
{
public:
    childPtr m_ptr;                 
    Child() {
        cout << "hello Child" << endl;
    }
    ~Child() {
        cout << "bye Child\n";
    }
};
 


int main()
{
    
    childPtr sp(new Child());
    sp->m_ptr =sp;
    cout << sp.use_count() << endl;//2
    
    return 0;
}
