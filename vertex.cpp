#include<algorithm>
#include "vertex.h"

Vertex::Vertex(double _x, double _y)
{
    // Assign the coords
    this->x = _x;
    this->y = _y;
}

Vertex* Vertex::next(int face)
{
    return FindHalfEdgeWithFace(face)->end;
}

Vertex* Vertex::prev(int face)
{
    return FindIncidentHalfEdgeWithFace(face)->start;
}

HalfEdge* Vertex::FindNextVertexEdge(Vertex* e)
{
    for(auto &x:halfEdges)
    {
        if(x->end->x == e->x && x->end->y == e->y)
            return x;
    }
    return NULL;
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

HalfEdge* Vertex::FindIncidentHalfEdgeWithFace(int face)
{
    // Search throught the array to find the half edge with the given face
    for(auto edge : incidentHalfEdges)
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

void Vertex::DeleteIncidentEdge(HalfEdge* halfEdge)
{
    // Delete the given half edge
    std::remove(incidentHalfEdges.begin(), incidentHalfEdges.end(), halfEdge);
}