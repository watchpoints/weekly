#include <iostream>
#include <cassert>
#include <memory>
using namespace std;
 
class Parent;
typedef std::shared_ptr<Parent> ParentPtr;
typedef std::weak_ptr<Parent> WeakParentPtr;

class Child
{
public:
    WeakParentPtr father;                 // 只有一环换成 weak_ptr, 即可打破环 
    Child() {
        cout << "hello Child" << endl;
    }
    ~Child() {
        cout << "bye Child\n";
    }
};
 
typedef std::shared_ptr<Child> ChildPtr;
typedef std::weak_ptr<Child> WeakChildPtr;

class Parent {
public:
    ChildPtr son;                 
    Parent() {
        cout << "hello parent\n";
    }
    ~Parent() {
        cout << "bye Parent\n";
    }
};


void testParentAndChild()
{
    ParentPtr p(new Parent());
    ChildPtr c(new Child());
    p->son = c;            
    c->father = p;        
    cout << (c->father).use_count() << endl;
    cout << (p->son).use_count() << endl;
}

int main()
{
    testParentAndChild();
    return 0;
}
