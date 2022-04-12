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
import random
import math

def greedy (preferences):
    """
    This is a greedy iterator of preferences.
    It iterates the source preferences from the best to the worst,
    and as soon as it meets a node which has not been assigned yet,
    it returns it.

    :param preferences: The iterable list of preferences.
    """
    for _, node in preferences:
        if not node.assigned:
            yield node


def BRA (preferences, beta=0.3):
    """
    This method carry out a biased-randomised selection over the list of preferences.
    The selection is based on a quasi-geometric function:
                    f(x) = (1 - beta) ^ x
    and it therefore prioritise the first elements in list.

    :param preferences: The set of options already sorted from the best to the worst.
    :param beta: The parameter of the quasi-geometric distribution.
    :return: The element picked at each iteration.
    """
    L = len(preferences)
    options = list(preferences)
    for _ in range(L):
        idx = int(math.log(random.random(), 1.0 - beta)) % len(options)
        _, node = options.pop(idx)
        if not node.assigned:
            yield node
