
// using standard exceptions
#include <iostream>
#include <exception>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <cstdlib>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
/**


class exception {
public:
  exception () noexcept;
  exception (const exception&) noexcept;
  exception& operator= (const exception&) noexcept;
  virtual ~exception();
  virtual const char* what() const noexcept;
}

assert vs throw
三、Catch Segmentation fault in c++

1. C++ try-catch blocks only handle C++ exceptions.  
   只能捕获The C++ Standard library提供的异常
2.  Signals are sometimes called software interrupts() 
	系统异常通过信号(signal)实现，try-catch无效，
   - Signals can be sent By the kernel to a process 
   - 信号是在软件层次上对中断机制的一种模拟，信号是进程间通信机制中唯一的异步通信机制
    - you can distinguish three types of interrupts: software Interrupts/Hardware Interrupts/Exceptions
       信号处理程序是在用户态下运行的，而中断处理程序是在核心态下运行

3. The signals SIGKILL 9  and SIGSTOP 19 cannot be caught or ignored.  不能被忽略、处理和阻塞

Signals are totally different than C++ exceptions.
You can't use a C++ try/catch block to handle a signal. 
Specifically, signals are a POSIX concept, not a C++ language concept. 
Signals are delivered asynchronously to your application by the kernel, 
whereas C++ exceptions are synchronous events defined by the C++ standard.




ref：
- https://gist.github.com/fairlight1337/55978671ace2c75020eddbfbdd670221
- http://www.cplusplus.com/reference/exception/exception/
- https://isocpp.org/wiki/faq/exceptions#why-exceptions
- https://stackoverflow.com/questions/2350489/how-to-catch-segmentation-fault-in-linux
- http://gityuan.com/2015/12/20/signal/
- https://www.youtube.com/watch?v=uFBNf7F3l60
- https://www.cnblogs.com/charlesblc/p/6277810.html
- http://www.delorie.com/djgpp/doc/ug/interrupts/inthandlers1.html

**/



class myexception : public exception
{
	virtual const char *what() const throw()
	{
		return "My exception happened";
	}

public:
	myexception()
	{
		//out<< "myexception()" <<endl;
	}
} myex;

class Foo
{
private:
	int _id;

public:
	Foo() : _id(0)
	{
		cout << "Default Construct " << this << " id = " << _id << endl;
	}
	Foo(int id) : _id(id)
	{
		cout << "Construct " << this << " id = " << _id << endl;
	}
	~Foo()
	{
		cout << "Destructor " << this << endl;
	}
	static void *operator new(size_t size)
	{
		cout << "static void *operator new (size_t size)" << endl;
		Foo *p = (Foo *)malloc(size);
		return p;
	}
	static void operator delete(void *p, size_t size)
	{
		cout << "static void operator delete" << endl;
		free(p);
	}
	//传入地址，返回地址，没有做其余操作
	static void *operator new(size_t size, void *start)
	{
		return start;
	}
	static void *operator new(size_t size, long extra)
	{
		return malloc(size + extra);
	}
	static void *operator new[](size_t size)
	{
		Foo *p = (Foo *)malloc(size);
		return p;
	}
	static void operator delete[](void *p, size_t size)
	{
		free(p);
	}
};

//自己抛出异常,自己catch
void testException()
{
	try
	{
		throw myex;
	}
	catch (exception &e)
	{
		cout << e.what() << '\n';
	}
}
//第三方库调用抛出异常,已经帮你throw了;
void test_out_of_range()
{
	std::vector<int> myvector(10);
	try
	{
		myvector.at(20) = 100; // vector::at throws an out-of-range
	}
	catch (const std::out_of_range &oor)
	{
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
}
//c++ sdk抛出异常    已经帮你throw了
//throwing (1)	void* operator new (std::size_t size);
void test_logic_error()
{
	try
	{
		int x = 10;
		int y = 0;
		// std::cin >> x >> y;
		std::cout << x / y << std::endl;
	}
	catch (logic_error &e)
	{ //Floating point exception
		std::cout << "std::logic_error=" << e.what() << "\n";
	}
	catch (...)
	{
		std::cout << "attempt to divide integer by 0." << std::endl;
	}
}

void test_segmentation()
{
	try
	{
		int *foo = NULL;
		*foo = 1; /* Cause a seg fault */
	}
	catch (std::exception &e)
	{
		std::cerr << "test_segmentation Exception caught : " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "test_segmentation ....." << std::endl;
	}
}
//liunx系统调用抛出异常
void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
	printf("Caught segfault at address %p\n", si->si_addr);
	exit(0); //进程退出
}
void test_sig_segment()
{
	int *foo = NULL;
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_sigaction;
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGSEGV, &sa, NULL);

	/* Cause a seg fault */
	*foo = 1;
}
int main()
{
	test_sig_segment();
	
	return 0;
}
