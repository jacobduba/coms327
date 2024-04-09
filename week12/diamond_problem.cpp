#include <iostream>

using namespace std;

// How could you have different configurations of A (so virt inher doesn't work)
// A has different state in B or C

class A {
      public:
        A() { cout << __PRETTY_FUNCTION__ << endl; }
        A(A &a) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~A() { cout << __PRETTY_FUNCTION__ << endl; }
        virtual void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class B : virtual public A {
      public:
        B() { cout << __PRETTY_FUNCTION__ << endl; }
        B(B &a) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~B() { cout << __PRETTY_FUNCTION__ << endl; }
        virtual void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class C : virtual public A {
      public:
        C() { cout << __PRETTY_FUNCTION__ << endl; }
        C(C &a) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~C() { cout << __PRETTY_FUNCTION__ << endl; }
        virtual void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class D : virtual public B, virtual public C {
      public:
        D() { cout << __PRETTY_FUNCTION__ << endl; }
        D(D &a) { cout << __PRETTY_FUNCTION__ << endl; }
        virtual ~D() { cout << __PRETTY_FUNCTION__ << endl; }
        virtual void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

int main(int argc, char *argv[]) {
        D d;
        // B b;

        // b.print();

        // A &a = b;
        // a.print();

        // d.print();
        // d.B::print();
        // d.C::print();

        // d.C::A::print();

        ((B)d).A::print();

        return 0;
}
