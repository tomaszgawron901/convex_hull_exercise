#ifndef HELPER_H
#define HELPER_H

#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>

using namespace std;

const double inf = 1e10;
const double eps = 1e-9;
const double pi = acos(-1);

struct point
{
    double x, y;
    point() {}
    point(double x, double y) : x(x), y(y) {}

    bool operator==(const point &other) const;
};

struct point_hash // for unordered_map<point, something, point_hash>
{
    size_t operator()(const point &p) const;
};

int cmp(point a, point b);

int cmp_2(point a, point b);

double turn_test(point p1, point p2, point p3);

double dis(point a, point b);

double clockwise_angle(point a, point b, point c);

vector<point> top_sort(vector<pair<point, point>> line_vec);

#endif