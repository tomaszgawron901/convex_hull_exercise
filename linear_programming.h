#ifndef LINEAR_H
#define LINEAR_H

#include "helper.h"

using namespace std;

double one_d_linear(const vector<double> &a, const vector<double> &b, double c, int *flag);

vector<double> init_v_by_c0(double c0, double c1);

void shuffle_vectors(vector<double> &a, vector<double> &b, vector<double> &c);

vector<double> two_d_linear(vector<double> &a, vector<double> &b, vector<double> &c,
                            double c0, double c1, int *flag);

vector<double> two_d_linear_point(const vector<point> &p, double c0, double c1, int *flag);
void test_1d();

#endif