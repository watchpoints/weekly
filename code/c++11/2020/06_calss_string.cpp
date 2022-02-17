#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;


string test()
{

  string temp("helloworld");
 // cout<<"temp"<<temp.c_str()<<endl;
  return temp;
}
int main()
{

    string str1 = "hello world";
    string str2 = str1;
    string temp;
    cout<<"sizeof str="<<sizeof(temp)<<endl;
    cout<<"sizeof str="<<sizeof(str1)<<endl;
    printf ("Sharing the memory:\n");
    printf ("/tstr1's address: %x \n", str1.c_str() );
    printf ("/tstr2's address: %x \n", str2.c_str() );
   
    str1[1]='q';
    str2[1]='w';
    
    printf ("After Copy-On-Write:\n");
    printf ("/tstr1's address: %x \n", str1.c_str() );
    printf ("/tstr2's address: %x \n", str2.c_str() );
     //https://my.oschina.net/u/257448/blog/1530238

}

//https://stackoverflow.com/questions/12011426/how-to-use-move-semantics-with-stdstring-during-function-returna




