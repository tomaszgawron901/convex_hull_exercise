#include "helper.h"
#include <string>
#include <iostream>

int main()
{
    vector<point> input_point;
    int n;
    for (int i = 0; i < 26; i++)
    {
        freopen(("text_files_parabolic/convex_in_"+to_string(i)+".txt").c_str(), "r", stdin);
        scanf("%d", &n);
        input_point.resize(n);
        for (int i = 0; i < n; i++)
        {
            scanf("%lf%lf", &input_point[i].x, &input_point[i].y);
        }
        fclose(stdin);

        auto start = chrono::high_resolution_clock::now();
        unique_x_points(input_point);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << to_string(duration.count()) << endl;

    }
    return 0;
}