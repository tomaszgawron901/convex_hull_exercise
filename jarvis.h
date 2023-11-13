#ifndef JARVIS_H
#define JARVIS_H

#include "helper.h"

using namespace std;

class Jarvis
{
public:
    vector<point> p, st;
    int n, top;

    Jarvis(int n);
    void do_jarvis();
};

#endif