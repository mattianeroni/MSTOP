import matplotlib.pyplot as plt
import networkx as nx
import os


def plot_problem (filename, path="./problems/",title=False):
    G = nx.Graph()
    pos = {}
    colors = []

    with open(path + filename, "r") as file:
        for line in file:
            id, x, y, revenue, color = tuple(line.split("\t"))
            G.add_node(int(id))
            pos[int(id)] = (float(x), float(y))
            colors.append(color.replace("\n", ""))

    nx.draw(G, pos=pos, node_color=colors,
            with_labels=True,
            node_size=100,
            font_size=6,
            font_weight="bold")

    if title:
        plt.title(filename)
    plt.show()



def plot_mapping (filename, path="./mappings/",title=False):
    G = nx.Graph()
    pos = {}
    colors = []

    with open(path + filename, "r") as file:
        for line in file:
            id, x, y, revenue, color = tuple(line.split("\t"))
            G.add_node(int(id))
            pos[int(id)] = (float(x), float(y))
            colors.append(color.replace("\n", ""))

    nx.draw(G, pos=pos, node_color=colors,
            with_labels=True,
            node_size=100,
            font_size=6,
            font_weight="bold")

    if title:
        plt.title(filename)
    plt.show()


def plot_routes (filename, path="./routes/", title=False):
    G = nx.DiGraph()
    pos = {}
    colors = []
    edgelist = []
    reading_routes = False
    with open(path + filename, "r") as file:
        for line in file:
            if line == "-----":
                reading_routes = True
                continue

            if not reading_routes:
                id, x, y, revenue, color = tuple(line.split("\t"))
                G.add_node(int(id))
                pos[int(id)] = (float(x), float(y))
                colors.append(color.replace("\n", ""))

            else:

                nodes = tuple(map(int, line.split("\t")))
                for i, j in zip(nodes[:-1], nodes[1:]):
                    edgelist.append( (i,j) )

    nx.draw(G, pos=pos, node_color=colors,
            edgelist=edgelist,
            with_labels=True,
            node_size=100,
            font_size=6,
            font_weight="bold")

    if title:
        plt.title(filename)
    plt.show()


if __name__ == '__main__':
    plot_problem("g456_4_n.txt")
    plot_mapping("g456_4_n.txt")
    #plot_routes("g456_2_c.txt")
