#include "darray.h"

using namespace std;

int main(int argc, char *argv[]) {
        darray<int> di;
        int i;

        for (i = 0; i < 100; i++) {
                di.add(i);
        }

        cout << di << endl;

        darray<float>::iterator dfi;

        while (dfi.is_valid()) {
                cout << *dfi << endl;
                dfi.next();
        }

        return 0;
}
