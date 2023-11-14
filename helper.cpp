#include "helper.h"

using namespace std;

bool point::operator==(const point &other) const
{
    return x == other.x && y == other.y;
}

size_t point_hash::operator()(const point &p) const
{
    return std::hash<double>()(p.x) ^ std::hash<double>()(p.y);
}

int cmp(point a, point b)
// compare for sort
{
    return a.x == b.x ? a.y < b.y : a.x < b.x;
}

int cmp_2(point a, point b)
// compare for sort, if a.x == b.x
{
    return a.x == b.x ? a.y > b.y : a.x < b.x;
}

double dis(point a, point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double turn_test(point p1, point p2, point p3)
// return: left (counter-clockwise): > 0
{
    return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
}

double clockwise_angle(point a, point b, point c)
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
    else if (angle_acb <= -Pi)
        angle_acb += 2 * Pi;

    // angle_acb in [-pi...pi]
    return angle_acb;
}

int get_left_most(vector<point> points) {
    int out = 0;
    for (int i = 1; i < points.size(); i++)
    {
        if(points[i].x < points[out].x || (points[i].x < points[out].x && points[i].y < points[out].y)) {
            out = i;
        }
    }
    return out;
}

vector<point> top_sort(vector<pair<point, point>> line_vec)
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
