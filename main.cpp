// test all these three algorithms
#include <string>
#include "graham.h"
#include "jarvis.h"
#include "marriage_before_conquest.h"

using namespace std;

vector<point> input_point;

void refersh_in_out()
{
    fclose(stdout);
    fclose(stdin);
    freopen("CON", "w", stdout);
    freopen("CON", "r", stdin);
}

void set_in_out_to_target(int index)
{
    char input_file[100], output_file[100];

    // sprintf just like f-string in Python
    sprintf(input_file, "text_files/convex_in_%d.txt", index);
    sprintf(output_file, "text_files/out_%d.txt", index);
    freopen(input_file, "r", stdin);
    freopen(output_file, "w", stdout);
}

// amazing, this function do not work somehow, even cannot scanf...
double test_alg_once(int index, string &alg, int times = 100)

// test one algotithm on one data case for 100 (default) times.
// alg in ["mbc", "graham", "jarvis"]
// gengrate input data first.

{
    set_in_out_to_target(index);

    int n;
    chrono::duration<double> duration;
    vector<point> output_point;

    scanf("%d", &n);
    input_point.resize(n);
    // printf("%lf %lf", input_point[0].x, input_point[0].y);
    for (int i = 0; i < n; i++)
    {
        scanf("%lf%lf", &input_point[i].x, &input_point[i].y);
    }
    if (alg == "mbc")
    {
        Mbc mbc(input_point);
        vector<pair<point, point>> full_convex_line;
        for (int i = 0; i < times; i++)
        {

            mbc.p = input_point;
            auto start = chrono::high_resolution_clock::now();
            full_convex_line = mbc.mbc_full();
            auto end = chrono::high_resolution_clock::now();
            duration += end - start;
        }
        // for (auto it : input_point)
        // {
        //     printf("%lf %lf\n", it.x, it.y);
        // }
        // for(auto it : full_convex_line)
        // {
        //     printf("%lf %lf, %lf %lf\n", it.first.x, it.first.y, it.second.x, it.second.y);
        // }
        // top_sort is just format, not necessary
        output_point = top_sort(full_convex_line);
    }
    if (alg == "graham")
    {

        Graham gra(n);
        for (int i = 0; i < times; i++)
        {
            gra.p = input_point; // it is deep copy
            auto start = chrono::high_resolution_clock::now();
            gra.do_graham();
            auto end = chrono::high_resolution_clock::now();
            duration += end - start;
        }
        gra.st.resize(gra.top);
        output_point = gra.st;
    }
    if (alg == "jarvis")
    {
        Jarvis jar(n);
        input_point.resize(n);
        for (int i = 0; i < n; i++)
        {
            scanf("%lf%lf", &input_point[i].x, &input_point[i].y);
        }
        for (int i = 0; i < times; i++)
        {
            jar.p = input_point;
            auto start = chrono::high_resolution_clock::now();
            jar.do_jarvis();
            auto end = chrono::high_resolution_clock::now();
            duration += end - start;
        }
        jar.st.resize(jar.top);
        output_point = jar.st;
    }
    printf("number of points of convex (+1): %d\n", output_point.size());
    for (auto &it : output_point)
    {
        printf("%lf %lf\n", it.x, it.y);
    }
    refersh_in_out();
    return duration.count(); // to double
}

int main()
{
    // alg in {"mbc", "graham", "jarvis"}
    string alg = "mbc";
    for (int i = 0; i < 10; i++)
    {
        printf("%d-th data case, %s duration: %lf s\n", i, alg.c_str(), test_alg_once(i, alg, 1));
    }
    return 0;
}
