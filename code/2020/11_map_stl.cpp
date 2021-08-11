#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <string>
using namespace std;
class Person
{
    public:
        Person(string strName, int iAge):m_strName(strName),m_iAge(iAge)
        {}  
        string m_strName;
        int m_iAge;
};
struct ptr_less : public binary_function<Person,Person,bool>
{
    bool operator()(const Person& a, const Person& b) const
    {   
        if(a.m_iAge > b.m_iAge)
            return false;
        else return true;
    }   
};
struct ptr_print :public unary_function< const pair<Person,int>&,void >
{
    void operator()(const pair<Person,int>& a)
    {   
        printf("Name is %20s,Age is %3d, salary is %10d\n",a.first.m_strName.c_str(), a.first.m_iAge,a.second);
    }   
};
int main()
{
    Person p1("hepeng",26);
    Person p2("zhangling",27);
    Person p3("lisi",15);
    map<Person,int, ptr_less> test;//if ptr_less is not specified, map will use pointer compare
   // map<Person,int> test;//if ptr_less is not specified, map will use pointer compare
    test.insert(pair<Person,int>(p1,15000));
    test.insert(pair<Person,int>(p2,25000));
    test.insert(pair<Person,int>(p3,5000));
    for_each(test.begin(),test.end(),ptr_print());
    return 0;
}
