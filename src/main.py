import os
import time
import functools

import utils
import iterators
import solver
from mapper import mapper
from pjs import PJS




if __name__ == "__main__":

    problem = utils.read_multi_source("g12_4_k.txt")

    alpha = solver.alpha_optimisation(problem)

    solver.set_savings(problem, alpha=alpha)




    _start = time.time()

    revenue, mapping, routes = solver.heuristic(problem, iterators.greedy, alpha)

    print(time.time() - _start)





    _start = time.time()

    revenue, mapping, routes = solver.multistart(problem, alpha, maxiter=1000, betarange=(0.1, 0.3))

    print(time.time() - _start)


    #_start = time.time()

    #elite_solutions = solver.multistart_keep_elites(problem, alpha, maxiter=1000, betarange=(0.1, 0.3), nelites=5)

    #print(time.time() - _start)

    #_start = time.time()

    #revenue, mapping, routes = solver.optimise_elites(problem, elite_solutions, alpha, maxiter=3000, betarange=(0.1, 0.3))

    #print(time.time() - _start)

    #utils.plot(problem, mapping=mapping, routes=routes)





    print("Program concluded \u2764\uFE0F")
