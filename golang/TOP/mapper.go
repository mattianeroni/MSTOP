package main

import (
	"math"
	"math/rand"
	"sort"
)

// Define the mapping type: a matrix whose elements (i, j) are equal to 1
// if node j is assigned to source i, and 0 otherwise
type Mapping [][]int

// This function make a biased randomised selection over a list
// according to a quasi-geometric probabilistic distribution
func BRA(list []Preference, beta float64) int {
	idx := int(math.Log(rand.Float64())/math.Log(1.0-beta)) % len(list)
	return idx
}

// This function carries out the mapping
// :param problem: The problem to map
// :param beta: The parameter of the quasi-geometric function
// :return: A Mapping instance
func Map(problem Problem, beta float64) Mapping {

	// Init number of sources and nodes
	N, S := len(problem.nodes), len(problem.sources)

	// Initialise useful variables
	dists := problem.dists

	// Initialise the mapping and reset the previous solution
	mapping := make(Mapping, S)
	for i, source := range problem.sources {
		source.preferences = make([]Preference, 0)
		source.nodes = make(map[int]*Node)
		mapping[i] = make([]int, N+S)
	}

	// Compose the list of preferences of each source
	for _, node := range problem.nodes {

		// Reset the assignment of the node
		node.assigned = false

		for i, source := range problem.sources {

			current_abs := dists[source.id][node.id]

			// Find the best other source
			m := math.Inf(1)
			for _, s := range problem.sources {
				if s.id != source.id {
					m = math.Min(dists[s.id][node.id], m)
				}
			}

			problem.sources[i].preferences = append(problem.sources[i].preferences, Preference{current_abs - m, node})
		}
	}

	// Sort preferences in place
	for i, _ := range problem.sources {
		pref := problem.sources[i].preferences
		sort.Slice(pref, func(i, j int) bool {
			return pref[i].vote < pref[j].vote
		})
	}

	// init variables used by the round-robin process
	var picked_node *Node
	var current_source *Node
	var sourceid int = 0

	// Round-robin process for assignment of nodes to sources
	for total_assigned := 0; total_assigned < N; {

		// Select the next source source
		current_source = problem.sources[sourceid]
		sourceid++
		// We loop over the sources
		if sourceid >= S {
			sourceid = 0
		}

		// If there are no more node that can be assigned to the current source
		// the next is considered
		if len(current_source.preferences) == 0 {
			continue
		}

		// Assign to the current source a number of nodes equal to the number of vehicles
		// starting from that source
		assigned := 0
		nVehicles := int(current_source.vehicles)
		for total_assigned < N && len(current_source.preferences) > 0 && assigned < nVehicles {

			// Pick a node and remove it from list of preferences
			idx := BRA(current_source.preferences, beta)
			picked_node = current_source.preferences[idx].node
			current_source.preferences = append(current_source.preferences[:idx], current_source.preferences[idx+1:]...)

			// Eventually assign the node if not already assigned
			if picked_node.assigned == false {
				picked_node.assigned = true
				current_source.nodes[picked_node.id] = picked_node
				mapping[current_source.id][picked_node.id] = 1
				assigned++
				total_assigned++
			}

		}

	}

	// Return the result
	return mapping
}
