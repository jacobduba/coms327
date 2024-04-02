#ifndef DARRAY_H

#include <iostream>
using namespace std;
#define DEFAULT_SIZE 10

template <class T> class darray {
      private:
        T *a;
        int size;
        int back;

        void resize() {
                T *tmp;
                int i;

                tmp = new T(size * 2);

                for (i = 0; i < size; i++) {
                        tmp[i] = a[i];
                }

                size *= i;
                delete[] a;
                a = tmp;
        }

      public:
        darray() : a(new T[DEFAULT_SIZE]), size(DEFAULT_SIZE), back(0) {}
        darray(int s) : a(new T[s]), size(s), back(0) {}
        ~darray() {
                delete[] a;
        } // Square brackets tell delete this was an array allocation.

        void add(const T &t) {
                if (back == size) {
                        resize();
                }
        }

        T &operator[](int i) { return a[i]; }

        T &remove(int i) {
                while (i < size - 1) {
                        a[i] = a[i + 1];
                }
                back--;
        }

        int length() { return back; }

        ostream &print(ostream &o) {
                int i;
                cout << "darray[" << endl;
                for (i = 0; i < back; i++) {
                        cout << " " << a[i] << endl;
                }
                cout << "]" << endl;
        }
};

#endif
