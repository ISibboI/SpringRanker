#include "simulator.h"
#include "io.h"

#include <algorithm>
#include <stdlib.h>
#include <iostream>

using namespace std;

const bool DEBUG = false;

double* delta_x;
unsigned length;

inline int fast_sign(double d) {
    uint64_t l = *((uint64_t*) (&d));
    return 1 - ((l >> 62) & 2);
}

inline double fast_abs(double d) {
    uint64_t* l = (uint64_t*) (&d);
    *l &= 0x7fffffffffffffff;
    return d;
}

double do_step(double* x, unsigned* eq_constraints, unsigned n_eq_constraints, unsigned* lt_constraints, unsigned n_lt_constraints, double surrounding_k, double delta_t) {
    for (unsigned i = 0; i < length; i++) {
        delta_x[i] = 0;
    }

    for (unsigned i = 0; i < n_eq_constraints; i++) {
        unsigned i_first = *(eq_constraints++);
        unsigned i_second = *(eq_constraints++);
        double first = x[i_first];
        double second = x[i_second];

        double distance = second - first;
        double delta = distance * delta_t;
        delta_x[i_first] += delta;
        delta_x[i_second] -= delta;

        if (DEBUG) cout << "Eq: " << i_first << " -> " << delta << " <- " << i_second << endl;
    }

    for (unsigned i = 0; i < n_lt_constraints; i++) {
        unsigned i_first = *(lt_constraints++);
        unsigned i_second = *(lt_constraints++);
        double first = x[i_first];
        double second = x[i_second];

        double distance = min(0., second - first - 1);
        double delta = distance * delta_t;
        delta_x[i_first] += delta;
        delta_x[i_second] -= delta;

        if (DEBUG) cout << "Lt: " << i_first << " -> " << delta << " <- " << i_second << endl;
    }

    double surrounding_delta = delta_t * surrounding_k;

    for (unsigned i = 0; i < length; i++) {
        double x_i = x[i];
        delta_x[i] -= x_i * surrounding_delta;
    }

    double max_delta = -1;
    for (unsigned i = 0; i < length; i++) {
        double dx = delta_x[i];
        x[i] += dx;

        max_delta = max(max_delta, fast_abs(dx));
    }

    if (DEBUG) cout << "Current x: ";
    if (DEBUG) print_x(x, length);
    if (DEBUG) cout << "Current delta_x: ";
    if (DEBUG) print_x(delta_x, length);

    return max_delta;
}

void simulate(double* x, unsigned* eq_constraints, unsigned n_eq_constraints, unsigned* lt_constraints, unsigned n_lt_constraints, double surrounding_k, unsigned max_steps, double max_delta) {
    for (unsigned i = 0; i < max_steps; i++) {
        if (DEBUG) cout << "Step " << i << ":" << endl;
        double delta = do_step(x, eq_constraints, n_eq_constraints, lt_constraints, n_lt_constraints, surrounding_k, 0.1);

        if (DEBUG) cout << "Delta is " << delta << endl;

        if (delta <= max_delta) {
            break;
        }
    }
}

void set_up(unsigned new_length) {
    delta_x = (double*) aligned_alloc(32, sizeof(double) * new_length);
    length = new_length;
}

void tear_down() {
    free(delta_x);
}
