#include "marriage_before_conquest.h"

using namespace std;

Mbc::Mbc(vector<point> &p)
{
    points = p;
    convex_line = {};
}

void Mbc::mbc_upper(const vector<point> &p)
// marriage before conquest algorithm to find upper hull
{
    int n = p.size();
    if (n == 0)
        return;
    if (n == 1)
    {
        convex_line.push_back(p[0]);
        return;
    }
    if (n == 2)
    {
        convex_line.push_back(min(p[0], p[1]));
        convex_line.push_back(max(p[0], p[1]));
        return;
    }

    // this way there is always at lest one point smaller and one point larger then mid_x
    // and also avoid point(s) on the mid_x line
    double mid_x = (p[0].x + p[1].x) / 2 + 0.1;
    /*
    find the bridge: transfer to 2d_linear programming:
    a, b is unknown, minimize x0*mid_x+x1, where:
    ( x0*p[i].x + x1 >= p[i].y ) ==> ( -p[i].x*x0 - x1 <= -p[i].y )
    for all point p[i]
    */
    double c0 = mid_x, c1 = 1;
    int flag;

    /*
    // two_d_linear_point() is faster, but can not shuffle_vectors()
    // if want to shuffle_vectors(), run follow codes instead of two_d_linear_point()
    vector<double> a, b, c;
    a.resize(n);
    b.resize(n);
    c.resize(n);
    for (int i = 0; i < n; i++)
    {
        a[i] = -p[i].x;
        b[i] = -1;
        c[i] = -p[i].y;
    }
    vector<double> ans_x = two_d_linear(a, b, c, c0, c1, &flag);
    */
    vector<double> ans_x = two_d_linear_point(p, c0, c1, &flag);

    if (flag != 0)
    {
        // debug
        printf("flag:%d, mid_x: %lf\n", flag, mid_x);
        printf("%lf %lf\n", ans_x[0], ans_x[1]);
        for (auto it : p)
        {
            printf("points: %lf, %lf\n", it.x, it.y);
        }
    }
    //   find 2 points on this line
    //   if these are more than 2 points, only save most left and most right one
    point left_point = {inf, 0}, right_point = {-inf, 0};
    for (auto &it : p)
    {
        if (abs(it.x * ans_x[0] + ans_x[1] - it.y) <= eps) // point on line
        {
            if (it.x < left_point.x)
                left_point = it;
            if (it.x > right_point.x)
                right_point = it;
        }
    }
    //  delete points under the line, and do recursion
    vector<point> pl, pr;
    for (auto &it : p)
    {
        if (it.x > right_point.x)
        {
            pr.push_back(it);
        }
        else if (it.x < left_point.x)
        {
            pl.push_back(it);
        }
    }
    pl.push_back(left_point);
    pr.push_back(right_point);
    mbc_upper(pl);
    mbc_upper(pr);
    return;
}

vector<point> Mbc::mbc_full()
{

    // compute full convex of full_points.
    mbc_upper(unique_x_points(points));

    flip_points(points.begin(), points.end());

    int bottom_hull_start = convex_line.size();
    mbc_upper(unique_x_points(points));

    flip_points(points.begin(), points.end()); // turn to original full_p
    // turn points on the lower hull to original space
    flip_points(convex_line.begin() + bottom_hull_start, convex_line.end());

    // in some case, there are some same continous points in convex_line somehow.
    // so, do unique. unique only delete continous same elements (keep the first one)
    int n = unique(convex_line.begin(), convex_line.end()) - convex_line.begin(); // O(n)
    convex_line.resize(n);
    // and if there are vertical line in the right side of convec hull,
    // convex_line may not include the last line somehow.
    // so, add it if it is missing.
    if (convex_line[0] != convex_line[convex_line.size() - 1])
    {
        convex_line.push_back(convex_line[0]);
    }
    return convex_line;
}
