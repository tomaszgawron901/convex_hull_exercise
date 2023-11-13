import matplotlib.pyplot as plt
import numpy as np

# x = np.array([4, 4, 5, 7])
# y = np.array([8, 12, 9.3, 8])


def read_point_set(file):
    f = open(file, "r")
    texts = list(f)
    texts = [it[:-1] for it in texts[1:]]
    texts = [it.split(" ") for it in texts]
    texts = [[float(it[0]), float(it[1])]for it in texts]
    print(texts)
    x = [it[0] for it in texts]
    y = [it[1] for it in texts]
    return x, y


def show_case(index):
    x, y = read_point_set(f"text_files/convex_in_{index}.txt")
    x_out, y_out = read_point_set(f"text_files/out_{index}.txt")

    plt.scatter(x, y, c='#1f77b4')  # ff7f0e
    plt.plot(x_out, y_out, c='#ff7f0e')
    plt.show()


if __name__ == "__main__":
    #show_case(1)
    for i in range(10):
        show_case(i)
