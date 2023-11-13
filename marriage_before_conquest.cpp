#include "marriage_before_conquest.h"

using namespace std;

Mbc::Mbc(vector<point> &p)
{
    full_p = p; // it should be deep copy
}

void Mbc::init_mbc_upper()
{
    most_left_point = {inf, 0};
    most_right_point = {-inf, 0};
    convex_line = {};

    p = full_p;
    // sorting is unnecessary. But now without sorting, there maybe some bugs.
    sort(p.begin(), p.end(), cmp_2);

    // when points have same x value, only save the top 1.
    // it only works when many points have same x value. not in general.
    // but it is necessary if there are many points in have same x value.
    // without it, it hard to deal with all points in mbc_upper have same x value.
    // unordered_map<double, double> mp;
    // for (auto &it : p)
    // {
    //     if (mp.find(it.x) == mp.end())
    //     {
    //         mp[it.x] = mp[it.y];
    //     }
    //     else
    //     {
    //         if (it.y > mp[it.x])
    //             mp[it.x] = mp[it.y];
    //     }
    // }
    // vector<point> temp_point;
    // temp_point.resize(mp.size());
    // for (auto &it : mp)
    // {
    //     temp_point.push_back({it.first, it.second});
    // }
    // p = temp_point;
    return;
}

void Mbc::mbc_upper(vector<point> p)
// marriage before conquest algorithm to find upper hull
{
    int n = p.size();
    if (n == 0)
        return;
    if (n == 1)
    {
        // add line of this point and most left (or right) point
        if (p[0].x < most_left_point.x)
        {
            convex_line.push_back(make_pair(p[0], most_left_point));
        }
        else if (p[0].x > most_right_point.x)
        {
            convex_line.push_back(make_pair(p[0], most_right_point));
        }
        return;
    }
    if (n == 2)
    {
        if (p[0].x != p[1].x)
        {
            convex_line.push_back(make_pair(p[0], p[1]));
        }
        return;
    }

    // double mid_x = (p[n / 2].x + p[n / 2 - 1].x) / 2;
    double mid_x = p[n / 2].x;
    // printf("mid_x=%lf\n", mid_x);

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
        // printf("%.1lf*x + %.1lf*y <= %.1lf, ", a[i], b[i], c[i]);
    }
    // printf("\n min(%lf*x+%.1lf*y) \n", c0, c1);
    // puts("");
    // puts("HERE!!");
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
    // printf("flag=%d\n", flag);
    // printf("%lf %lf\n", ans_x[0], ans_x[1]);

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
    // if (left_point.x == inf || left_point.x == right_point.x)
    // {
    //     // no bridges.
    //     // printf("debug: %d %lf %lf\n", p.size(), ans_x[0], ans_x[1]);
    //     // for (auto it : p)
    //     // {
    //     //     printf("points: %lf, %lf\n", it.x, it.y);
    //     // }
    // }
    // printf("line point: %lf %lf, %lf %lf\n", left_point.x, left_point.y, right_point.x, right_point.y);
    convex_line.push_back(make_pair(left_point, right_point));

    // update most_left_point, most_right_point;
    if (most_left_point.x > left_point.x)
        most_left_point = left_point;
    if (most_right_point.x < right_point.x)
        most_right_point = right_point;

    // delete points under the line, and do recursion
    vector<point> pl, pr;
    for (auto &it : p)
    {
        if (it.x >= left_point.x && it.x <= right_point.x)
            // Note: do delete left_point/right_point
            continue;
        if (it.x == mid_x)
            continue;
        if (it.x < mid_x)
            pl.push_back(it);
        else
            pr.push_back(it);
    }
    pl.push_back(left_point);
    pr.push_back(right_point);
    mbc_upper(pl);
    mbc_upper(pr);
    return;
}

vector<pair<point, point>> Mbc::mbc_full()
{
    vector<pair<point, point>> full_convex_line = {};
    // compute full convex of full_p.
    // mian idea: compute upper hull and lower hull, then put them together
    init_mbc_upper();
    mbc_upper(p);

    full_convex_line = convex_line;
    // for (auto &it : convex_line)
    // {
    //     printf("line: %lf %lf, %lf %lf\n", it.first.x, it.first.y, it.second.x, it.second.y);
    // }
    //  Flip all points of full_p on the x-axis
    for (auto &it : full_p)
    {
        it.y = -it.y;
    }

    init_mbc_upper();
    mbc_upper(p);
    // Flip all convex_line points on the x-axis
    // now it is lower hull of original full_p
    for (auto &it : convex_line)
    {
        it.first.y = -it.first.y;
        it.second.y = -it.second.y;
        // printf("line: %lf %lf, %lf %lf\n", it.first.x, it.first.y, it.second.x, it.second.y);
    }
    full_convex_line.insert(full_convex_line.end(), convex_line.begin(), convex_line.end());

    // add left/right edge if exist
    for (auto &it : full_p) // turn to original full_p
    {
        it.y = -it.y;
    }
    sort(full_p.begin(), full_p.end(), cmp);
    int i = 1;
    while (full_p[i].x == full_p[0].x)
        i++;
    if (i > 1)
    {
        full_convex_line.push_back(make_pair(full_p[0], full_p[i - 1]));
    }
    i = full_p.size() - 1;
    while (full_p[i].x == full_p[full_p.size() - 1].x)
        i--;
    if (i < full_p.size() - 2)
    {
        full_convex_line.push_back(make_pair(full_p[full_p.size() - 1], full_p[i + 1]));
    }

    return full_convex_line;
}
