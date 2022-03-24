# Team Orienteering Problem Benchmark Instances

In this folder all the single-source and multi-source Team Orienteering Problem instances are kept.

### Directories
- *single*: The single-source instances
- *multi*: The multi-source instances

### Files structure

The first three lines contains the following data: 
-	n = number of nodes
-	m = number of vehicles / paths
-	Tmax = maximum travel distance per vehicles / time budget for path


The remaining lines contain the data of each node. 
For each node, the line contains the following data:
- x	= x-coordinate
- y	= y-coordinate
- Revenue	= score or revenue
- s (only in multi-source) = 1 if the node is a source, and 0 otherwise
- v (only in multi-source) = the number of vehicles / paths starting from that source

### Note
-	The first point is the starting point.
-	The last point is the ending point.
-	The Euclidian distance is used.

### Reference
All these instances come from:
- *Chao I. Algorithms and solutions to multi-level vehicle routing problems. Ph.D. Dissertation, Applied Mathematics Program, University of Maryland, College Park, USA, 1993.*
- *Chao I, Golden B, Wasil E. Theory and Methodology - The Team Orienteering Problem. European Journal of Operational Research 1996a; 88; 464-474.*
