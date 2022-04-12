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

class Edge:
    """
    An instance of this class represents an edge connecting two nodes to visit.

    Only edges connecting nodes to visit to each other are instantiated.
    We don't need edges connecting sources to nodes and nodes to depot.
    """
    def __init__(self, inode, jnode, cost):
        """
        Initialise.
        :param inode: The starting node.
        :param jnode: The ending node.
        :param cost: The length of the path from inode to jnode.
        """
        self.inode = inode
        self.jnode = jnode
        self.cost = cost
        self.savings = dict()
