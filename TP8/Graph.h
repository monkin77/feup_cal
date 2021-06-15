/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <iostream>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;

constexpr auto INF = std::numeric_limits<double>::max();

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
    T info;
    std::vector<Edge<T> *> outgoing;  // adj
    std::vector<Edge<T> *> incoming;
    Edge<T> * addEdge(Vertex<T> *dest, double c, double f);
    Vertex(T in);

    bool visited;  // for path finding
    Edge<T> *path; // for path finding

public:
    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    friend class Graph<T>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {
}

template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *dest, double c, double f) {
    Edge<T> * e = new Edge<T>(this, dest, c, f);
    this->outgoing.push_back(e);
    dest->incoming.push_back(e);
    return e;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getAdj() const {
    return this->outgoing;
}


/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
    Vertex<T> * orig;
    Vertex<T> * dest;
    double capacity;
    double flow;
    Edge(Vertex<T> *o, Vertex<T> *d, double c, double f=0);

public:
    double getFlow() const;
    Vertex<T> *getDest() const;

    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double w, double f): orig(o), dest(d), capacity(w), flow(f){}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const {
    return dest;
}


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;
    Vertex<T>* findVertex(const T &inf) const;
    void resetFlows();
    bool findAugmentationPath(Vertex<T> *s, Vertex<T> *t);
    void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual);
    double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);
    void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double flow);
public:
    std::vector<Vertex<T> *> getVertexSet() const;
    Vertex<T> *addVertex(const T &in);
    Edge<T> *addEdge(const T &sourc, const T &dest, double c, double f=0);
    void fordFulkerson(T source, T target);

};

template <class T>
Vertex<T> * Graph<T>::addVertex(const T &in) {
    Vertex<T> *v = findVertex(in);
    if (v != nullptr)
        return v;
    v = new Vertex<T>(in);
    vertexSet.push_back(v);
    return v;
}

template <class T>
Edge<T> * Graph<T>::addEdge(const T &sourc, const T &dest, double c, double f) {
    auto s = findVertex(sourc);
    auto d = findVertex(dest);
    if (s == nullptr || d == nullptr)
        return nullptr;
    else
        return s->addEdge(d, c, f);
}

template <class T>
Vertex<T>* Graph<T>::findVertex(const T & inf) const {
    for (auto v : vertexSet)
        if (v->info == inf)
            return v;
    return nullptr;
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}


/**
 * Finds the maximum flow in a graph using the Ford Fulkerson algorithm
 * (with the improvement of Edmonds-Karp).
 * Assumes that the graph forms a flow network from source vertex 's'
 * to sink vertex 't' (distinct vertices).
 * Receives as arguments the source and target vertices (identified by their contents).
 * The result is defined by the "flow" field of each edge.
 */
template <class T>
void Graph<T>::fordFulkerson(T source, T target) {
    Vertex<T>* src = this->findVertex(source);
    Vertex<T>* dest = this->findVertex(target);

    cout << src->info << dest ->info << endl;

    this->resetFlows();
    int tot = 0;
    double f;

    while(this->findAugmentationPath(src, dest)){  // while there is an augmentation Path
        f = this->findMinResidualAlongPath(src, dest);
        this->augmentFlowAlongPath(src, dest, f);
        tot += f;
    }

    // return tot;
}

template <class T>
void Graph<T>::resetFlows() {
    for(Vertex<T>* vertex : this->vertexSet)
        for(Edge<T>* edge : vertex->outgoing){
            edge->flow = 0;
        }
}

template<class T>
bool Graph<T>::findAugmentationPath(Vertex<T> *s, Vertex<T> *t) {
    for(Vertex<T>* vertex : this->vertexSet)
        vertex->visited = false;

    s->visited = true;

    queue<Vertex<T> *> Q;
    Q.push(s);

    while(!Q.empty() && !t->visited){   // while Q isn't empty and destination not visited
        Vertex<T>* v = Q.front();
        Q.pop();

        for(Edge<T>* e : v->outgoing){  // direct residual edges
            this->testAndVisit(Q, e, e->dest, e->capacity - e->flow);   // Check if this edge can be incremented
        }
        for(Edge<T>* e : v->incoming){  // reverse residual edges
            this->testAndVisit(Q, e, e->orig, e->flow);
        }
    }
    return t->visited;  // whether the destination was visited  ( if there are more augmentation paths)
}

template <class T>
void Graph<T>::testAndVisit(std::queue<Vertex<T> *> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    if(!w->visited && residual > 0){
        w->visited = true;
        w->path = e;    // previous edge in the shortest path
        q.push(w);      // Add vertex to the queue
    }
}

template <class T>
double Graph<T>::findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;
    Vertex<T>* v = t;
    while(v != s){      // from dest to src
        Edge<T>* e = v->path;
        if(e->dest == v){   // direct residual edge
            f = min(f, e->capacity - e->flow);
            v = e->orig;
        }
        else{   // reverse residual edge
            f = min(f, e->flow);
            v = e->dest;    // because the edge is reversed
        }
    }
    return f;
}

template <class T>
void Graph<T>::augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double flow) {
    Vertex<T>* v = t;

    while(v != s){      // from dest to src
        Edge<T>* e = v->path;

        if(e->dest == v){   // direct residual edge
            e->flow += flow;
            v = e->orig;
        }
        else{   // reverse residual edge
            e->flow -= flow;
            v = e->dest;
        }
    }
}

#endif /* GRAPH_H_ */
