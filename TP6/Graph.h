/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"
#include <stack>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class FloydMatrixElement {
public:
    int distance;
    Vertex<T> *path;
    FloydMatrixElement(int distance, Vertex<T> *path){
        this->distance = distance;
        this->path = path;
    }
    FloydMatrixElement(){
        this->distance = INT_MAX;
        this->path = NULL;
    };
};

template <class T>
class Vertex {
    T info;						// content of the vertex
    std::vector<Edge<T> > adj;		// outgoing edges

    double dist = 0;
    Vertex<T> *path = NULL;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    bool visited = false;		// auxiliary field
    bool processing = false;	// auxiliary field

    void addEdge(Vertex<T> *dest, double w);

public:
    Vertex(T in);
    T getInfo() const;
    double getDist() const;
    Vertex *getPath() const;

    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
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
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
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
    Vertex<T> * dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);
    Edge(){
        this->dest = NULL;
        this->weight = 9999;
    };
    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set
    std::vector<std::vector<int>> next;
public:
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    // Fp06 - single source
    void unweightedShortestPath(const T &s);    //TODO...
    void dijkstraShortestPath(const T &s);      //TODO...
    void bellmanFordShortestPath(const T &s);   //TODO...
    std::vector<T> getPath(const T &origin, const T &dest) const;   //TODO...

    // Fp06 - all pairs
    void floydWarshallShortestPath();   //TODO...
    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;   //TODO...
    std::vector<vector<FloydMatrixElement<T>>> distMatrix;
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
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if ( findVertex(in) != NULL)
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
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    Vertex<T>* src = this->findVertex(orig);

    for(int i = 0; i < this->getNumVertex(); i++)
        this->vertexSet[i]->dist = INT_MAX;
    src->dist = 0;

    queue<Vertex<T> *> Q;
    Q.push(src);

    while(!Q.empty()){
        Vertex<T>* currVertex = Q.front();
        Q.pop();
        for(int i = 0; i < currVertex->adj.size(); i++){
            Vertex<T> *destVertex = currVertex->adj[i].dest;
            if(destVertex->dist == INT_MAX){
                destVertex->dist = currVertex->dist+1;
                destVertex->path = currVertex;
                Q.push(destVertex);
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    Vertex<T>* src = this->findVertex(origin);

    for(int i = 0; i < this->getNumVertex(); i++){
        this->vertexSet[i]->dist = INT_MAX;
        this->vertexSet[i]->path = NULL;
    }

    src->dist = 0;
    MutablePriorityQueue<Vertex<T> > Q;
    Q.insert(src);
    while(!Q.empty()){
        Vertex<T>* currVertex = Q.extractMin();
        for(Edge<T> edge : currVertex->adj){
            Vertex<T>* destVertex = edge.dest;
            int oldDist = destVertex->dist;
            if(destVertex->dist > currVertex->dist + edge.weight){
                destVertex->dist = currVertex->dist + edge.weight;
                destVertex->path = currVertex;
                if(oldDist == INT_MAX)
                    Q.insert(destVertex);
                else
                    Q.decreaseKey(destVertex);
            }
        }
    }

}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    Vertex<T> *src = this->findVertex(orig);

    for(int i = 0; i < this->getNumVertex(); i++){
        this->vertexSet[i]->dist = INT_MAX;
        this->vertexSet[i]->path = NULL;
    }

    src->dist = 0;

    for(int i = 1; i < this->getNumVertex(); i++){   // number of used edges
        for(Vertex<T>* vertex : this->vertexSet){
            for(Edge<T> edge : vertex->adj){
                Vertex<T> *destVertex = edge.dest;
                if(destVertex->dist > vertex->dist + edge.weight){
                    destVertex->dist = vertex->dist + edge.weight;
                    destVertex->path = vertex;
                }
            }
        }
    }

    for(Vertex<T>* vertex : this->vertexSet){
        for(Edge<T> edge : vertex->adj){
            Vertex<T>* destVertex = edge.dest;
            if(destVertex->dist > vertex->dist + edge.weight){
                cout << "There are cycles of negative weight" << endl;
            }
        }
    }

}


template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    std::vector<T> res;

    Vertex<T>* destVertex = this->findVertex(dest);
    Vertex<T>* currVertex = destVertex;

    stack<T> reversedPath;
    reversedPath.push(dest);
    while(true){
        currVertex = currVertex->path;
        reversedPath.push(currVertex->info);
        if(currVertex->info == origin)
            break;
    }

    while(!reversedPath.empty()){
        res.push_back(reversedPath.top());
        reversedPath.pop();
    }

    return res;
}



