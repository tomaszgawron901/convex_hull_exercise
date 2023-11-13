from scipy.spatial import ConvexHull
from random_point import random_point_cycle, random_point_rec
import matplotlib.pyplot as plt


def n_h_test():
    n_h_list = []
    n = 4
    times = 100
    iter = 10
    for i in range(iter):
        h = 0
        for _ in range(times):
            #points = random_point_rec(n,n)
            points = random_point_cycle(n, n)
            # points = random_point_parabolic(n, int(n**1.5))
            points = list(points)
            hull = ConvexHull(points)
            h += len(hull.simplices)
        h /= times
        n_h_list.append((n, h))
        print(f"i={i}, n={n}, h={h}")
        n *= 2
    return n_h_list


def plot_n_h_list(n_h_list):
    x = [it[0] for it in n_h_list]
    y = [it[1] for it in n_h_list]
    plt.scatter(x, y, c='#1f77b4')
    plt.show()
    return


if __name__ == "__main__":
    n_h_list = n_h_test()
    print(f"n_h_list: {n_h_list}")
    plot_n_h_list(n_h_list)
