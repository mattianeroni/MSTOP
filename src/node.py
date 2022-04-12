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
import collections

class Node:
    """
    An instance of this class represents a node to visit or
    a source some vehicles are starting from.
    It is used for the depot too.
    """
    def __init__(self, id, x, y, revenue, *, color='#FDDD71', issource=False, vehicles=0, isdepot=False):
        """
        Initialise.

        :param id: The unique id of the node.
        :param x: The x-coordinateof the node.
        :param y: The y-coordinate of the node.
        :param revenue: The revenue.
        :param issource: A boolean variable that says if the node is a source.
        :param isdepot: A boolean variable that says if the node is the depot.
        :param vehicles: The number of vehicles starting from this node (it is 0
                         if the node is not a source).

                    *** Parameters used by the Mapper ***
        :attr assigend: True if the node is assigned to a source and 0 otherwise
        :attr preferences: Used in case of source node for the round-robin process.
        :attr nodes: Used in case of source for keeping the nodes assigned to it.

                    *** Parameters used by the PJS ***
        :attr from_source: The length of the current path from the source to this node.
        :attr to_depot: The length of the current path from this node to the depot.
        :attr route: The current route corresponding to the node.
        :attr link_left: True if the node is linked to the source, False otherwise.
        :attr link_right: True if the node is linked to the depot, False otherwise.
        """
        self.id = id
        self.x = x
        self.y = y
        self.revenue = revenue
        self.issource = issource
        self.vehicles = vehicles
        self.isdepot = isdepot

        # Attributes used by the Mapper
        self.assigned = False
        self.preferences = collections.deque()
        self.nodes = collections.deque()

        # Attributes used by the PJS
        self.from_source = 0
        self.to_depot = 0
        self.route = None
        self.link_left = False
        self.link_right = False

    def __copy__(self):
        obj = Node.__new__(self.__class__)
        obj.__dict__.update(self.__dict__)
        return obj

    def __repr__(self):
        return f"Node {self.id}"

    def __hash__(self):
        return self.id
