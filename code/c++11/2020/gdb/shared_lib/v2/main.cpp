#include "foo.h"

int main() {
    Foo* foo = new Foo();
    foo->foo();
    foo->bar();
    delete foo;

    return 0;
}
