#include "graphviewer.h"
#include <fstream>

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex3() {
    // Instantiate GraphViewer
    GraphViewer gv;
    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    ifstream fin;
    fin.open("../TP7_graphviewer/resources/map1/nodes.txt");
    int numNodes;
    fin >> numNodes;
    for (int i = 0; i < numNodes; ++i) {
        int id, x, y;
        fin >> id >> x >> y;
        gv.addNode(id, sf::Vector2f(x, y));
    }

    fin.close();
    fin.open("../TP7_graphviewer/resources/map1/edges.txt");
    int numEdges;
    fin >> numEdges;
    for (int i = 0; i < numEdges; ++i) {
        int id, n1, n2;
        fin >> id >> n1 >> n2;
        gv.addEdge(id, gv.getNode(n1), gv.getNode(n2), GraphViewer::Edge::UNDIRECTED);
    }

    // Create window
    gv.createWindow(600, 600);

    // Join viewer thread (blocks till window closed)
    gv.join();
}