/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    /*
    //FloydMatrixElement<T> graphMatrix[this->getNumVertex()][this->getNumVertex()];
    vector<vector<FloydMatrixElement<T> > > graphMatrix(this->getNumVertex(), vector<FloydMatrixElement<T>>(this->getNumVertex()));

    for(int i = 0; i < graphMatrix.size(); i++){    // initialize the matrix
        for(int j = 0; j < graphMatrix[0].size(); j++){
            if(i == j)
                graphMatrix[i][j] = FloydMatrixElement<T>(0, NULL);
            else{
                Vertex<T>* vertex = this->vertexSet[i];
                bool foundEdge = false;
                for(Edge<T> edge : vertex->adj){
                    Vertex<T> *destVertex = edge.dest;
                    if(this->vertexSet[j] == destVertex){   // There exists an edge (from i to j)
                        graphMatrix[i][j] = FloydMatrixElement<T>(edge.weight, destVertex);
                        foundEdge = true;
                        break;
                    }
                }
                if(!foundEdge)
                    graphMatrix[i][j] = FloydMatrixElement<T>(INT_MAX, NULL);;
            }
        }
    }

    for(int k = 0; k < this->getNumVertex(); k++){  // intermediate vertex index
        for(int i = 0; i < this->getNumVertex(); i++){  // iterate all vertices as source
            for(int j = 0; j < this->getNumVertex(); j++){  // iterate all vertices as destination
                Vertex<T> *vertexK = this->vertexSet[k];
                Vertex<T> *vertexI = this->vertexSet[i];
                Vertex<T> *vertexJ = this->vertexSet[j];
                Edge<T> edgeItoK, edgeKToJ;
                bool found1 = false, found2 = false;
                for(Edge<T> edge : vertexI->adj){
                    if(edge.dest == vertexK){
                        edgeItoK = edge;
                        found1 = true;
                        break;
                    }
                }
                if(!found1)
                    continue;
                for(Edge<T> edge : vertexK->adj){
                    if(edge.dest == vertexJ){
                        edgeKToJ = edge;
                        found2 = true;
                        break;
                    }
                }
                if(!found2)
                    continue;

                FloydMatrixElement<T> currMatrixElem = graphMatrix[i][j];
                if(edgeItoK.weight + edgeKToJ.weight < currMatrixElem.distance){
                    graphMatrix[i][j].distance = edgeItoK.weight + edgeKToJ.weight;
                    graphMatrix[i][j].path = vertexK;
                }
            }
        }
    }

    this->distMatrix = graphMatrix;
*/
    std::vector<std::vector<double>> dist(vertexSet.size(), std::vector<double>(vertexSet.size(), 1E6));
    next = std::vector<std::vector<int>>(vertexSet.size(), std::vector<int>(vertexSet.size(), -1)); // vertex indices matrix
    for (int i = 0; i < vertexSet.size(); ++i) {
        Vertex<T>* v = vertexSet[i];
        for (Edge<T> e : v->adj) {
            int j;
            for (int k = 0; k < vertexSet.size(); ++k)
                if (vertexSet[k]->info == e.dest->info) {
                    j = k;
                    break;
                }
            dist[i][j] = e.weight;
            next[i][j] = j;
        }
    }
    for (int i = 0; i < vertexSet.size(); ++i) {
        dist[i][i] = 0;
        next[i][i] = i;
    }
    for (int k = 0; k < vertexSet.size(); ++k)
        for (int i = 0; i < vertexSet.size(); ++i)
            for (int j = 0; j < vertexSet.size(); ++j)
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
}

template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
    /*
    std::vector<T> res;

    int destIdx = dest-1;
    int srcIdx = orig-1;

    int i = srcIdx, j = destIdx;

    stack<T> reversedPath;
    reversedPath.push(dest);
    while(true){
        FloydMatrixElement<T> elem = this->distMatrix[i][j];
        if(elem.path == NULL){
            break;
        }
        reversedPath.push(elem.path->info);
        j = elem.path->info-1;
    }

    while(!reversedPath.empty()){
        res.push_back(reversedPath.top());
        reversedPath.pop();
    }

    return res;
     */
    std::vector<T> res;
    int u, v;
    for (int i = 0; i < vertexSet.size(); ++i) {
        if (vertexSet[i]->info == orig) u = i;
        if (vertexSet[i]->info == dest) v = i;
    }
    if (next[u][v] == -1) return res;
    res.push_back(vertexSet[u]->info);
    while (u != v) {
        u = next[u][v];
        res.push_back(vertexSet[u]->info);
    }
    return res;
}


#endif /* GRAPH_H_ */
