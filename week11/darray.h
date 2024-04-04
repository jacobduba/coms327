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
                T *t = new T[size * 2];
                int i;

                for (i = 0; i < size; i++) {
                        t[i] = a[i];
                }

                delete[] a;
                a = t;
                size *= 2;
        }

        // core files are memory dumps of program that segfaulted
        // use for debugging

      public:
        class iterator {
              private:
                int i;
                darray<T> &d;

              public:
                iterator() { i = 0; }
                iterator(darray<T> &d) : d(d), i(0) {}
                void next() { i++; }
                T operator*() const { return d[i]; }
                bool is_valid() const { return i < d.back(); }
        };

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

        T &operator[](int i) const {
                if (i >= back) {
                        throw "darray inex out of bounds";
                }
                return a[i];
        }

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
                return cout;
        }
};

template <class T> ostream &operator<<(ostream &o, const darray<T> &d) {
        return d.print(o);
}

#endif
