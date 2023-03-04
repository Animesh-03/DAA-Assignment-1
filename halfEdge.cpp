#include "halfEdge.h"


HalfEdge::HalfEdge(Vertex* _start, Vertex* _end, int _face)
{
    // Assign the faces and vertices
    this->start = _start;
    this->end = _end;
    this->face = _face;

    start->halfEdges.push_back(this);
    end->incidentHalfEdges.push_back(this);
}

void HalfEdge::PrintStart()
{
    // Print the start vertex coords
    start->Print();
    std::cout << "-->";
}

HalfEdge* HalfEdge::Next()
{
    // Get the next half edge with the same face
    return end->FindHalfEdgeWithFace(face);
}

HalfEdge* HalfEdge::Next(int _face)
{
    // Get the next half edge with the given face
    return end->FindHalfEdgeWithFace(_face);
}