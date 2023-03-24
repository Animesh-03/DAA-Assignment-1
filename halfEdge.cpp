#include "halfEdge.h"

/*
    Team Members : Vibhum Raj Tripathi - 2020A7PS0247H
                   Aditha Venkata Animesh - 2020A7PS0193H
                   Manan Mayur Popat - 2020A7PS0029H
                   Divyan Goyal - 2020A7PS0042H
*/

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