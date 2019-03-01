#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

/*******************************************************************************
			Sam Bowmer	shortest path A* algorithm
*******************************************************************************/
//~~~~~~~~~~~~~~~~~~~~~~~ GLOBAL VARIABLES & CONSTANTS ~~~~~~~~~~~~~~~~~~~~~~~~~
const int arraySize = 1000;
int nVertices;
int nEdges;
struct vertex {
	char label;
	int x;
	int y;
};
struct path {
	float dist;				//value of the distance 
	float heuristic;		//the euclidean distance from the path's reached vertex to the goal
	char via[arraySize];	//labels of vertices along the path
	int viaEnd;				//the index to mark the end of via[]
};
char start, g;			//label of start and goal vertices
vertex goal;			//goal vertices
vertex C[arraySize];	//Candidates
vertex S[arraySize];	//Selected
path paths[arraySize];	//Priority queue of paths (this is a heap so the array starts at 1 to make things easier)
int pathsEnd = 1;		//the index to mark the end of distances array paths[]
int CEnd = 0; 			//the index to mark the end of candidates array C[]
int SEnd = 0;			//the index to mark the end of selected array S[]
char fileName[arraySize];

//~~~~~~~~~~~~~~~~~~~~~~~~~~ FUNCTION PROTOTYPES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int toInt(char input);						//converts alphabet characters to integers (a to 0, b to 1 and so on)
char toChar(int input); 					//does the reverse of toChar
vertex getVertex(char toFind);				//finds the vertex with the inputted label from candidates array C()
void removeVertex(char topathelete);		//removes the vertex with the inputted label from candidates array C()
void CtoS(char toMove);						//moves Vertex from label toMove from C to S
float  getHeuristic(vertex to);				//returns the heuristic (euclidian distance from to to goal)
void insertpath(path p);					//inserts value d into paths
void siftUp(int position);					//sifts up float at position in paths
void removePath(int toRemove);				//removes path at toRemove from paths
void swap(int left, int right);				//swaps two paths in paths[]

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char** argv) {
	cout << "File name: ";
	cin >> fileName;

	ifstream fin;
	
	fin.open(fileName);
	if(!fin.good()) {
		cout<<"File not good"<<endl;
		return 1;
	}
	
	//read in number of C 
	fin >> nVertices;
	CEnd = nVertices;
	fin >> nEdges;
	
	//initialize adjacency matrix
	int adjMatrix[nVertices][nVertices];
	for (int i=0; i < nVertices; i++) {
		for (int j=0; j < nVertices; j++) {
			adjMatrix[i][j] = 0;
		}
	}
	
	//read in vertices
	for (int i=0; i<nVertices; i++) {
		fin >> C[i].label;
		fin >> C[i].x;
		fin >> C[i].y;
	}
	
	//read each edge into adjacency matrix
	char to, from;
	int value;
	for (int i=0; i<nEdges; i++) {
		fin >> to;
		fin >> from;
		fin >> value;
		
		adjMatrix[toInt(to)][toInt(from)] = value;
//		adjMatrix[toInt(from)][toInt(to)] = value;
	}
	
	//read in start and goal vertices
	fin >> start;
	fin >> g;
	goal = getVertex(g);
	
	//move start from C to S
	CtoS(start);
	int timesReachedGoal = 0;
	
	//A* algorithm:
	while(timesReachedGoal != 2) {
		//add all distanes from last value in s[] to next reachable vertex into paths[]
		for (int i=0; i<nVertices; i++) {
			if(adjMatrix[toInt(S[SEnd-1].label)][i] > 0) {						//if the distance between the last value in s[] and i > 0
				path temp = paths[1];	
				temp.via[0]=start;
				temp.dist += adjMatrix[toInt(S[SEnd-1].label)][i];			//	append distance of the lastest edge
				temp.heuristic = getHeuristic(getVertex(toChar(i)));				//	overwrite with new heuristic
				temp.via[temp.viaEnd] = toChar(i);								//	add the new via[] to temp
				temp.viaEnd++;
				insertpath(temp);												//	insert temp into paths
			}
		}
		//remove path[0]
		removePath(1);
		//chose lowest value in paths[] and move the vertex at the end of it's via[] from C to S
		CtoS( paths[1].via[paths[1].viaEnd-1] );
		
			
		//check if the the next vertex is the goal
		if (paths[1].via[paths[1].viaEnd-1] == g) {	
			timesReachedGoal++;
			if (timesReachedGoal == 1) {							
				cout << "\nReached goal, shortest path is:\n";
			}
			else {
				cout << "\nSecond shortest path is:\n";	
			}
			int shortestPath = 0;
			cout << paths[1].via[0];
			for (int i=1; i<paths[1].viaEnd; i++) {
				shortestPath += adjMatrix[toInt(paths[1].via[i-1])][toInt(paths[1].via[i])];
				cout << " --> " << paths[1].via[i];
			}
			cout << "\nThe length of this path is " << shortestPath << endl;
		}
	}
	
	return 0;
}

int toInt(char input) {
	return (int)input - 97;
}

char toChar(int input) {
	input += 97;
	return (char)input;
}

vertex getVertex(char toFind) {
	for (int i=0; i<CEnd; i++) {
		if(toFind == C[i].label) {
			return C[i];
		}
	}
}

void removeVertex(char toRemove) {
	for (int i=0; i<CEnd; i++) {
		if(toRemove == C[i].label) {
			for(int j=i; j<CEnd-1; j++) {
				C[j] = C[j+1];
			}
			CEnd--;
			return;
		}
	}
}

void CtoS(char toMove) {
	S[SEnd] = getVertex(toMove);
	SEnd++;
	removeVertex(toMove);
}

float getHeuristic(vertex to) {
	return sqrt( (goal.x-to.x)*(goal.x-to.x) + (goal.y-to.y)*(goal.y-to.y) );
}

void insertpath(path p) {
	paths[pathsEnd] = p;
	siftUp(pathsEnd);
	pathsEnd++;
}

void siftUp(int position) {												
	if (position == 1) return;
	int parent = position/2;
	if ( 
		(paths[parent].dist+paths[parent].heuristic) 
		< (paths[position].dist+paths[position].heuristic)
	) {			//if this path's distance + heuristic is less than that of its parent path
		return;	
	}
	else {
		swap(paths[parent],paths[position]);
		siftUp(position);
	} 
}

void removePath(int toRemove) {
	for(int i=0; i<pathsEnd-1; i++) {
		paths[i] = paths[i+1];
	}
	pathsEnd--;
}

void swap(int left, int right) {
	path temp = paths[left];
	paths[left] = paths[right];
	paths[right] = temp;
}
