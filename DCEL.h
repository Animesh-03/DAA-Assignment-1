#ifndef DCEL_H
#define DCEL_H

#include<iostream>
#include<vector>
#include<set>

#include "vertex.h"
#include "halfEdge.h"

/// @brief A Doubly-Connected-Edge-List which is used to traverse the polygon in O(n) time complexity
class DCEL {
    public:

        /// @brief Stores all vertices of the polygon
        std::vector<Vertex*> vertices;

        /// @brief Stores all edges of the polygon
        std::vector<HalfEdge*> edges;

        /// @brief Stores only the diagonals of the polygon
        std::vector<HalfEdge*> diagonals;

        /// @brief Stores the total number of faces in the polygon, which also corresponds to the number of convex polygons after decompostion
        std::set<int> faces;

        /// @brief DCEL Constructor takes in all the vertices of the polygon in clockwise order
        /// @param _vertices Array contains vertices in clockwise order
        DCEL(std::vector<Vertex*> _vertices);
        DCEL();

        /// @brief Adds a half edge and its twin on the vertices specified
        /// @param _start One of the vertices
        /// @param _end Another vertex
        /// @return Returns the half edge created between start -> end
        HalfEdge* AddFullEdge(Vertex* _start, Vertex* _end,int _face);
        void generateFace();
        int getAvailableFace();
        HalfEdge* getEdge(Vertex* _start, Vertex* _end);
        int lastFace = 0;
        /// @brief Searches for the first half edge that is a part of the face supplied
        /// @param face The face number whose edge is to be found
        /// @return The edge which is a part of the face
        HalfEdge* FindHalfEdgeWithFace(int face);

        /// @brief Splits a face between two vertices by adding a full edge
        /// @param start The start vertex
        /// @param end The end vertex
        /// @param face The face number that is to be split
        int SplitFace(Vertex* start, Vertex* end, int face);

        /// @brief Merges a face by deleting a full edge between start and end vertices
        /// @param start The start vertex
        /// @param end The end vertex
        /// @param face The face that is to be merged
        void MergeFace(Vertex* start, Vertex*end);

        void TryRemoveDiagonal(HalfEdge* diagonal);

        /// @brief Traverses the given face
        /// @param face The face that is to be traversed
        void Traverse(int face);

        void PrintDiagonals();
};

#endif