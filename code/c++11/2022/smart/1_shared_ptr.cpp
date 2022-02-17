#include <iostream>
#include <memory>
using namespace std;

class Parent;
typedef std::shared_ptr<Parent> ParentPtr;

class Child
{
public:
    ParentPtr father;
    Child()
    {
        cout << "hello Child" << endl;
    }
    ~Child()
    {
        cout << "bye Child\n";
    }
};

typedef std::shared_ptr<Child> ChildPtr;

class Parent
{
public:
    ChildPtr son;
    Parent()
    {
        cout << "hello parent\n";
    }
    ~Parent()
    {
        cout << "bye Parent\n";
    }
};

void testParentAndChild()
{
    ParentPtr p(new Parent());
    ChildPtr c(new Child());
    p->son = c;
    c->father = p;
}

int main()
{
    testParentAndChild();
    return 0;
}
