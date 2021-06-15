#include "graphviewer.h"
#include <fstream>
#include <iostream>

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex3() {
    // Instantiate GraphViewer
    GraphViewer gv;

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    ifstream fin;
    fin.open("./resources/map1/nodes.txt");

    if(!fin.is_open()){
        cout << "Error opening nodes.txt" << endl;
    }

    int numNodes;
    fin >> numNodes;

    string line;
    int id;
    float x, y;
    for(int i = 0; i < numNodes; i++){
        fin >> id >> x >> y;
        gv.addNode(id, sf::Vector2f(x, y)); // Create nodes
    }

    fin.close();

    fin.open("./resources/map1/edges.txt");

    if(!fin.is_open()){
        cout << "Error opening edges.txt" << endl;
    }

    int numEdges;
    fin >> numEdges;
    int srcNode, destNode;
    for(int i = 0; i < numEdges; i++){
        fin >> id >> srcNode >> destNode;
        gv.addEdge(id, gv.getNode(srcNode), gv.getNode(destNode), Edge::DIRECTED);
    }

    fin.close();

    // Create window
    gv.createWindow(600, 600);

    // Join viewer thread (blocks till window closed)
    gv.join();
}

