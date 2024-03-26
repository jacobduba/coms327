#include <iostream>

using namespace std;

class shape {
      public:
        virtual ~shape() {}
        virtual double perimeter() = 0;
        virtual double area() = 0;
        virtual ostream &print(ostream &o) = 0;
};

ostream &operator<<(ostream &o, const shape &s) { return s.print(o); }

class rectangle : public shape {
      private:
        double w, h;

      public:
        rectangle(double width, double height) {
                w = width;
                h = height;
        }

        double perimeter() const { return 2 * (w + h); }

        double area() const { return w * h; }

        ostream &print(ostream &o) const {
                o << "rectangle[" << w << "," << h << "]";
        }
};

int main() {
        shape *s;
        rectangle r(7, 4);

        cout << r << endl;

        s = &r;

        cout << *s << endl;

        return 0;
}
