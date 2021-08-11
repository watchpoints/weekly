#include <iostream>
using namespace std;
class base
{
public: 
                 ~base()
                //virtual ~base()
		{
 
		}
		//virtual void print()
		 void print()
		{
			cout<<"we are in base"<<endl;
		}
protected:
private:
};
class derived : public base
{
public:
	virtual void  print()
	{
		cout <<"we are in derived"<<endl;
	}
protected:
private:
};
int main()
{
	base *base_ptr1 = new base;
	derived *derived_ptr1 =  dynamic_cast<derived*>(base_ptr1);   //base_ptr1实际指向base，不能转化为derived
	base *base_ptr2 = new derived;
	derived *derived_ptr2 = dynamic_cast<derived*>(base_ptr2);   //base_ptr1实际指向derived，不能转化为derived
	//derived *derived_ptr2 = new derived;
	//base *base_ptr2 = dynamic_cast<base*>(derived_ptr2);
	if (derived_ptr1 == NULL)
	{
		cout <<"base pointer pointed to base can be dynamic_cast to derived pointer  is wrong"<<endl;
	} 
	else
	{
		cout<<"pointer to base can be dynamic_cast to derived pointer  is success"<<endl;
	}
    
	if (derived_ptr2 == NULL)
	{
		cout <<"base pointer pointed to derived can be dynamic_cast to derived pointer  is wrong"<<endl;
	} 
	else
	{
		cout<<"base pointer pointed to derived can be dynamic_cast to derived pointer  is success"<<endl;
	}
	return 0;
}
