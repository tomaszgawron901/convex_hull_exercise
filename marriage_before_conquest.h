#ifndef MBC_H
#define MBC_H

#include "linear_programming.h"

using namespace std;

class Mbc
{
public:
    vector<pair<point, point>> convex_line;
    point most_left_point, most_right_point;
    vector<point> full_p, p;

    Mbc(vector<point> &p);

    void init_mbc_upper();

    void mbc_upper(vector<point> p);

    vector<pair<point, point>> mbc_full();
};

#endif