import matplotlib.pyplot as plt


x = "2    & 4    & 6     & 8     & 10    & 12    & 14    & 16     & 18"
y1 = "3.55 & 7.44 & 12.78 & 20.74 & 34.01 & 53.57 & 86.91 & 136.18 & 217.43"
y2 = "3.54 & 6.87 & 10.56 & 14.13 & 17.93 & 21.71 & 25.25 & 28.51  & 32.55"

x = [float(it.strip()) for it in x.split("&")]
y1 = [float(it.strip()) for it in y1.split("&")]
y2 = [float(it.strip()) for it in y2.split("&")]


def plot_n_h():

    plt.plot(x, y1, c='#ff7f0e', label='cycle case')
    plt.plot(x, y2, c='#1f77b4', label='square case')
    plt.xlabel('log(n)')
    plt.ylabel('h')
    plt.legend()
    plt.show()
    return


def h_test():
    x_h = [(2**it) for it in x]
    y_h = [it**3 for it in y1]

    plt.plot(x_h, y_h, c='#ff7f0e', label='cycle case')
    plt.xlabel('n')
    plt.ylabel('h**3')
    plt.legend()
    plt.show()
    return


if __name__ == "__main__":
    plot_n_h()
    h_test()
