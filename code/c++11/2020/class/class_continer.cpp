<<<<<<< HEAD
#include <iostream>
#include <vector>
using namespace std;

void testVector(){

	vector<int> array(10);//create an empty vector  
	cout<<" size="<<array.size() << " cap=" <<array.capacity()<<endl; // size=10 cap=10
	array.push_back(10);
	cout<<" size="<<array.size() << " cap=" <<array.capacity()<<endl; // size=10 cap=10

}
//g++ -std=c++11 class_continer.cpp
int main() 
{
	testVector();
=======
#include <iostream>
#include <vector>
using namespace std;

void testVector(){

	vector<int> array(10);//create an empty vector  
	cout<<" size="<<array.size() << " cap=" <<array.capacity()<<endl; // size=10 cap=10
	array.push_back(10);
	cout<<" size="<<array.size() << " cap=" <<array.capacity()<<endl; // size=10 cap=10

}
//g++ -std=c++11 class_continer.cpp
int main() 
{
	testVector();
>>>>>>> 55d65b891bace52a834abb8e74e3ab2ec75d63f6
}