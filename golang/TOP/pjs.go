package main

import (
	"math"
	"math/rand"
	"sort"
)

func computeSavings(problem Problem, alpha float64) []Edge {

	dists := problem.dists
	depot := problem.depot
	edges := make([]Edge, len(problem.edges))

	for i, edge := range problem.edges {

		inode, jnode := edge.inode, edge.jnode
		edge.savings = make(map[int]float64)

		for _, source := range problem.sources {

			edge.savings[source.id] = alpha*(dists[source.id][jnode.id]+dists[inode.id][depot.id]-edge.cost) + (1.0-alpha)*float64((inode.revenue+jnode.revenue))
		}

		edges[i] = edge
	}

	return edges
}

func remove(slice []*Route, element *Route) []*Route {
	newSlice := make([]*Route, 0)
	for _, route := range slice {
		if route != element {
			newSlice = append(newSlice, route)
		}
	}
	return newSlice
}

type Route struct {
	nodes   []*Node
	cost    float64
	revenue int64
}

// This function make a biased randomised selection over a list
// according to a quasi-geometric probabilistic distribution
func BRS(list []Preference, beta float64) int {
	idx := int(math.Log(rand.Float64())/math.Log(1.0-beta)) % len(list)
	return idx
}

// This function is an implementation of the Panadero Juan Savings heuristic
// on a multi-source situation
func MultiSourcePJS(problem Problem, beta float64) (int64, []*Route) {

	// Init the cost of the total solution
	var total_revenue int64 = 0
	var total_routes = make([]*Route, 0)

	for i, _ := range problem.sources {

		revenue, routes := PJS(problem, i, beta)
		total_revenue += revenue
		total_routes = append(total_routes, routes...)

	}

	return total_revenue, total_routes
}

func PJS(problem Problem, sourceid int, beta float64) (int64, []*Route) {

	// Init useful variables
	source, depot := problem.sources[sourceid], problem.depot
	dists, edges, Tmax := problem.dists, problem.edges, problem.Tmax
	nodes := source.nodes
	n_vehicles := int(source.vehicles)

	// Init final solution
	routes := make([]*Route, 0)

	// Generates dummy solution
	for _, node := range nodes {
		node.from_source = dists[sourceid][node.id]
		node.to_depot = dists[node.id][depot.id]
		node.linkStart = true
		node.linkEnd = true
		newRoute := &Route{nodes: make([]*Node, 0), cost: node.from_source + node.to_depot, revenue: node.revenue}
		node.route = newRoute
		routes = append(routes, newRoute)
	}

	// Sort edges
	sort.Slice(edges, func(i, j int) bool {
		return edges[i].savings[sourceid] > edges[j].savings[sourceid]
	})

	// Merging process
	for _, edge := range edges {

		inode, jnode := edge.inode, edge.jnode
		iRoute, jRoute := inode.route, jnode.route

		// Check inode is one of the nodes to consider
		if _, ok := nodes[inode.id]; !ok {
			continue
		}

		// Check jnode is one of the nodes to consider
		if _, ok := nodes[jnode.id]; !ok {
			continue
		}

		// Check routes are different
		if iRoute == jRoute {
			continue
		}

		// Check inode is linked to the sink and jnode linked to the source
		if !inode.linkEnd || !jnode.linkStart {
			continue
		}

		// Check travel distance of the new route
		if iRoute.cost+jRoute.cost+edge.cost-inode.to_depot-jnode.from_source > Tmax {
			continue
		}

		// Merge routes
		inode.linkEnd = false
		jnode.linkStart = false
		iRoute.cost = iRoute.cost + jRoute.cost + edge.cost - inode.to_depot - jnode.from_source
		iRoute.revenue += jRoute.revenue
		for _, node := range jRoute.nodes {
			node.route = iRoute
			iRoute.nodes = append(iRoute.nodes, node)
		}
		routes = remove(routes, jRoute)

		if len(routes) == n_vehicles {
			break
		}
	}

	// Sort routes
	sort.Slice(routes, func(i, j int) bool {
		return routes[i].revenue > routes[j].revenue
	})

	// Keep only the best routes
	routes = routes[:n_vehicles]

	// Calculate the cost of routes
	var revenue int64 = 0
	for _, route := range routes {
		revenue += route.revenue
	}

	// Return the routes
	return revenue, routes
}
