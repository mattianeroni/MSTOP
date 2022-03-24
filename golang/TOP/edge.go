package main

type Edge struct {
	inode, jnode *Node
	cost         float64
	savings      map[int]float64
}
