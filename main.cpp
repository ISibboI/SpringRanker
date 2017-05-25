#include "simulator.h"
#include "io.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

unsigned nx;
unsigned n_eq_constraints;
unsigned n_lt_constraints;

double surrounding_k;
unsigned max_rounds;
double max_delta;

double* x;
unsigned* eq_constraints;
unsigned* lt_constraints;

void getline_noerror(istream& in, string& line, unsigned& lc) {
    lc++;

    if (!getline(in, line)) {
        stringstream ss;
        ss << "Input file is missing lines. First missing line: " << lc;
        throw runtime_error(ss.str());
    }
}

void read_params(istream& in) {
    string line;
    unsigned lc = 0;

    getline_noerror(in, line, lc);
    nx = stoul(line);
    getline_noerror(in, line, lc);
    n_eq_constraints = stoul(line);
    getline_noerror(in, line, lc);
    n_lt_constraints = stoul(line);

    getline_noerror(in, line, lc);
    surrounding_k = stod(line);
    getline_noerror(in, line, lc);
    max_rounds = stoul(line);
    getline_noerror(in, line, lc);
    max_delta = stod(line);

    x = (double*) aligned_alloc(32, sizeof(double) * nx);
    eq_constraints = (unsigned*) aligned_alloc(32, sizeof(unsigned) * n_eq_constraints * 3);
    lt_constraints = (unsigned*) aligned_alloc(32, sizeof(unsigned) * n_lt_constraints * 3);

    for (unsigned i = 0; i < nx; i++) {
        getline_noerror(in, line, lc);
        x[i] = stod(line);
    }

    for (unsigned i = 0; i < n_eq_constraints * 3; i++) {
        getline_noerror(in, line, lc);
        eq_constraints[i] = stoul(line);
    }

    for (unsigned i = 0; i < n_lt_constraints * 3; i++) {
        getline_noerror(in, line, lc);
        lt_constraints[i] = stoul(line);
    }

    getline_noerror(in, line, lc);
    if (line.length() > 0) {
        throw runtime_error("Extra lines in input file!");

    }

    if (getline(in, line)) {
        throw runtime_error("Extra lines in input file!");
    }
}

int main() {
    read_params(cin);

    set_up(nx);

    simulate(x, eq_constraints, n_eq_constraints, lt_constraints, n_lt_constraints, surrounding_k, max_rounds, max_delta);

    for (unsigned i = 0; i < nx; i++) {
        cout << x[i] << endl;
    }

    unsigned* indexes = (unsigned*) aligned_alloc(32, sizeof(unsigned) * nx);

    for (unsigned i = 0; i < nx; i++) {
        indexes[i] = i;
    }

    sort(indexes, indexes + nx, [](unsigned a, unsigned b) {
        return x[a] > x[b];
    });

    for (unsigned i = 0; i < nx; i++) {
        cout << indexes[i] << endl;
    }

    free(indexes);
    tear_down();

    free(x);
    free(eq_constraints);
    free(lt_constraints);

    return 0;
}
