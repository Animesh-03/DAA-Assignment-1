#include<iostream>
#include "DCEL.h"

using namespace std;

int main()
{
    vector<Vertex*> verts = {
        new Vertex(-2,0),
        new Vertex(-2,2),
        new Vertex(0,5),
        new Vertex(3,5),
        new Vertex(7,3),
        new Vertex(2,2),
        new Vertex(5,-2),
        new Vertex(2,-3),
        new Vertex(-1,-3),
        new Vertex(0,-1)
    };

    DCEL* dcel = new DCEL(verts);

    dcel->Traverse(0);
    dcel->Traverse(1);

    // dcel->SplitFace(verts[7], verts[3], 0);
    dcel->SplitFace(verts[0], verts[3], 0);
    dcel->Traverse(0);
    dcel->Traverse(1);
    dcel->Traverse(2);
    // dcel->Traverse(3);
    dcel->MergeFace(verts[0], verts[3]);
    dcel->Traverse(2);
}

