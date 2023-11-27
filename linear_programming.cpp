#include "linear_programming.h"

using namespace std;

double one_d_linear(const vector<double> &a, const vector<double> &b, double c, int *flag, int *index)
/*
unknown x, minimize c1*x, where:
a[0]*x <= b[0],
a[1]*x <= b[1],
...,
a(n-1)*x <= b[n-1].
When flag=0: Feasible, the minimize value is return value
When flag=1: Infeasible
*/
{
    if (c == 0)
    {
        return 0.0;
    }
    (*flag) = 0;
    (*index) = -1;
    int len = a.size();
    double min_bound = -inf, max_bound = inf;
    int min_index = -1, max_index = -1;
    double ans = min_bound;
    for (int i = 0; i < len; i++)
    {
        // a[i]*x <= b[i]
        if (a[i] == 0.0)
        {
            if (b[i] < 0)
            {
                (*flag) = 1; // infeasible
                break;
            }
            else
                continue; // b[i] >= 0
        }
        double bound = b[i] / a[i];
        if (a[i] > 0) // x <= bound
        {
            if(max_bound > bound)
            {
                max_bound = bound;
                max_index = i;
            }
        }
        else // x >= bound
        {
            if(min_bound < bound)
            {
                min_bound = bound;
                min_index = i;
            }
        }
    }

    if (max_bound < min_bound)
        (*flag) = 1; // infeasible

    if ((*flag) == 1)
        return -1;

    if (c > 0)
    {
        (*index) = min_index;
        return min_bound;
    }
    else
    {
        (*index) = max_index;
        return max_bound;
    }
}

vector<double> init_v_by_c0(double c0, double c1)
// return v(x0, x1) of 2D_LP, when minimize c0*x0+c1*x1 and
{
    vector<double> x{0.0, 0.0};
    if (c0 > 0)
        x[0] = -inf;
    else
        x[0] = inf;

    if (c1 > 0)
        x[1] = -inf;
    else
        x[1] = inf;

    return x;
}

vector<point> find_bridge(vector<point> &p, double pivot, int *flag)
// two_d_linear based on points in MbC algorithm
// pass point instead of vector a, b, c can avoid copy
{
    random_shuffle(p.begin(), p.end());

    vector<double> opt_v = {0, -inf};
    (*flag) = 0;
    pair<int, int> index = pair(-1, -1);
    int len = p.size();
    int is_unbounded = 1;
    // p is const so we cannot do shuffle
    // but for random data, no shuffle is ok
    // shuffle_vectors(a, b, c);

    for (int i = 0; i < len; i++)
    {
        // b[i] is always -1

        if (-p[i].x * opt_v[0] - 1 * opt_v[1] <= -p[i].y)
            continue;

        //  do 1d linear programming on this line:
        //  x[1] = c[i]/b[i]-a[i]/b[i]*x[0]
        //  in MbC algorithm, b[i]=-1
        vector<double> a_1d, b_1d;
        a_1d.resize(i);
        b_1d.resize(i);
        for (int j = 0; j < i; j++)
        {
            a_1d[j] = p[i].x - p[j].x;
            b_1d[j] = p[i].y - p[j].y;
        }
        double c_1d = pivot - p[i].x;

        int flag_1d = 0;
        int index_1d = -1;
        double ans_1d = one_d_linear(a_1d, b_1d, c_1d, &flag_1d, &index_1d);

        if (flag_1d == 1) // infeasible
        {
            (*flag) = 1;
            break;
        }
        is_unbounded = 0;

        index = pair(i, index_1d);

        opt_v[0] = ans_1d;
        opt_v[1] = p[i].y - p[i].x * opt_v[0];

        if ((*flag) != 1 && is_unbounded == 1)
            (*flag) = 2; // unbounded
    }

    vector<point> bridge;
    bridge.push_back(p[index.first]);
    if(index.second >= 0) {
        bridge.push_back(p[index.second]);
    }

    return bridge;
}
