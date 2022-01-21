all: libfoo.so main
main:main.cpp
	g++ -g -o main main.cpp -L . -lfoo -I .
libfoo.so:foo.cpp
	g++ -fPIC -o foo.o -c $<
	g++ -shared -o $@ foo.o
clean:
	 rm libfoo.so main
