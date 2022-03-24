package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {

	// Set the random seed
	rand.Seed(time.Now().UnixNano())

	problem := read_multi_source("g456_4_n.txt", "./tests/multi/")
	export_problem(problem, "./utils/problems/")

	problem.edges = computeSavings(problem, 0.7)

	start := time.Now()
	mapping := Map(problem, 0.9999)
	export_mapping(mapping, problem, "./utils/mappings/")

	MultiSourcePJS(problem, 0.9999)

	fmt.Println("Execution Time: ", time.Now().Sub(start).Seconds())

	//

	fmt.Printf("Program executed \u2764\uFE0F\n")
}
