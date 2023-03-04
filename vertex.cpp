#include<algorithm>
#include "vertex.h"

Vertex::Vertex(double _x, double _y)
{
    // Assign the coords
    this->x = _x;
    this->y = _y;
}

void Vertex::Print()
{
    // Print the coords
    std::cout << "(" << x << "," << y << ")" << " ";
}

HalfEdge* Vertex::FindHalfEdgeWithFace(int face)
{
    // Search throught the array to find the half edge with the given face
    for(auto edge : halfEdges)
    {
        if(edge->face == face)
            return edge;
    }

    return NULL;
}

void Vertex::DeleteEdge(HalfEdge* halfEdge)
{
    // Delete the given half edge
    std::remove(halfEdges.begin(), halfEdges.end(), halfEdge);
}