#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex1() {
    // Instantiate GraphViewer
    GraphViewer gv;

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));


    // Create a blue vertex with ID 0 at (200, 300)
    Node &node0 = gv.addNode(0, sf::Vector2f(200, 300)); // Create node
    node0.setColor(GraphViewer::BLUE); // Change color

    // Create a blue vertex with ID 1 at (400, 300)
    Node &node1 = gv.addNode(1, sf::Vector2f(400, 300)); // Create node
    node1.setColor(GraphViewer::BLUE); // Change color

    // Create a black edge between the two previously created vertices
    Edge &edge1 = gv.addEdge(0, node0, node1, Edge::UNDIRECTED);

    // Remove vertex 1
    gv.removeNode(1);

    // Add a new vertex with ID 2 at (500, 300)
    Node &node2 = gv.addNode(2, sf::Vector2f(500, 300));
    node2.setColor(GraphViewer::RED);

    // Add a black edge between vertices 0 and 2
    Edge &edge2 = gv.addEdge(1, node0, node2, Edge::UNDIRECTED);
    edge2.setColor(GraphViewer::BLACK);

    // Add a label to vertex 2 with a text of your choosing
    node2.setLabel("Vertex 2");

    // Add a label to an edge with a text of your choosing
    edge2.setLabel("Edge 2");

    // Make vertex 2 green
    node2.setColor(GraphViewer::GREEN);

    // Make the edges yellow
    for(Edge *edge : gv.getEdges()){
        edge->setColor(GraphViewer::YELLOW);
    }

    // Make the “background.png” image the background
    gv.setBackground("./resources/background.png");

    // Create window
    gv.createWindow(600, 600);

    // Join viewer thread (blocks till window closed)
    gv.join();

}
