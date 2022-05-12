"""
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
This file is part of the collaboration with Universitat Oberta de Catalunya (UOC) on
Multi-Source Team Orienteering Problem (MSTOP).
The objective of the project is to develop an efficient algorithm to solve this extension
of the classic team orienteering problem, in which the vehicles / paths may start from
several different sources.

Author: Mattia Neroni, Ph.D., Eng.
Contact: mneroni@uoc.edu
Date: January 2022
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
"""
import os
import math
import itertools
import collections
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

import node
import edge


# Single-source benchmarks
single_source_benchmarks = tuple(os.listdir("../tests/single/"))

# Multi-source benchmarks
multi_source_benchmarks = tuple(os.listdir("../tests/multi/"))

# Default colors for nodes, source nodes, adn depot
NODES_COLOR = '#FDDD71'
SOURCES_COLORS = ('#8FDDF4', '#8DD631', '#A5A5A5', '#DB35EF', '#8153AB')
DEPOT_COLOR = '#F78181'



def euclidean (inode, jnode):
    """
    The euclidean distance between two nodes.

    :param inode: First node.
    :param jnode: Second node.
    """
    return math.sqrt((inode.x - jnode.x)**2 + (inode.y - jnode.y)**2)



class Problem:
    """
    An instance of this class represents a single-source Team Orienteering
    Problem.

    It may also be translated according to different rules in a multi-source
    version of it.
    """

    def __init__(self, name, n_nodes, n_vehicles, Tmax, sources, nodes, depot):
        """
        Initialise.

        :param name: The name of the problem
        :param n_nodes: The number of nodes.
        :param n_vehicles: The number of vehicles / paths.
        :param Tmax: The maximum distance vehicles can run / time budget for paths.
        :param sources: The source nodes.
        :param nodes: The nodes to visit.
        :param depot: The depot.

        :attr dists: The matrix of distances between nodes.
        :attr positions: A dictionary of nodes positions.
        :attr edges: The edges connecting the nodes.
        """
        self.name = name
        self.n_nodes = n_nodes
        self.n_vehicles = n_vehicles
        self.Tmax = Tmax
        self.sources = sources
        self.nodes = nodes
        self.depot = depot

        # Initialise edges list and nodes positions
        edges = collections.deque()
        dists = np.zeros((n_nodes, n_nodes))
        # Calculate the matrix of distances and instantiate the edges
        # and define nodes colors and positions
        source_id = 0
        for node1, node2 in itertools.permutations(itertools.chain(sources, nodes, (depot,)), 2):
            # Calculate the edge cost
            id1, id2 = node1.id, node2.id
            cost = euclidean(node1, node2)
            # Compile the oriented matrix of distances
            dists[id1, id2] = cost
            # Create the edge
            if not node1.isdepot and not node2.issource:
                edges.append(edge.Edge(node1, node2, cost))

        self.dists = dists
        self.edges = edges


    def __hash__(self):
        return id(self)


    def __repr__(self):
        return f"""
        Problem {self.name}
        ---------------------------------------------
        nodes: {self.n_nodes}
        vehicles: {self.n_vehicles}
        Tmax: {self.Tmax}
        multi-source: {self.multi_source}
        ---------------------------------------------
        """

    @property
    def multi_source (self):
        """ A property that says if the problem is multi-source or not. """
        return len(self.sources) > 1


    def iternodes (self):
        """ A method to iterate over all the nodes of the problem (i.e., sources, customers, depot)"""
        return itertools.chain(self.sources, self.nodes, (self.depot,))



def plot (problem, *, routes=tuple(), mapping=None, figsize=(6,4), title=None):
    """
    This method is used to plot a problem using a graph representation that
    makes it easy-to-read.

    :param figsize: The size of the plot.
    :param title: The title of the plot.
    :param routes: The eventual routes found.
    """
    plt.figure(figsize=figsize)
    if title:
        plt.title(title)

    # Build the graph of nodes
    colors, pos = [], {}
    G = nx.DiGraph()
    source_id = 0

    for node in problem.iternodes():
        # Compile the graph
        pos[node.id] = (node.x, node.y)
        G.add_node(node.id)

        # Define nodes colors
        if node.issource:
            colors.append(SOURCES_COLORS[source_id])
            source_id += 1
        elif node.isdepot:
            colors.append(DEPOT_COLOR)
        else:
            if mapping is None:
                colors.append(NODES_COLOR)
            else:
                for i in range(len(problem.sources)):
                    if mapping[i, node.id] == 1:
                        colors.append(SOURCES_COLORS[i] + "60")
                        break

    # Save the routes
    edges = []
    for r in routes:
        # NOTE: Nodes of the route are supposed to always be in the order in which
        # they are stored inside the deque.
        nodes = tuple(r.nodes)
        edges.extend([(r.source.id, nodes[0].id), (nodes[-1].id, r.depot.id)])
        for n1, n2 in zip(nodes[:-1], nodes[1:]):
            edges.append((n1.id, n2.id))

    nx.draw(G, pos=pos, node_color=colors, edgelist=edges, with_labels=True, node_size=100, font_size=6, font_weight="bold")
    plt.show()



def export (problem, path):
    """
    This method exports the problem into a text file.

    :param problem: The problem to export.
    :param path: The directory where the problem will be saved.
    """
    with open(path + problem.name, 'w') as file:
        # Export number of nodes, number of vehicles, and Tmax
        file.write(f"n {problem.n_nodes}\n")
        file.write(f"m {problem.n_vehicles}\n")
        file.write(f"tmax {problem.Tmax}\n")
        # For each node
        for node in problem.iternodes():
            # Export coordinates and reveneu
            file.write(f"{round(node.x, 1)}\t{round(node.y, 1)}\t{node.revenue}\t")
            # If multi-source import indicator of source nodes and number of
            # vehicles starting from each source.
            if problem.multi_source:
                file.write(f"{int(node.issource)}\t{node.vehicles}")
            file.write('\n')





