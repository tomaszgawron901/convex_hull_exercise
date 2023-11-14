#include "jarvis.h"

using namespace std;

Jarvis::Jarvis(int n)
{
    Jarvis::n = n;
    st.resize(n * 4);
    p.resize(n);
}

void Jarvis::do_jarvis()
{
    int lm = get_left_most(p);
    top = 0;
    st[top++] = p[lm];

    int pivot = lm;
    point inf_point = {p[lm].x, -inf};
    do {
        double min_angle = inf;
        int min_index = -1;
        for (int i = 0; i < n; i++)
        {
            if (dis(p[pivot], p[i]) < eps) // avoid same point
                continue;
            double this_angle = pi - clockwise_angle( p[pivot], p[i], inf_point);
            if (this_angle < min_angle)
            {
                min_angle = this_angle;
                min_index = i;
            }
        }
        st[top++] = p[min_index];
        inf_point = p[pivot];
        pivot = min_index;
    }
    while (pivot != lm);
}
