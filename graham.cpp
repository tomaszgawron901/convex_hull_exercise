#include "graham.h"

using namespace std;

Graham::Graham(int n)
{
    Graham::n = n;
    st.resize(n * 2);
    p.resize(n);
}

void Graham::do_graham()
{
    sort(p.begin(), p.end(), cmp);
    top = 0;
    st[top++] = p[0];
    st[top++] = p[1];
    for (int i = 2; i < n; i++)
    {
        while (top >= 2 && turn_test(st[top - 2], st[top - 1], p[i]) > 0)
        {
            top--;
        }
        st[top++] = p[i];
    }
    st[top++] = p[n - 2];
    for (int i = n - 3; i >= 0; i--)
    {
        while (top >= 2 && turn_test(st[top - 2], st[top - 1], p[i]) > 0)
        {
            top--;
        }
        st[top++] = p[i];
    }
    return;
}