def read_single_source (filename, path="../tests/single/"):
    """
    This method is used to read a single-source Team Orienteering Problem
    from a file and returns a standard Problem instance.

    :param filename: The name of the file to read.
    :param path: The path where the file is.
    :return: The problem instance.
    """
    with open(path + filename, 'r') as file:
        # Read problem parameters
        n_nodes = int(next(file).replace('\n','').split(' ')[1])
        n_vehicles = int(next(file).replace('\n','').split(' ')[1])
        Tmax = float(next(file).replace('\n','').split(' ')[1])
        # Initialise nodes lists
        sources, nodes, depot = [], [], None
        # Read nodes characteristics
        for i, line in enumerate(file):
            node_info = line.split('\t')
            if i == 0:
                # Add a source node
                sources.append(node.Node(i, float(node_info[0]), float(node_info[1]), int(node_info[2]),
                              issource=True, vehicles=n_vehicles))
            elif i == n_nodes - 1:
                # Add the depot
                depot = node.Node(i, float(node_info[0]), float(node_info[1]), int(node_info[2]), isdepot=True)
            else:
                # Add a node to visit
                nodes.append(node.Node(i, float(node_info[0]), float(node_info[1]), int(node_info[2])))

        # Instantiate and return the problem
        return Problem(filename, n_nodes, n_vehicles, Tmax, tuple(sources), tuple(nodes), depot)



def read_multi_source (filename, path="../tests/multi/"):
    """
    This method is used to read a multi-source Team Orienteering Problem
    from a file and returns a standard Problem instance.

    :param filename: The name of the file to read.
    :param path: The path where the file is.
    :return: The problem instance.
    """
    with open(path + filename, 'r') as file:
        # Read problem parameters
        n_nodes = int(next(file).replace('\n','').split(' ')[1])
        n_vehicles = int(next(file).replace('\n','').split(' ')[1])
        Tmax = float(next(file).replace('\n','').split(' ')[1])
        # Initialise nodes lists
        sources, nodes, depot = [], [], None
        # Read nodes characteristics
        for i, line in enumerate(file):
            node_info = line.split('\t')
            # If the node is depot
            if i == n_nodes - 1:
                depot = node.Node(i, float(node_info[0]), float(node_info[1]), int(node_info[2]), isdepot=True)
                continue
            # If the node is source
            if node_info[3] == '1':
                sources.append(node.Node(i, float(node_info[0]), float(node_info[1]), int(node_info[2]),
                              issource=True, vehicles=int(node_info[4])))
            else:
                # Add a node to visit
                nodes.append(node.Node(i, float(node_info[0]), float(node_info[1]), int(node_info[2])))

        # Instantiate and return the problem
        return Problem(filename, n_nodes, n_vehicles, Tmax, tuple(sources), tuple(nodes), depot)



def merge (*problems, name="pmulti.txt", non_negative=False):
    """
    This method merges many TOP problem instances to create a
    multi-source TOP problem instance.
    None of the starting instances in modified in any way.

    The merging is made translating the problems (starting from
    the second) on the first one, so that the depots perfectly match.

    :param problems: The problem to merge.
    :param name: The name given to the new multi-source problem.
    :param non_negative: If True avoid negative coordinates (it does not have any real impact).
    :return: A new multi-source problem instance.
    """
    # Init name and parameters of the new problem
    n_sources = sum(len(p.sources) for p in problems)
    n_nodes = sum(p.n_nodes for p in problems) - (len(problems) - 1)
    n_vehicles = sum(p.n_vehicles for p in problems)
    Tmax = max(p.Tmax for p in problems)
    # Define the depot
    depot = problems[0].depot.__copy__()
    # Initialise the ids
    # NOTE: We want them to be increasing from the sources to the depot (just convention)
    depot.id = n_nodes - 1
    source_id = 0
    node_id = n_sources
    # Find the sources and the nodes
    sources, nodes = [], []
    for i, problem in enumerate(problems):
        # Calculate of how much the current problem must be
        # translated so that its depot match with that of the
        # other problems.
        dx = depot.x - problem.depot.x
        dy = depot.y - problem.depot.y
        # For each node...
        for node in problem.iternodes():
            # If the node is the depot there is no need to consider it
            if node.isdepot:
                continue
            # Make a copy of the node
            node = node.__copy__()
            # Translate the node
            node.x += dx
            node.y += dy
            # If the node is a source append it to sources
            if node.issource:
                sources.append(node)
                node.id = source_id
                source_id += 1
                continue
            # If the node is not a source append it to normal nodes
            nodes.append(node)
            node.id = node_id
            node_id += 1
    # Eventually translate the graph to avoid negative coordinates
    if non_negative:
        minX, minY = float("inf"), float("inf")
        for node in itertools.chain(sources, nodes, (depot,)):
            if node.x < minX:
                minX = node.x
            if node.y < minY:
                minY = node.y
        dx, dy = max(0, 0 - minX), max(0, 0 - minY)
        if dx > 0 or dy > 0:
            for node in itertools.chain(sources, nodes, (depot,)):
                node.x += dx
                node.y += dy

    return Problem(name, n_nodes, n_vehicles, Tmax, tuple(sources), tuple(nodes), depot)



