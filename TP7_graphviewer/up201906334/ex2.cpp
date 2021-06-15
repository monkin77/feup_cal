#include <synchapi.h>
#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex2() {
    // Instantiate GraphViewer
    GraphViewer gv;
    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    Node& node0 = gv.addNode(0, sf::Vector2f(300, 50));
    Node& node1 = gv.addNode(1, sf::Vector2f(318, 58));
    Node& node2 = gv.addNode(2, sf::Vector2f(325, 75));
    Node& node3 = gv.addNode(3, sf::Vector2f(318, 93));
    Node& node4 = gv.addNode(4, sf::Vector2f(300, 100));
    Node& node5 = gv.addNode(5, sf::Vector2f(282, 93));
    Node& node6 = gv.addNode(6, sf::Vector2f(275, 75));
    Node& node7 = gv.addNode(7, sf::Vector2f(282, 58));
    Node& node8 = gv.addNode(8, sf::Vector2f(150, 200));
    Node& node9 = gv.addNode(9, sf::Vector2f(300, 200));
    Node& node10 = gv.addNode(10, sf::Vector2f(450, 200));
    Node& node11 = gv.addNode(11, sf::Vector2f(300, 400));
    Node& node12 = gv.addNode(12, sf::Vector2f(200, 550));
    Node& node13 = gv.addNode(13, sf::Vector2f(400, 550));

    gv.addEdge(0, node0, node1, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(1, node1, node2, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(2, node2, node3, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(3, node3, node4, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(4, node4, node5, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(5, node5, node6, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(6, node6, node7, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(7, node7, node0, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(8, node4, node9, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(9, node9, node8, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(10, node9, node10, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(11, node9, node11, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(12, node11, node12, GraphViewer::Edge::UNDIRECTED);
    gv.addEdge(13, node11, node13, GraphViewer::Edge::UNDIRECTED);

    // Create window
    gv.createWindow(600, 600);

    int x = 250, y = 550;
    while (true) {
        Sleep(1000);
        gv.lock();
        Node& node12 = gv.getNode(12);
        node12.setPosition(sf::Vector2f(x, y));
        if (x == 250) x = 350;
        else x = 250;
        gv.unlock();
    }
    // Join viewer thread (blocks till window closed)
    gv.join();
}

