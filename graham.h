#ifndef GRAHAM_H
#define GRAHAM_H

#include "helper.h"

using namespace std;


class Graham
{
public:
    vector<point> p, st;
    int n, top;

    Graham(int n);
    void do_graham();

};

#endif