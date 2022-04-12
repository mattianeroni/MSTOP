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
import numpy as np
import itertools
import collections
import operator



def _reset_assignment (node):
    """
    Method used to reset the affiliation of a node to a source.
    """
    node.assigned = False
    return node




def mapper (problem, iterator):
    """
    An instance of this class represents the Mapper.
    The Mapper is engaged to assign each node to visit to a source.

    NOTE: This method change the problem nodes in place.

    :param problem: The instance of the Multi-Source Team Orienteering Problem to solve.
    :param iterator: The iterator used by source to pick the preferences.
    :return: A mapping --i.e., a 2-dimensional np.array where element (i, j) is 1 if
            node j is assigned to source i, and 0 otherwise.
    """
    # Extract the characteristics of the problem
    dists = problem.dists
    sources, nodes, depot = problem.sources, problem.nodes, problem.depot
    n_sources, n_nodes = len(problem.sources), len(problem.nodes)

    # Reset the source the nodes belongs to
    nodes = tuple(map(_reset_assignment, nodes))

    # Compute the absolute distances
    abs_dists = np.array([[dists[s.id, n.id] for n in nodes] for s in sources]).astype("float32")
    # NOTE: Suggestion to present
    #abs_dists = np.array([[dists[s.id, n.id] + dists[n.id, depot.id] for n in nodes] for s in sources]).astype("float32")

    # Compute the marginal distances
    for i, source in enumerate(sources):
        marginal_dists = abs_dists[i,:] - np.concatenate((abs_dists[:i,:], abs_dists[i+1:,:],), axis=0).min(axis=0)
        source.preferences = iterator(sorted(zip(marginal_dists, nodes), key=operator.itemgetter(0)))
        source.nodes = collections.deque()


    # Assign nodes to sources
    # Init the number of nodes already assigned and the mapping matrix
    n_assigned = 0
    mapping = np.zeros((n_sources, n_sources + n_nodes))
    _null_element = object()
    # NOTE: Until nodes are not concluded a source at each turn pick a number of preferred
    # nodes that depend on the number of vehicles it has.
    for source in itertools.islice(itertools.cycle(sources), n_nodes):
        # Consider the preferences of the currently considered source
        preferences = source.preferences
        # Pick a number of preferences that depend on the number of vehicles
        # that start from the source.
        for _ in range(source.vehicles):
            # Pick a node
            picked_node = next(preferences, _null_element)
            # If the generator is exhausted exit the loop
            if picked_node is _null_element:
                break
            # Assign the node to the source
            source.nodes.append(picked_node)
            picked_node.assigned = True
            mapping[source.id, picked_node.id] = 1
            n_assigned += 1

        # If all the nodes have already been assigned we exit the loop
        if n_assigned == n_nodes:
            break

    # Return the mapping
    return mapping
