#ifndef VERTEX_H
#define VERTEX_H

#include<iostream>
#include<vector>

#include "halfEdge.h"

class HalfEdge;
/// @brief The vertex class stores the co-ordinates of a point in the polygon
class Vertex {
    public:
        /// @brief X-Coordinate of the vertex
        double x;
        
        /// @brief Y-Coordinate of the vertex
        double y;

        /// @brief Stores all the half edges that originate from this vertex
        std::vector<HalfEdge*> halfEdges;
        std::vector<HalfEdge*> incidentHalfEdges;
        /// @brief The constructor takes in the co-ordinates of the vertex
        Vertex(double _x, double _y);

        /// @brief Prints the co-ordinates of the vertex
        void Print();

        /// @brief Finds the half edge which is part of the given face
        /// @param The face number whose edge is to be found
        /// @returns Half Edge which is part of the given face
        HalfEdge* FindHalfEdgeWithFace(int face);
        void DeleteIncidentEdge(HalfEdge* halfEdge);
        Vertex* next(int face);
        Vertex* prev(int face);
        HalfEdge* FindIncidentHalfEdgeWithFace(int face);
        HalfEdge* FindNextVertexEdge(Vertex* v);
        /// @brief Deletes the given half edge
        /// @param The half edge to be deleted
        void DeleteEdge(HalfEdge* halfEdge);
        
};


#endif