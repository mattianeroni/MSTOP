package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strconv"
	"strings"
)

// Initialise the color of nodes
var SOURCES_COLORS = [...]string{"#8FDDF4", "#8DD631", "#A5A5A5", "#DB35EF", "#8153AB"}

const NODES_COLOR = "#FDDD71"
const DEPOT_COLOR = "#F78181"

type Problem struct {
	name       string      // The name of the problem
	n_nodes    int64       // The number of nodes
	n_vehicles int64       // The number of vehicles
	Tmax       float64     // The max distance each vehicle can run
	dists      [][]float64 // The matrix of distances between nodes
	sources    []*Node     // The sources
	nodes      []*Node     // The nodes
	depot      *Node       // The depot
	edges      []Edge      // The edges
}

// This function returns the euclidean distance between two nodes
func euclidean(node1, node2 Node) float64 {
	return math.Sqrt(math.Pow(node1.x-node2.x, 2) + math.Pow(node1.y-node2.y, 2))
}

// This function is used to read a single source TOP problem instance
func read_single_source(filename string, path string) Problem {
	// Open file
	f, err := os.Open(path + filename)
	if err != nil {
		log.Fatal(err)
	}

	// Remember to close the file at the end of the program
	defer f.Close()

	// Read the file line by line using scanner
	scanner := bufio.NewScanner(f)

	// Read n, m, and Tmax
	scanner.Scan()
	n_nodes, _ := strconv.ParseInt(strings.Fields(scanner.Text())[1], 10, 64)
	scanner.Scan()
	n_vehicles, _ := strconv.ParseInt(strings.Fields(scanner.Text())[1], 10, 64)
	scanner.Scan()
	Tmax, _ := strconv.ParseFloat(strings.Fields(scanner.Text())[1], 64)

	// Read the nodes
	sources := make([]*Node, 0)
	nodes := make([]*Node, 0)
	var depot *Node
	var lineid int = 0
	for scanner.Scan() {
		words := strings.Fields(scanner.Text())
		x, _ := strconv.ParseFloat(words[0], 64)
		y, _ := strconv.ParseFloat(words[1], 64)
		revenue, _ := strconv.ParseInt(words[2], 10, 64)
		if lineid == 0 {
			sources = append(sources, &Node{id: lineid, x: x, y: y, revenue: revenue, issource: true, isdepot: false, vehicles: n_vehicles})
		} else if lineid == int(n_nodes-1) {
			depot = &Node{id: lineid, x: x, y: y, revenue: revenue, issource: false, isdepot: true}
		} else {
			nodes = append(nodes, &Node{id: lineid, x: x, y: y, revenue: revenue, issource: false, isdepot: false})
		}
		lineid++
	}

	// Isolate error of the scanner
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	// Initialise matrix of distances and edges
	dists := make([][]float64, n_nodes)
	edges := make([]Edge, 0)

	// Compute matrix of distances and edges
	full_list := append(sources, nodes...)

	full_list = append(full_list, depot)

	for i, node1 := range full_list {
		dists[i] = make([]float64, n_nodes)
		for j, node2 := range full_list {
			dists[i][j] = euclidean(*node1, *node2)
			if !node1.isdepot && !node2.issource {
				edges = append(edges, Edge{inode: node1, jnode: node2, cost: dists[i][j]})
			}
		}

	}

	// Return the problem instance
	return Problem{name: filename, n_nodes: n_nodes, n_vehicles: n_vehicles, Tmax: Tmax, nodes: nodes, sources: sources, depot: depot, dists: dists, edges: edges}
}







