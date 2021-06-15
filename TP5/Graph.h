/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	std::vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	bool processing;       // auxiliary field used by isDAG
	int indegree;          // auxiliary field used by topsort

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
};

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v,  std::vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
    std::vector<T> dfs() const;
    std::vector<T> bfs(const T &source) const;
    std::vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if(this->findVertex(in) == NULL){
        Vertex<T> *vert = new Vertex<T>(in);
        this->vertexSet.push_back(vert);
        return true;
    }
    return false;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    Vertex<T> *srcVert = findVertex(sourc);
    Vertex<T> *destVert = findVertex(dest);
    if((srcVert != NULL) && (destVert != NULL)){
        srcVert->addEdge(destVert, w);
        return true;
    }
    return false;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    Edge<T> newEdge(d, w);
    this->adj.push_back(newEdge);
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> *srcVert = findVertex(sourc);
    Vertex<T> *destVert = findVertex(dest);
    if((srcVert != NULL) && (destVert != NULL)){
        if(srcVert->removeEdgeTo(destVert))
            return true;
        return false;
    }

    return false;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    for(auto it = this->adj.begin(); it != this->adj.end(); it++){
        if(it->dest == d){
            this->adj.erase(it);
            return true;
        }
    }
    return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    Vertex<T> *vert = findVertex(in);
    if(vert != NULL){
        for(int i = 0; i < this->vertexSet.size(); i++){
            this->vertexSet[i]->removeEdgeTo(vert);     // remove edges in both directions that
            vert->removeEdgeTo(this->vertexSet[i]);     // include vert
        }
        for(auto it = this->vertexSet.begin(); it != this->vertexSet.end(); it++){
            if(*it == vert){
                this->vertexSet.erase(it);
                return true;
            }
        }
    }

    return false;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::dfs() const {
    std::vector<T> res;

    for(auto& vert : this->vertexSet){
        vert->visited = false;
    }

    for(auto& vert : this->vertexSet){
        if(!vert->visited){
            this->dfsVisit(vert, res);
        }
    }

    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> & res) const {
    v->visited = true;
    res.push_back(v->info);
    for(auto& edges : v->adj){
        Vertex<T> *adjVert = edges.dest;
        if(!adjVert->visited)
            dfsVisit(adjVert, res);
    }
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::bfs(const T & source) const {
    std::vector<T> res;

    for(auto& vert : this->vertexSet)
        vert->visited = false;

    queue<Vertex<T> *> Q;

    Vertex<T> *srcVert = findVertex(source);
    srcVert->visited = true;

    Q.push(srcVert);

    while(!Q.empty()){
        Vertex<T> *currVert = Q.front();
        Q.pop();
        res.push_back(currVert->info);
        for(auto& edge : currVert->adj){
            Vertex<T> *adjVert = edge.dest;
            if(!adjVert->visited){
                Q.push(adjVert);
                adjVert->visited = true;
            }
        }
    }

    return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
std::vector<T> Graph<T>::topsort() const {
    std::vector<T> res;

    for(auto& vert : this->vertexSet)
        vert->indegree = 0;

    for(auto& vert : this->vertexSet){
        for(auto& edge : vert->adj){
            Vertex<T> *adjVert = edge.dest;
            adjVert->indegree++;
        }
    }

    queue<Vertex<T> *> C;

    for(auto& vert : this->vertexSet)
        if(vert->indegree == 0)
            C.push(vert);

    list<Vertex<T> *> t;

    while(!C.empty()){
        Vertex<T> *currVert = C.front();
        C.pop();
        t.push_front(currVert);
        res.push_back(currVert->info);
        for(auto& edge : currVert->adj){
            Vertex<T> *adjVert = edge.dest;
            adjVert->indegree--;
            if(adjVert->indegree == 0)
                C.push(adjVert);
        }
    }

    if(t.size() != this->vertexSet.size())
        return vector<T>();

    return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T>
int Graph<T>::maxNewChildren(const T & source, T &inf) const {
    int maxNumChildren = 0;
    int currNumChildren;
    for(auto& vert : this->vertexSet)
        vert->visited = false;

    queue<Vertex<T> *> Q;

    Vertex<T> *srcVert = findVertex(source);
    srcVert->visited = true;

    Q.push(srcVert);

    while(!Q.empty()){
        Vertex<T> *currVert = Q.front();
        Q.pop();
        currNumChildren = 0;
        for(auto& edge : currVert->adj){
            Vertex<T> *adjVert = edge.dest;
            if(!adjVert->visited){
                Q.push(adjVert);
                adjVert->visited = true;
                currNumChildren++;
            }
        }
        if(currNumChildren > maxNumChildren){
            maxNumChildren = currNumChildren;
            inf = currVert->info;
        }
    }

    return maxNumChildren;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {

    for(auto& vert : this->vertexSet){
        vert->visited = false;
    }

    for(auto& vert : this->vertexSet){
        if(!vert->visited){
            for(auto& vert2 : this->vertexSet)
                vert2->processing = false;  // processing tells if a certain vertix was already seen on the current tree (top down)
            if(!this->dfsIsDAG(vert))
                return false;
        }
    }

    return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    v->visited = true;
    v->processing = true;
    bool isDAG = true;
    for(auto& edges : v->adj){
        Vertex<T> *adjVert = edges.dest;
        if(adjVert->processing)
            return false;
        if(!adjVert->visited){
            isDAG = dfsIsDAG(adjVert);
            if(!isDAG)
                return false;
        }
    }

    return true;
}

#endif /* GRAPH_H_ */
