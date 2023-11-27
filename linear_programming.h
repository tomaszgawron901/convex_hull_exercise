#ifndef LINEAR_H
#define LINEAR_H

#include "helper.h"

using namespace std;

double one_d_linear(const vector<double> &a, const vector<double> &b, double c, int *flag, int *index);

vector<point> find_bridge(vector<point> &p, double pivot, int *flag);

#endif