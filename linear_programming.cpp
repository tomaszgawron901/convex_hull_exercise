#include "linear_programming.h"

using namespace std;

double one_d_linear(const vector<double> &a, const vector<double> &b, double c, int *flag)
/*
unknown x, minimize c1*x, where:
a[0]*x <= b[0],
a[1]*x <= b[1],
...,
a(n-1)*x <= b[n-1].
When flag=0: Feasible, the minimize value is return value
When flag=1: Infeasible
When flag=2: Unbounded
*/
{
    if (c == 0)
    {
        return 0.0;
    }
    (*flag) = 0;
    int len = a.size();
    double min_bound = -inf, max_bound = inf;
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
            max_bound = min(max_bound, bound);
        else // x >= bound
            min_bound = max(min_bound, bound);
    }

    if ((*flag) == 1)
        return -1;
    if (max_bound < min_bound)
    {
        (*flag) = 2;
        return -1;
    }
    if (c > 0)
        return min_bound;
    else
        return max_bound;
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

void shuffle_vectors(vector<double> &a, vector<double> &b, vector<double> &c)
// shuffle constraints of 2D_LP
{
    /*
    -- To shuffle an array a of n elements (indices 0..n-1):
    for i from 0 to n−2 do
     j ← random integer such that i ≤ j < n
     exchange a[i] and a[j]
    */
    int n = a.size();
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < n - 1; i++)
    {
        // rand_index in [i,n)
        int rand_index = rand() % (n - i) + i;
        if (rand_index != i)
        {
            swap(a[i], a[rand_index]);
            swap(b[i], b[rand_index]);
            swap(c[i], c[rand_index]);
        }
    }
    return;
}

vector<double> two_d_linear_point(const vector<point> &p, double c0, double c1, int *flag)
// two_d_linear based on points in MbC algorithm
// pass point instead of vector a, b, c can avoid copy
{
    vector<double> opt_v = {0, -inf}; // init_v_by_c0(c0, c1);
    (*flag) = 0;
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
        double c_1d = c0 - c1 * p[i].x;

        int flag_1d = 0;
        double ans_1d = one_d_linear(a_1d, b_1d, c_1d, &flag_1d);
        if (flag_1d == 2) // unbounded
            continue;
        if (flag_1d == 1) // infeasible
        {
            (*flag) = 1;
            break;
        }
        is_unbounded = 0;

        opt_v[0] = ans_1d;
        opt_v[1] = p[i].y - p[i].x * opt_v[0];

        if ((*flag) != 1 && is_unbounded == 1)
            (*flag) = 2; // unbounded
    }
    return opt_v;
}

vector<double> two_d_linear(vector<double> &a, vector<double> &b, vector<double> &c,
                            double c0, double c1, int *flag)
/*
two_d_linear ONLY for MbC (marriage_before_conquest) algorithm.
unknown x0, x1, minimize c0*x0+x1, where:
a[i]*x0 + b[i]*x1 <= c[i]. (b[i] always is 1 in MbC)

When flag=0: Feasible, the minimize value is return value
When flag=1: Infeasible
When flag=2: Unbounded
in MbC algorithm, flag is always Feasible.

Algorithm:
1. random a point v (x0, x1). (in MbC algorithm, it can always be (0, -inf))
2. for i in range n:
    2.1. if  constraint i does not violate v: pass
    2.2. Solve 1D_LP with constraints 1 to i − 1 on boundary of constraint i

return the best (x0, x1) (if exist)
*/
{
    vector<double> opt_v = {0, -inf}; // init_v_by_c0(c0, c1);
    (*flag) = 0;
    int len = a.size();
    int is_unbounded = 1;
    shuffle_vectors(a, b, c);

    // when i=0, there is no constraints in 1D_LP
    for (int i = 0; i < len; i++)
    {
        // b[i] is always -1

        if (a[i] * opt_v[0] + b[i] * opt_v[1] <= c[i])
            continue;

        //  do 1d linear programming on this line:
        //  x[1] = c[i]/b[i]-a[i]/b[i]*x[0]
        //  in MbC algorithm, b[i]=-1
        vector<double> a_1d, b_1d;
        a_1d.resize(i);
        b_1d.resize(i);
        for (int j = 0; j < i; j++)
        {
            a_1d[j] = (a[j] - b[j] * a[i] / b[i]);
            b_1d[j] = c[j] - b[j] * c[i] / b[i];
        }
        double c_1d = c0 - c1 * a[i] / b[i];

        int flag_1d = 0;
        double ans_1d = one_d_linear(a_1d, b_1d, c_1d, &flag_1d);
        if (flag_1d == 2) // unbounded
            continue;
        if (flag_1d == 1) // infeasible
        {
            (*flag) = 1;
            break;
        }
        is_unbounded = 0;

        opt_v[0] = ans_1d;
        opt_v[1] = c[i] / b[i] - a[i] / b[i] * opt_v[0];

        if ((*flag) != 1 && is_unbounded == 1)
            (*flag) = 2; // unbounded
    }
    return opt_v;
}

void test_1d()
{
    int n;
    double c;
    vector<double> a, b;
    freopen("text_files/linear_1d.in", "r", stdin); // output should be: 2.0

    while (~scanf("%d%lf", &n, &c))
    {
        a.resize(n);
        b.resize(n);
        for (int i = 0; i < n; i++)
        {
            scanf("%lf %lf", &a[i], &b[i]);
        }
        int flag;
        double ans = one_d_linear(a, b, c, &flag);
        printf("flag: %d, ans: %lf\n", flag, ans);
    }
    fclose(stdin);
    return;
}

void test_2d()
{
    int n;
    double c0, c1;
    vector<double> a, b, c;
    // freopen("text_files/linear_2d.in", "r", stdin); // output should be: (3.5, 1.5)
    freopen("text_files/linear_2d_2.in", "r", stdin); // output should be: (1.8, 1.6)

    while (~scanf("%d%lf%lf", &n, &c0, &c1))
    {
        a.resize(n);
        b.resize(n);
        c.resize(n);
        for (int i = 0; i < n; i++)
        {
            scanf("%lf %lf %lf", &a[i], &b[i], &c[i]);
        }
        int flag;
        vector<double> ans = two_d_linear(a, b, c, c0, c1, &flag);
        printf("flag: %d, ans: %lf %lf\n", flag, ans[0], ans[1]);
    }
    fclose(stdin);
    return;
}
