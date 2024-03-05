#include <iostream>

// std is the standard namespace. Everyting in the STL (Standard template
// library) is in the standard namespace. To look inside a namespace, use
// namespace::   :: is called the scope resolution operator

using namespace std;
// This line tells the compiler to always look for symbols in the standard
// namespace. It is no longer necessary to use std:: after this line in order to
// access STL symbols. However. it contaiminates the default namespace with all
// of those thousands of symbols. This is considered bad practice!

// That said, in lecture I will (nearly) always do this.
// 1) So I don't waste your time fixing all of the palces I forgot to add std::
// and 2) because it really doesn't hurt anyting in the small programs we write.
// This is an example of "do as I say, not as I do".

int main(int argc, char *argv[]) {
        // printf("Hello World!\n");
        cout << "Hello World!" << std::endl;

        return 0;
}
