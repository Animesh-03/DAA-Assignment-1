#ifndef HALFEDGE_H
#define HALFEDGE_H

#include<iostream>
#include<vector>

#include "vertex.h"

class Vertex;

/// @brief The Half Edge between two vertices
class HalfEdge {
    public:
        /// @brief The vertex through which the half edge originates
        Vertex* start;

        /// @brief The vertex where the half edge ends
        Vertex* end;

        /// @brief The twin of the current half edge
        HalfEdge* twin;

        /// @brief The face number of which this half edge is a part of
        int face;

        /// @brief The constructor for the half edge which adds the edge to the vertices
        /// @param _start The start vertex
        /// @param _end The end vertex
        /// @param face The face number of which this half edge is a part of
        HalfEdge(Vertex* _start, Vertex* _end, int _face);
        
        /// @brief Prints the start vertex
        void PrintStart();

        /// @brief Gets the next half edge which is also part of the same face
        /// @returns Half Edge which is the next edge of the face
        HalfEdge* Next();

        /// @brief Gets the next half edge which is also part of the given face
        /// @returns Half Edge which is the next edge of the given face
        HalfEdge* Next(int _face);

};

#endif