#include<algorithm>
#include "DCEL.h"

int DCEL::getAvailableFace()
{
    if(faces.empty())
        return 0;
    return 1+*(--faces.end());
}

DCEL::DCEL()
{

}

HalfEdge* DCEL::getEdge(Vertex* _start, Vertex* _end)
{
    for(auto &x:_start->halfEdges)
    {
        if(x->end == _end)
            return x;
    }
    return NULL;
}

void DCEL::generateFace()
{
    faces.insert(getAvailableFace());
}

DCEL::DCEL(std::vector<Vertex*> _vertices)
{
    // Assign vertices
    this->vertices = _vertices;

    // Add Full Edges
    generateFace();
    for(int i = 0; i < vertices.size() - 1; i++)
    {
        AddFullEdge(vertices[i], vertices[i+1],0);
    }
    AddFullEdge(vertices[vertices.size() - 1], vertices[0],0);

    generateFace();
}

HalfEdge* DCEL::AddFullEdge(Vertex* _start, Vertex* _end,int _face)
{
    // Create twin edges
    HalfEdge* newEdge = new HalfEdge(_start, _end, _face);
    HalfEdge* newTwinEdge = new HalfEdge(_end, _start, getAvailableFace());
    // Assign Twin edges
    newEdge->twin = newTwinEdge;
    newTwinEdge->twin = newEdge;
    // Add edges to vector
    edges.push_back(newEdge);
    edges.push_back(newTwinEdge);
    return newEdge;
}

HalfEdge* DCEL::FindHalfEdgeWithFace(int face)
{   
    // Search through the array for the edge with given face
    for(auto edge : edges)
    {
        if(edge->face == face)
            return edge;
    }

    return NULL;
}

int DCEL::SplitFace(Vertex* start, Vertex* end, int face)
{
    // Add a full edge between start and end vertices
    auto test = start->FindNextVertexEdge(end);
    if(test != NULL)
    {
        return test->face;
    }
    int av_face = getAvailableFace();
    generateFace();
    HalfEdge* startEdge = AddFullEdge(start, end,av_face);
    // Assign the twin edge side to be of the original face
    startEdge->twin->face = face;

    // Assign the faces of the other edges to a new face
    HalfEdge* current = startEdge->Next(face);
    do 
    {
        current->face = startEdge->face;
        current = current->Next(face);
    } while(current->end != startEdge->start);
    current->face = startEdge->face;

    diagonals.push_back(startEdge);
    diagonals.push_back(startEdge->twin);

    return startEdge->face;
    // Increase the number of faces
}

void DCEL::MergeFace(Vertex* start, Vertex*end)
{
    // Find the half edge with the given face and vertex
    HalfEdge* e = getEdge(start,end);
    HalfEdge* e1 = e->twin;
    std::cout<<e->face<<std::endl;
    int face = e->face;
    // Store the face of the twin edge
    int twinFace = e1->face;

    // Delete the edges between start and end vertices for the given face
    start->DeleteEdge(e);
    start->DeleteIncidentEdge(e1);
    end->DeleteEdge(e1);
    start->DeleteIncidentEdge(e);

    remove(diagonals.begin(), diagonals.end(), e);
    remove(diagonals.begin(), diagonals.end(), e1);

    // Reassign the faces after the merge
    HalfEdge* startEdge = start->FindHalfEdgeWithFace(twinFace);
    HalfEdge* current = startEdge;
    do
    {
        current->face = face;
        current = current->Next(twinFace);
    } while(current->start != end);

    // Reduce the number of faces by one
    faces.erase(twinFace);
}

void DCEL::Traverse(int face)
{

    HalfEdge* start = this->FindHalfEdgeWithFace(face);
    HalfEdge* current = start;

    int n = 1;

    do
    {
        n++;
        current = current->Next();
    } while(current != start);

    std::cout << n << std::endl;

    do
    {
        std::cout << current->start->x  << " " << current->start->y << std::endl;
        current = current->Next();
    } while(current != start);
    std::cout << current->start->x  << " " << current->start->y << std::endl;
}

void DCEL::TryRemoveDiagonal(HalfEdge* diagonal)
{
    Vertex* v = diagonal->start;
    Vertex* vNext = v->next(diagonal->twin->face);
    Vertex* vPrev = v->prev(diagonal->face);

    vPrev->Print();
    std::cout << "-->";
    v->Print();
    std::cout << "-->";
    vNext->Print();
}

void DCEL::PrintDiagonals()
{
    std::cout << "Diagonals:" << std::endl;
    for(auto edge: diagonals)
    {
        edge->PrintStart();
        edge->end->Print();
        std::cout << std::endl;
    }
}