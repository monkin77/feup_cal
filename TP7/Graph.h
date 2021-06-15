/*
 * Graph.h.
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    friend double spanningTreeCost(const std::vector<Vertex<int>*> &);
    T info;                 // contents
    std::vector<Edge<T> *> adj;  // outgoing edges

    bool visited;
    double dist = 0;
    Vertex<T> *path = nullptr;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    // Fp07 - minimum spanning tree (Kruskal)
    int id;
    int rank;

    Edge<T> * addEdge(Vertex<T> *dest, double w);
public:
    Vertex(T in);
    bool operator<(const Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    T getInfo() const;
    double getDist() const;
    Vertex *getPath() const;
    friend class Graph<T>;
    friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *d, double w) {
    Edge<T> *e = new Edge<T>(this, d, w);
    adj.push_back(e);
    return e;
}

template <class T>
bool Vertex<T>::operator<(const Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
    Vertex<T> *orig; 		    // Fp07
    Vertex<T> * dest;           // destination vertex
    double weight;              // edge weight
    bool selected = false;      // Fp07
    Edge<T> *reverse = nullptr; // Fp07
public:
    Edge(Vertex<T> *o, Vertex<T> *d, double w);
    friend class Graph<T>;
    friend class Vertex<T>;
    bool operator<(Edge<T> *edge);
    double getWeight() const;
    Vertex<T> *getOrig() const;
    Vertex<T> *getDest() const;
};

template <class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double w): orig(o), dest(d), weight(w) {}

template <class T>
double Edge<T>::getWeight() const {
    return weight;
}

template <class T>
Vertex<T> *Edge<T>::getOrig() const {
    return orig;
}

template <class T>
Vertex<T> *Edge<T>::getDest() const {
    return dest;
}

/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    // Fp07 (Kruskal's algorithm)
    void makeSet(Vertex<T> * x);
    Vertex<T> * findSet(Vertex<T> * x);
    void linkSets(Vertex<T> * x, Vertex<T> * y);
    void dfsKruskalPath(Vertex<T>* vertex);


public:
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w);
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;
    ~Graph();

    // Fp07 - minimum spanning tree
    std::vector<Vertex<T>*> calculatePrim();
    std::vector<Vertex<T>*> calculateKruskal();
};


template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return nullptr;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    Vertex<T> *first = findVertex(sourc);
    Vertex<T> *second = findVertex(dest);
    if (first == nullptr || second == nullptr)
        return false;
    Edge<T> *e1 = first->addEdge(second, w);
    Edge<T> *e2 = second->addEdge(first, w);
    e1->reverse = e2;
    e2->reverse = e1;

    return true;
}

template <class T>
Graph<T>::~Graph() {
    /*
	for (auto v : vertexSet) {
		for (auto e : v->adj)
			delete e;
		delete v;
	}
    */
}

/**************** Minimum Spanning Tree  ***************/

template <class T>
std::vector<Vertex<T>* > Graph<T>::calculatePrim() {
    if(this->vertexSet.size() == 0)
        return this->vertexSet;

    for(Vertex<T> *vertex : this->getVertexSet()){
        vertex->path = NULL;
        vertex->dist = INF;
        vertex->visited = false;
    }

    MutablePriorityQueue<Vertex<T>> Q;
    this->getVertexSet()[0]->dist = 0;
    Q.insert(this->getVertexSet()[0]);

    while(!Q.empty()){
        Vertex<T> *currVert = Q.extractMin();
        currVert->visited = true;
        for(auto edge : currVert->adj){
            Vertex<T> *vertexDest = edge->dest;
            if(!vertexDest->visited){   // update the distance on the queue
                double oldDist = vertexDest->dist;
                if(edge->weight < vertexDest->dist){
                    vertexDest->dist = edge->weight;
                    vertexDest->path = currVert;
                    if(oldDist == INF){         // insert for the 1st time
                        Q.insert(vertexDest);
                    }
                    else                        // update the distance on the queue
                        Q.decreaseKey(vertexDest);
                }
            }
        }
    }

    return vertexSet;
}

/**
 * Disjoint sets operations (page 571, Introduction to Algorithms) for Kruskal's algorithm.
 */

template <class T>
void Graph<T>::makeSet(Vertex<T> * x) {
    x->path = x;
    x->rank = 0;
}

template <class T>
void Graph<T>::linkSets(Vertex<T> * x, Vertex<T> * y) {     // rank represents the number of levels the tree has
    if (x->rank > y->rank)
        y->path = x;
    else {
        x->path = y;
        if (x->rank == y->rank)
            y->rank++;
    }
}

template <class T>
Vertex<T> * Graph<T>::findSet(Vertex<T> * x) {  // find the starting vertex of the tree
    if (x != x->path)
        // return findSet(x->path);
        x->path = findSet(x->path);     // not sure if this is the problem but the paths are being changed to the parent (probably)
    return x->path;
}

template <class T>
bool Edge<T>::operator<(Edge<T> *edge) {    // by default the greatest
    return this->weight >= edge->weight;
}

/**
 * Implementation of Kruskal's algorithm to find a minimum
 * spanning tree of an undirected connected graph (edges added with addBidirectionalEdge).
 * It is used a disjoint-set data structure to achieve a running time O(|E| log |V|).
 * The solution is defined by the "path" field of each vertex, which will point
 * to the parent vertex in the tree (nullptr in the root).
 */
template <class T>
std::vector<Vertex<T>*> Graph<T>::calculateKruskal() {
    auto comp = [](const Edge<T> *e1, const Edge<T> *e2) { return e1->weight > e2->weight; };
    std::priority_queue<Edge<T>*, std::vector<Edge<T> *>, decltype(comp)> h(comp);
    // priority_queue<Edge<T> *> h;

    int counter = 0;
    for(Vertex<T>* vertex : this->vertexSet){
        this->makeSet(vertex);  // make vertex a set instance
        vertex->id = counter++;
        vertex->visited = false;
    }

    for(Vertex<T>* vertex : this->vertexSet){
        for(Edge<T> *edge : vertex->adj){
            edge->selected = false;
            if(edge->orig->id < edge->dest->id)     // force to see the graph in only one direction
                h.push(edge);
        }
    }

    int numVertices = this->vertexSet.size();
    int edgesAccepted = 0;

    while( (edgesAccepted < numVertices -1) && !h.empty() ){
        Edge<T> *e = h.top();
        h.pop();
        Vertex<T> *edgeSrc = e->orig;
        Vertex<T> *edgeDest = e->dest;
        Vertex<T> *srcTreeTop = this->findSet(edgeSrc);
        Vertex<T> *destTreeTop = this->findSet(edgeDest);
        if(srcTreeTop != destTreeTop){      /* only process this edge if the 2 vertices are not connected already, because we would have a cycle otherwise */
            this->linkSets(srcTreeTop, destTreeTop);
            e->selected = true;
            e->reverse->selected = true;
            edgesAccepted++;
        }
    }

    this->dfsKruskalPath(this->vertexSet[0]);

    return this->vertexSet;
}

/**
 * Auxiliary function to set the "path" field to make a spanning tree.
 */
template <class T>
void Graph<T>::dfsKruskalPath(Vertex<T>* vertex) {
    vertex->visited = true;
    for(Edge<T> *edge : vertex->adj){
        if(edge->selected && !edge->dest->visited){
            Vertex<T> *src = edge->orig;
            Vertex<T> *dest = edge->dest;
            dest->path = src;
            dfsKruskalPath(dest);
            if(dest->path == src && src->path == dest)
                src->path = nullptr;
        }
    }
}

#endif /* GRAPH_H_ */
