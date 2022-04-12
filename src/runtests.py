import os
import time
import functools
import itertools
import operator

import utils
import iterators
import solver
from mapper import mapper
from pjs import PJS, multistartPJS




if __name__ == "__main__":

    header = ",".join([
        "Problem",
        "P1",
        "P2",
        "P3",
        "PJS1",
        "PJS2",
        "PJS3",
        "MSPJS1",
        "MSPJS2",
        "MSPJS3",
        "Heur",
        "RTMeta",
        "IntMeta"
    ])

    # Plot the headers of the csv files
    with open("Revenues.csv", "w") as file:
        file.write(f"{header}\n")

    with open("Times.csv", "w") as file:
        file.write(f"{header}\n")

    with open("Distances.csv", "w") as file:
        file.write(f"{header}\n")





    for filename in sorted(os.listdir("../tests/multi/"), key=lambda i : len(i)):

        print(filename)

        # Prot problem name
        with open("Revenues.csv", "a") as file:
            file.write(f"{filename},")
        with open("Times.csv", "a") as file:
            file.write(f"{filename},")
        with open("Distances.csv", "a") as file:
            file.write(f"{filename},")


        # Optimise alpha and set savings
        problem = utils.read_multi_source(filename)
        alpha = solver.alpha_optimisation(problem)
        solver.set_savings(problem, alpha=alpha)



        # Get number of problems combined together
        n_probs = len(filename.split("_")[0]) - 1
        # Get characteristic of the problem
        v_type = filename[n_probs + 2]
        tmax_type = filename[n_probs + 4]

        # Write problems names for single source ones
        for i, _ in itertools.zip_longest(filename[1:1 + n_probs], range(3), fillvalue=None):
            if i is None:
                with open("Revenues.csv", "a") as file:
                    file.write(",")
                with open("Times.csv", "a") as file:
                    file.write(",")
                with open("Distances.csv", "a") as file:
                    file.write(",")
                continue


            name = f"p{i}.{v_type}.{tmax_type}.txt"
            with open("Revenues.csv", "a") as file:
                file.write(f"{name},")
            with open("Times.csv", "a") as file:
                file.write(f"{name},")
            with open("Distances.csv", "a") as file:
                file.write(f"{name},")



        # Run the heuristic pjs
        for i, _ in itertools.zip_longest(filename[1:1 + n_probs], range(3), fillvalue=None):

            if i is None:
                with open("Revenues.csv", "a") as file:
                    file.write(",")

                with open("Times.csv", "a") as file:
                    file.write(",")

                with open("Distances.csv", "a") as file:
                    file.write(",")
                continue

            name = f"p{i}.{v_type}.{tmax_type}.txt"
            prob = utils.read_single_source(name)
            prob.Tmax = problem.Tmax
            solver.set_savings(prob, alpha)
            _start = time.time()
            routes = PJS(prob, prob.sources[0], prob.nodes, prob.depot, beta=0.9999)
            duration = time.time() - _start

            with open("Revenues.csv", "a") as file:
                file.write(f"{sum(r.revenue for r in routes)},")

            with open("Times.csv", "a") as file:
                file.write(f"{duration},")

            with open("Distances.csv", "a") as file:
                file.write(f"{sum(r.cost for r in routes)},")


        # Run the multi start PJS
        for i, _ in itertools.zip_longest(filename[1:1 + n_probs], range(3), fillvalue=None):

            if i is None:
                with open("Revenues.csv", "a") as file:
                    file.write(",")
                with open("Times.csv", "a") as file:
                    file.write(",")
                with open("Distances.csv", "a") as file:
                    file.write(",")
                continue

            name = f"p{i}.{v_type}.{tmax_type}.txt"
            prob = utils.read_single_source(name)
            prob.Tmax = problem.Tmax
            solver.set_savings(prob, alpha)
            _start = time.time()
            routes, revenue = multistartPJS (prob, prob.sources[0], prob.nodes, prob.depot, alpha, maxiter=1000, betarange=(0.1,0.3))
            duration = time.time() - _start

            with open("Revenues.csv", "a") as file:
                file.write(f"{revenue},")

            with open("Times.csv", "a") as file:
                file.write(f"{duration},")

            with open("Distances.csv", "a") as file:
                file.write(f"{sum(r.cost for r in routes)},")



        # Run the Multi Source Heuristic
        _start = time.time()

        revenue, mapping, routes = solver.heuristic(problem, iterators.greedy, alpha)

        duration = time.time() - _start

        with open("Revenues.csv", "a") as file:
            file.write(f"{revenue},")

        with open("Times.csv", "a") as file:
            file.write(f"{duration},")

        with open("Distances.csv", "a") as file:
            file.write(f"{sum(r.cost for r in routes)},")




        # Run the multi start heuristic
        _start = time.time()

        revenue, mapping, routes = solver.multistart(problem, alpha, maxiter=1000, betarange=(0.1, 0.3))

        duration = time.time() - _start

        with open("Revenues.csv", "a") as file:
            file.write(f"{revenue},")

        with open("Times.csv", "a") as file:
            file.write(f"{duration},")

        with open("Distances.csv", "a") as file:
            file.write(f"{sum(r.cost for r in routes)},")


        # Run the long run optimization on the elite solutions
        _start = time.time()

        elite_solutions = solver.multistart_keep_elites(problem, alpha, maxiter=1000, betarange=(0.1, 0.3), nelites=5)

        revenue, mapping, routes = solver.optimise_elites(problem, elite_solutions, alpha, maxiter=3000, betarange=(0.1, 0.3))

        duration = time.time() - _start

        with open("Revenues.csv", "a") as file:
            file.write(f"{revenue},\n")

        with open("Times.csv", "a") as file:
            file.write(f"{duration},\n")

        with open("Distances.csv", "a") as file:
            file.write(f"{sum(r.cost for r in routes)},\n")

        #utils.plot(problem, mapping=mapping, routes=routes)





    print("Program concluded \u2764\uFE0F")
