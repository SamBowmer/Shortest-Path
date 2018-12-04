This program uses an a* algorithm to find the shortest possible path between two points. 
It does this by first reading in all vertices and storing them in an array C[], 
then reading edges into an adjacency matrix adjMatrix[][]. 

The program then reads in the start and goal vertices, storing the goal vertex in its own 
variable and moving the start vertex from candidates C[] to selected S[]. 

The program then loops, finding all vertices reachable from the last selected vertex 
S[SEnd-1] (in the case of the first iteration of the loop this would be the start vertex) 
and storing: 
1.  The length of the edge(s) to that vertex plus the heuristic (the heuristic being the 
    euclidean distance from that vertex to the goal vertex) 
2.  The vertices visited along the path taken in an array of vertex labels (chars) via[]. 
    In a struct path that is then appended to an ordered heap paths[]. 

The program then takes the shortest possible path (paths[1]) and moves the vertex reached 
by it from C[] to S[] before removing it. This is repeated until the goal vertex is added 
to S[] twice (for the first and second shortest path) and those paths that reached the 
goal are printed. 