#include "marriage_before_conquest.h"

using namespace std;

Mbc::Mbc(vector<point> &p)
{
    points = p;
    convex_line = {};
}

void Mbc::mbc_upper(vector<point> p)
// marriage before conquest algorithm to find upper hull
{
    int n = p.size();
    if(n < 3) {
        sort(p.begin(), p.end(), cmp);
        for (auto &it : p)
        {
            convex_line.push_back(it);
        }
        return;
    }

    double mid_x = (p[0].x + p[1].x) / 2;  // this way there is always at lest one point smaller and one point larger then mid_x

    /*
    find the bridge: transfer to 2d_linear programming:
    a, b is unknown, minimize x0*mid_x+x1, where:
    ( x0*p[i].x + x1 >= p[i].y ) ==> ( -p[i].x*x0 - x1 <= -p[i].y )
    for all point p[i]
    */
    double c0 = mid_x, c1 = 1;
    int flag;
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
    if (flag != 0)
    {
        // debug
        printf("flag:%d\n", flag);
        printf("%lf %lf\n", ans_x[0], ans_x[1]);
        for (auto it : p)
        {
            printf("points: %lf, %lf\n", it.x, it.y);
        }
    }

    //  find 2 points on this line
    //  if these are more than 2 points, only save most left and most right one
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

    // delete points under the line, and do recursion
    vector<point> pl, pr;
    for (auto &it : p)
    {
        if(it.x > right_point.x)
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
    // compute full convex of full_p.
    mbc_upper(points);

    for (auto &it : points)
    {
        it.y = -it.y;
        it.x = -it.x;
    }

    int bottom_hull_start = convex_line.size();
    mbc_upper(points);

    for (auto &it : points) // turn to original full_p
    {
        it.y = -it.y;
        it.x = -it.x;
    }

    for (size_t i = bottom_hull_start; i < convex_line.size(); i++)
    {
        convex_line[i].y = -convex_line[i].y;
        convex_line[i].x = -convex_line[i].x;
    }
    

    return convex_line;
}
