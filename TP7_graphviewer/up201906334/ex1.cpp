#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex1() {
    // Instantiate GraphViewer
    GraphViewer gv;
    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    // Create graph
    Node& node0 = gv.addNode(0, sf::Vector2f(200, 300));
    node0.setColor(GraphViewer::BLUE);

    Node& node1 = gv.addNode(1, sf::Vector2f(400, 300));
    node1.setColor(GraphViewer::BLUE);

    // for bidirectional edges
    Edge &edge0 = gv.addEdge(0, node0, node1, GraphViewer::Edge::UNDIRECTED);

    gv.removeNode(1);

    Node& node2 = gv.addNode(2, sf::Vector2f(500, 300));
    Edge& edge1 = gv.addEdge(1, node0, node2, GraphViewer::Edge::UNDIRECTED);
    node2.setLabel("I'm a crazy Vertex");
    edge1.setLabel("I'm a loco edge");
    node2.setColor(GraphViewer::GREEN);

    for (Edge* edge : gv.getEdges())
        edge->setColor(GraphViewer::YELLOW);
    gv.setBackground("../TP7_graphviewer/resources/background.png");

    // Create window
    gv.createWindow(600, 600);
    // Join viewer thread (blocks till window closed)
    gv.join();
}
