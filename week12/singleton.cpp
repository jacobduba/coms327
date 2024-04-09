#include <iostream>

using namespace std;

class singleton {
      private:
        singleton() {}
        static singleton get_instance() { return singleton(); }
};

int main(int argc, char *argv[]) {
        singleton s;
        return 0;
}
