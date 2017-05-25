#include "io.h"

void print_x(double* x, unsigned length) {
    for (unsigned i = 0; i < length; i++) {
        cout << x[i];

        if (i != length - 1) {
            cout << ", ";
        }
    }

    cout << endl;
}
