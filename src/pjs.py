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
import operator
import collections
import functools
import heapq
import math
import random



class Route:
    """
    An instance of this class represents a Route --i.e., a path
    from the source to the depot made by a vehicle.
    """
    def __init__(self, source, depot, starting_node):
        """
        Initialise.
        :param source: The source of the route.
        :param depot: The depot of the route.
        :param starting_node: The first node included into the route.

        :attr nodes: The nodes part of the route.
        :attr revenue: The total revenue of the route.
        :attr cost: The total cost of the route.
        """
        self.source = source
        self.depot = depot
        self.nodes = collections.deque([starting_node])
        self.revenue = starting_node.revenue
        self.cost = starting_node.from_source + starting_node.to_depot

    def merge (self, other, edge, dists):
        """
        This method merges in place this route with another.

        :param other: The other route.
        :param edge: The edge used for merging.
        :param dists: The matrix of distances between nodes.
        """
        # Get the cost and the nodes of the edge
        inode, jnode = edge.inode, edge.jnode
        edge_cost = dists[inode.id, jnode.id]
        # Update the list of nodes in the route
        self.nodes.extend(other.nodes)
        # inode is not conneted to the depot anymore
        # and jnode is not connected to the source anymore
        inode.link_right = False
        jnode.link_left = False
        # Update the revenue and the cost of this route
        self.cost += edge_cost - inode.to_depot + (other.cost - jnode.from_source)
        self.revenue += other.revenue
        # Update the route the nodes belong to
        for node in other.nodes:
            node.route = self



def _bra (edges, beta):
    """
    Biased randomised selection of the edges.

    :param edges: The list of edges.
    :param beta: The parameter of the quasi-geometric distribution.
    :return: The retrieved edge.
    """
    L = len(edges)
    options = list(edges)
    for _ in range(L):
        idx = int(math.log(random.random(), 1.0 - beta)) % len(options)
        yield options.pop(idx)



def PJS (problem, source, nodes, depot, beta):
    """
    An implementation of the Panadero Juan Savings heuristic algorithm.
    It is generally used to solve a single source team orienteering problem.

    :param problem: The instance of the problem to solve.
    :param source: The source for which the PJS will be used.
    :param nodes: The customers nodes to visit.
    :param depot: The destination depot.
    :param beta: The parameter of the biased randomisation (i.e. close to 1 for a greedy behaviour)

    :return: The routes the vehicles starting from source will make.
    """
    # Move useful references to the stack
    n_vehicles, nodes = source.vehicles, set(nodes),
    dists, Tmax = problem.dists, problem.Tmax
    source_id, depot_id = source.id, depot.id

    # Filter edges keeping only those that interest this subset of nodes and sort them
    sorted_edges = sorted([e for e in problem.edges if e.inode in nodes and e.jnode in nodes], key=lambda edge: edge.savings[source_id], reverse=True)

    # Build a dummy solution where a vehicle starts from the source, visits
    # a single node, and then goes to the depot.
    routes = collections.deque()
    for node in nodes:
        # Calculate the distances from the node to the depot and
        # from the source to the node and save them.
        node.from_source = dists[source_id, node.id]
        node.to_depot = dists[node.id, depot_id]
        # Verify if the node can be visited according to the Tmax.
        if node.from_source + node.to_depot > Tmax:
            node.route = None
            continue
        # Eventually construct a new route that goes from the
        # source to the node and from the node to the depot.
        route = Route(source, depot, node)
        node.route = route
        node.link_left = True
        node.link_right = True
        routes.append(route)

    # Merge the routes giving priority to edges with highest efficiency
    for edge in _bra(sorted_edges, beta):
        inode, jnode = edge.inode, edge.jnode
        # If the edge connect nodes already into the same route
        # next edge is considered
        iroute, jroute = inode.route, jnode.route
        if iroute is None or jroute is None or iroute == jroute:
            continue
        # If inode is the last of its route and jnode the first of its
        # route, the merging is possible.
        if inode.link_right and jnode.link_left:
            # Compare the length of the new route to Tmax
            if iroute.cost - inode.to_depot + jroute.cost - jnode.from_source + edge.cost <= Tmax:
                # Merge the routes
                iroute.merge(jroute, edge, dists)
                # Remove the route incorporated into iroute
                routes.remove(jroute)
        # If the number of routes is already equal to the number of vehicles,
        # interrupt the procedure.
        if len(routes) == n_vehicles:
            break

    # Return the solution as a list of the best possible routes.
    return sorted(routes, key=operator.attrgetter("revenue"), reverse=True)[:n_vehicles]


@functools.lru_cache(maxsize=None)
def PJS_cache (problem, source, nodes, depot, alpha):
    """
    Cached implementation of the PJS.
    Used only for heuristic and deterministic behaviour when we do not
    need to explore different solutions.

    :param alpha: The alpha value used to calculate edges savings (used only for caching)

    :return: The routes the vehicles starting from source will make.
    """
    return PJS(problem, source, nodes, depot, beta=0.9999)



def multistartPJS (problem, source, nodes, depot, alpha, maxiter, betarange):
    """
    This method is a multi-start execution of the PJS.
    At each iteration, a new solution is generated by using a different beta
    parameter in the selection of edges for the merging process.

    :param maxiter: The maximum number of iterations.
    :param betarange: The range in which beta is randomly generated at each iteration.
    :return: The best solution found as a set of routes, and the respective revenue.
    """
    # Generate the starting greedy solution
    bestroutes = PJS_cache(problem, source, nodes, depot, alpha)
    bestrevenue = sum(r.revenue for r in bestroutes)

    # Save beta ranges
    betamin, betamax = betarange

    for _ in range(maxiter):

        # Generate a new solution
        routes = PJS(problem, source, nodes, depot, beta=random.uniform(betamin, betamax))
        revenue = sum(r.revenue for r in routes)

        # Eventually update the best
        if revenue > bestrevenue:
            bestroutes, bestrevenue = routes, revenue

    # Return the best solution found so far
    return bestroutes, bestrevenue
