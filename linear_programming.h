#ifndef LINEAR_H
#define LINEAR_H

#include "helper.h"

using namespace std;

double one_d_linear(const vector<double> &a, const vector<double> &b, double c, int *flag, int *index);

vector<double> init_v_by_c0(double c0, double c1);

void shuffle_vectors(vector<double> &a, vector<double> &b, vector<double> &c);

vector<double> two_d_linear(const vector<double> &a, const vector<double> &b,
                            const vector<double> &c, double c0, double c1, int *flag, pair<int, int> *index);

void test_1d();

#endif