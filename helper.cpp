#include "helper.h"

using namespace std;

bool point::operator==(const point &other) const
{
    return x == other.x && y == other.y;
}

bool point::operator!=(const point &other) const
{
    return x != other.x || y != other.y;
}

bool point::operator<(const point &other) const
{
    return x < other.x || (x == other.x && y < other.y);
}

size_t point_hash::operator()(const point &p) const
{
    return std::hash<double>()(p.x) ^ std::hash<double>()(p.y);
}

int cmp(const point &a, const point &b)
// compare for sort
{
    return a.x == b.x ? a.y < b.y : a.x < b.x;
}

int cmp_2(const point &a, const point &b)
// compare for sort, if a.x == b.x
{
    return a.x == b.x ? a.y > b.y : a.x < b.x;
}

double dis(const point &a, const point &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double turn_test(const point &p1, const point &p2, const point &p3)
// return: left (counter-clockwise): > 0
{
    return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
}

double clockwise_angle(const point &a, const point &b, const point &c)
// angle between AB and BC in clockwise direction
{
    //  atan2  return arctan y/x in [−pi , pi]
    double a_to_b = atan2(b.y - a.y, b.x - a.x);
    double a_to_c = atan2(c.y - a.y, c.x - a.x);
    double angle_acb = a_to_b - a_to_c;

    // range in [-pi, pi]
    const double Pi = acos(-1); // or use some π constant
    if (angle_acb > Pi)
        angle_acb -= 2 * Pi;
    else if (angle_acb < -Pi)
        angle_acb += 2 * Pi;

    // angle_acb in [-pi...pi]
    return angle_acb;
}

void flip_points(vector<point>::iterator begin, vector<point>::iterator end)
{
    for (auto it = begin; it < end; it++)
    {
        it->y = -(it->y);
        it->x = -(it->x);
    }
    return;
}

vector<point> top_sort(const vector<pair<point, point>> &line_vec)
{
    // transfer the line<point1, point2>, line<point2, point3>, ..., to:
    // point1, point2, ...
    unordered_map<point, vector<point>, point_hash> mp;
    vector<point> ans;
    unordered_set<point, point_hash> visited_point;
    for (auto it : line_vec)
    {
        mp[it.first].push_back(it.second);
        mp[it.second].push_back(it.first);
    }
    point p = mp.begin()->first;
    ans.push_back(p);
    visited_point.insert(p);
    while (ans.size() < line_vec.size())
    {
        for (auto it : mp[p])
        {
            if (visited_point.find(it) == visited_point.end())
            {
                p = it;
                ans.push_back(p);
                visited_point.insert(p);
                break;
            }
        }
    }
    // add the first point to the end, to be a cycle
    ans.push_back(ans[0]);
    return ans;
}

vector<point> unique_x_points(const vector<point> &p)
// only save the most top point when there are many points in a vertical line
// it's O(n), but with a big constant when input size become larger
{
    // all points in p are in integer coordinate
    // value of mp is the index of p
    unordered_map<int, int> mp;
    mp.reserve(p.size());
    for (int i = 0; i < p.size(); i++)
    {
        int x = p[i].x;
        if (mp.find(x) == mp.end() || p[mp[x]].y < p[i].y)
            mp[x] = i;
    }

    vector<point> ans;
    ans.reserve(mp.size());
    for (auto &it : mp)
    {
        ans.emplace_back(p[it.second]);
    }
    return ans;
}