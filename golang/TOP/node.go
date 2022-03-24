package main

type Node struct {
	id       int     // The unique id of the node
	x, y     float64 // The coordinates in space
	revenue  int64   // The revenue associated to the node
	vehicles int64   // The number of vehicles starting from this node is it is a source
	issource bool    // A boolean indicator that says if the node is a source
	isdepot  bool    // A boolean indicator that says if the node is the depot

	// Attributes used by the mapper
	preferences []Preference // The set of preferences
	nodes       map[int]*Node
	assigned    bool

	// Attributes used by the savings based heuristic
	from_source, to_depot float64
	route                 *Route
	linkStart, linkEnd    bool
}

type Preference struct {
	vote float64
	node *Node
}
