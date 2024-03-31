#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

class shape {
      public:
        virtual ~shape() {}
        virtual double perimeter() const = 0;
        virtual double area() const = 0;
        virtual ostream &print(ostream &o) const = 0;
};

ostream &operator<<(ostream &o, const shape &s) { return s.print(o); }

class rectangle : public shape {
      protected:
        double w, h;

      public:
        rectangle(double width, double height) {
                w = width;
                h = height;
        }

        double perimeter() const { return 2 * (w + h); }

        double area() const { return w * h; }

        ostream &print(ostream &o) const {
                return o << "rectangle[" << w << "," << h << "]";
        }
};

class square : public rectangle {
      public:
        // Initialization lists allow you to call non-default constructors. All
        // members of a class must be instantiated before the constructor may
        // run, so this is the only way to get the behavior you may want from a
        // non-default constructor.
        square(double side) : rectangle(side, side) {}

        ostream &print(ostream &o) const { return o << "square[" << h << "]"; }
};

class circle : public shape {
      protected:
        double r;

      public:
        circle(double radius) : r(radius) { r = radius; }

        double circumference() const { return 2 * M_PI * r; }

        double perimeter() const { return circumference(); }

        double area() const { return M_PI * r * r; };

        ostream &print(ostream &o) const { return o << "circle[" << r << "]"; }
};

#define rand_float(i) (i * (rand() / MAXFLOAT))

int main() {
        shape *s;
        rectangle r(7, 4);
        // Basically array list
        vector<shape *> v;
        int i;
        vector<shape *>::iterator vi;

        cout << r << endl;

        s = &r;

        cout << *s << endl;

        // New and malloc are on seperate heaps.
        circle *c = new circle(7);
        cout << *c << endl;
        cout << c->area() << " " << c->circumference() << endl;

        s = c;

        s = new square(9);

        cout << *s << endl;
        // Undefined behavior.
        // cout << s->area() << " " << ((circle *)s)->circumference() << endl;

        for (i = 0; i < 100; i++) {
                switch (rand() % 3) {
                case 0:
                        v.push_back(
                            new rectangle(rand_float(10), rand_float(10)));
                        break;
                case 1:
                        v.push_back(new square(rand_float(10)));
                        break;
                case 2:
                        v.push_back(new circle(rand_float(10)));
                        break;
                }
        }

        for (vi = v.begin(); vi != v.end(); vi++) {
                // Use the dereference operator to "look inside" an iterator. In
                // this case, since or container is filled with pointers, we
                // need a second * to dereference the object.
                cout << **vi << endl;
                if (dynamic_cast<circle *>(*vi)) {
                        cout << ((circle *)(*vi))->circumference() << endl;
                }

                delete *vi;
        }

        return 0;
}
