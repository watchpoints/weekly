#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

int main ()
{
    string a = "0123456789abcdef" ;
    string b = a ;
    cout << "a.data() =" << (void *)a. data() << endl ;
    cout << "b.data() =" << (void *)b. data() << endl ;
    assert( a.data () == b. data());
    cout << endl;

    string c = a ;
    cout << "a.data() =" << (void *)a. data() << endl ;
    cout << "b.data() =" << (void *)b. data() << endl ;
    cout << "c.data() =" << (void *)c. data() << endl ;
    assert( a.data () == c. data());

    cout << endl;
    c[0] = '1';
    cout << "after write:\n";
    cout << "a.data() =" << (void *)a. data() << endl ;
    cout << "b.data() =" << (void *)b. data() << endl ;
    cout << "c.data() =" << (void *)c. data() << endl ;
    assert( a.data () != c. data() && a .data() == b.data ());
    return 0;
}
