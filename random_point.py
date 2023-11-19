import random


def random_point_rec(num, size=10, full_vertices=False):
    # generate unique num points that radom in [0, size]
    points = set({})
    if full_vertices:
        points.add((0, 0))
        points.add((0, size))
        points.add((size, 0))
        points.add((size, size))

    while len(points) < num:
        points.add((random.randint(0, size), random.randint(0, size)))
    return points


def random_point_cycle(num, size=10):
    # generate unique num points that radom in a r=size-cycle
    points = set({})
    while len(points) < num:
        x, y = random.randint(-size, size), random.randint(-size, size)
        if x*x + y*y < size*size:
            points.add((x, y))
    return points


def random_point_parabolic(num, size=10, sign=1):
    # generate unique num points that radom in y=x^2
    # sign = 1: y=x^2
    # sign = -1: y=-x^2
    points = set({})
    while len(points) < num:
        x = random.randint(0, size)
        points.add((x, sign*x*x))
    return points

# TODO: add other cases


def write_points(points, file):
    f = open(file, "w")
    print(len(points))
    f.write(str(len(points)) + "\n")
    for it in points:
        f.write(str(it[0]) + " " + str(it[1]) + "\n")
    f.close()


if __name__ == "__main__":

    # random 10 files once run
    n = 4
    for i in range(18):
        #points = random_point_rec(n,n, full_vertices=True)
        #points = random_point_rec(n,n)
        #points = random_point_cycle(n, n)
        points = random_point_parabolic(n, int(n*3))
        write_points(points, f"text_files/convex_in_{i}.txt")
        n *= 2