func read_multi_source(filename string, path string) Problem {
	// Open file
	f, err := os.Open(path + filename)
	if err != nil {
		log.Fatal(err)
	}
	// Remember to close the file at the end of the program
	defer f.Close()

	// Read the file line by line using scanner
	scanner := bufio.NewScanner(f)

	// Read n, m, and Tmax
	scanner.Scan()
	n_nodes, _ := strconv.ParseInt(strings.Fields(scanner.Text())[1], 10, 64)
	scanner.Scan()
	n_vehicles, _ := strconv.ParseInt(strings.Fields(scanner.Text())[1], 10, 64)
	scanner.Scan()
	Tmax, _ := strconv.ParseFloat(strings.Fields(scanner.Text())[1], 64)

	// Read the nodes
	sources := make([]*Node, 0)
	nodes := make([]*Node, 0)
	var depot *Node
	var lineid int = 0
	for scanner.Scan() {
		words := strings.Fields(scanner.Text())
		x, _ := strconv.ParseFloat(words[0], 64)
		y, _ := strconv.ParseFloat(words[1], 64)
		revenue, _ := strconv.ParseInt(words[2], 10, 64)
		source, _ := strconv.ParseInt(words[3], 10, 64)
		vehicles, _ := strconv.ParseInt(words[4], 10, 64)

		if lineid == int(n_nodes-1) {
			depot = &Node{id: lineid, x: x, y: y, revenue: revenue, issource: false, isdepot: true, assigned: false}
		} else {
			if source == 1 {
				sources = append(sources, &Node{id: lineid, x: x, y: y, revenue: revenue, issource: true, isdepot: false, vehicles: vehicles, assigned: false})
			} else {
				nodes = append(nodes, &Node{id: lineid, x: x, y: y, revenue: revenue, issource: false, isdepot: false, assigned: false})
			}
		}
		lineid++
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	// Calculate matrix of distances and edges
	dists := make([][]float64, n_nodes)
	edges := make([]Edge, 0)

	full_list := append(sources, nodes...)
	full_list = append(full_list, depot)
	for i, node1 := range full_list {
		dists[i] = make([]float64, n_nodes)
		for j, node2 := range full_list {
			dists[i][j] = euclidean(*node1, *node2)
			if !node1.isdepot && !node2.issource {
				edges = append(edges, Edge{inode: node1, jnode: node2, cost: dists[i][j]})
			}
		}
	}

	return Problem{name: filename, n_nodes: n_nodes, n_vehicles: n_vehicles, Tmax: Tmax, nodes: nodes, sources: sources, depot: depot, dists: dists}
}

// This function exports a problem
func export_problem(problem Problem, path string) {

	file, err := os.Create(path + problem.name)
	if err != nil {
		return
	}
	defer file.Close()

	// Export the sources
	for i, source := range problem.sources {
		file.WriteString(fmt.Sprint(source.id) + "\t" + fmt.Sprint(source.x) + "\t" + fmt.Sprint(source.y) + "\t" + fmt.Sprint(source.revenue) + "\t" + SOURCES_COLORS[i] + "\n")
	}

	// Export the nodes
	for _, node := range problem.nodes {
		file.WriteString(fmt.Sprint(node.id) + "\t" + fmt.Sprint(node.x) + "\t" + fmt.Sprint(node.y) + "\t" + fmt.Sprint(node.revenue) + "\t" + NODES_COLOR + "\n")
	}

	// Export the depot
	depot := problem.depot
	file.WriteString(fmt.Sprint(depot.id) + "\t" + fmt.Sprint(depot.x) + "\t" + fmt.Sprint(depot.y) + "\t" + fmt.Sprint(depot.revenue) + "\t" + DEPOT_COLOR + "\n")
}

// This function exports a mapping
func export_mapping(mapping Mapping, problem Problem, path string) {

	file, err := os.Create(path + problem.name)
	if err != nil {
		return
	}
	defer file.Close()

	// Export the sources
	for i, source := range problem.sources {
		file.WriteString(fmt.Sprint(source.id) + "\t" + fmt.Sprint(source.x) + "\t" + fmt.Sprint(source.y) + "\t" + fmt.Sprint(source.revenue) + "\t" + SOURCES_COLORS[i] + "\n")
	}

	// Export the nodes
	for _, node := range problem.nodes {
		for j, source := range problem.sources {
			if mapping[source.id][node.id] == 1 {
				file.WriteString(fmt.Sprint(node.id) + "\t" + fmt.Sprint(node.x) + "\t" + fmt.Sprint(node.y) + "\t" + fmt.Sprint(node.revenue) + "\t" + SOURCES_COLORS[j] + "60\n")
			}
		}
	}

	// Export the depot
	depot := problem.depot
	file.WriteString(fmt.Sprint(depot.id) + "\t" + fmt.Sprint(depot.x) + "\t" + fmt.Sprint(depot.y) + "\t" + fmt.Sprint(depot.revenue) + "\t" + DEPOT_COLOR + "\n")
}

func export_solution(routes []Route, mapping Mapping, problem Problem, path string) {

}
