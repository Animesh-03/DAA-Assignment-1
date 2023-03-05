#include<iostream>
#include <limits.h>
#include <algorithm>
#include <fstream>

#include "DCEL.h"

using namespace std;

#define THRESH -0.00001

class Line
{
    public:
    double A,B,C;

    Line(Vertex* a,Vertex* b)
    {
        if(a->x == b->x)
        {
            B = 0;
            C = -a->x;
            A = 1;
        }
        else if(a->y == b->y)
        {
            B = 1;
            C = -a->y;
            A = 0;
        }
        else
        {
            B=1;
            double m = (b->y-a->y)/(b->x-a->x);
            A = -m;
            C = -A*a->x - B*a->y;
        }
    }

    void printLine()
    {
        cout<<"Line : ("<<A<<","<<B<<","<<C<<")\n";
    }

    double satisfy(Vertex* a)
    {
        return A*a->x+B*a->y+C;
    }

    double satisfyAB(Vertex* a,Vertex* b)
    {
        return satisfy(a)*satisfy(b);
    }

    bool differentSide(Vertex* a,Vertex* b)
    {
        return satisfy(a)*satisfy(b) < THRESH;
    }
};

/// @brief Class handles the decomposition of a polygon and minimising the edges by merging faces
class PolygonDecomp
{
    public:
    /// @brief DCEL of the polygon
    DCEL dcel;
    /// @brief Array stores all the notches in the polygon
    vector<Vertex*> notches;    
    /// @brief Set of all vertices in the polygon
    set<Vertex*> P;
    
    /// @brief Constructs the polygon that is to be decomposed
    /// @param v The vertices of the polygon to be decomposed
    PolygonDecomp(vector<Vertex*> v)
    {
        dcel=DCEL(v);
        for(auto &vertex:dcel.vertices)
        {
            P.insert(vertex);
        }
    }

    /// @brief Returns the centroid of the given polygon
    /// @param polygon The vertices of the polygon
    /// @return The centroid of the polygon
    static Vertex* GetCentroid(vector<Vertex*> &polygon)
    {
        double sumx = 0,sumy = 0;
        for(auto &a:polygon)
        {
            sumx += a->x;
            sumy += a->y;
        }
        sumx /= polygon.size();
        sumy /= polygon.size();
        return new Vertex(sumx,sumy);
    }

    /// @brief Determines if a vertex is inside the current polygon
    /// @param v The point that is to be determined
    /// @param polygon The current polygon's vertices
    /// @returns True is v is inside polygon else false
    static bool VertexInside(Vertex* v,vector<Vertex*> &polygon)
    {
        auto centroid = GetCentroid(polygon);
        int l = 0;
        int nVertices = polygon.size();
        while(l<nVertices)
        {
            int r = (l+1)%nVertices;
            Line L(polygon[l],polygon[r]);
            if(L.differentSide(v,centroid))
            {
                return false;
            }
            l++;
        }
        return true;
    }

    /// @brief Cross Product between 3 points
    /// @param A Point 1
    /// @param B Point 2
    /// @param C Point 3
    /// @return The cross product between A B and C
    double CrossProduct(Vertex* A,Vertex* B,Vertex* C)
    {
        double X1 = (A->x - B->x);
    
        double Y1 = (A->y - B->y);
    
        double X2 = (C->x - B->x);
    
        double Y2 = (C->y - B->y);
        return (X1 * Y2 - Y1 * X2);
    }

    /// @brief Determines if b is notch
    /// @param a Vertex 1
    /// @param b Vertex 2
    /// @param c Vertex 3
    /// @return True is b is a notch else false
    bool IsNotch(Vertex* a,Vertex* b,Vertex* c)
    {
        double d = CrossProduct(a,b,c);
        return d<0;
    }

    /// @brief Generates all the notches in the current polygon
    /// @param face The face of the polygon
    void GenerateNotches(int face)
    {
        vector<Vertex*> notches;
        for(auto &vertex:dcel.vertices)
        {
            if(IsNotch(vertex->prev(face),vertex,vertex->next(face)))
                notches.push_back(vertex);
            P.insert(vertex);
        }

        for(auto &vertex:notches)
        {
            cout<<"Notch : "<<vertex->x<< " "<<vertex->y<<endl;
        }
    }

    set<Vertex*> SetDifference(set<Vertex*> &a,set<Vertex*> &b)
    {
        set<Vertex*> c;
        for(auto &x:a)
        {
            if(b.find(x) == b.end())
                c.insert(x);
        }      
        return c;
    }

    /// @brief Determines if a vertex v is inside a rectangle
    /// @param v Point to be determined
    /// @return True if v is inside rectangle else false
    bool InsideRect(double xmin,double xmax,double ymin,double ymax,Vertex* v)
    {
        return v->x<=xmax && v->x>=xmin && v->y<=ymax && v->y>=ymin;
    }

    /// @brief Decomposes the polygon into multiple convex polygons
    /// @param p Decomposition process starts from this
    void Decompose(Vertex* p)
    {
        vector<Vertex*> L;
        L.push_back(p);

        int n = P.size();
        vector<Vertex*> L1;
        // Stores the current face/polygon
        int face = 0;

        while(n>=3)
        {
            auto v1 = L.back(); auto v2 = v1->next(face);
            L1 = {v1,v2};
            Vertex* vi = v2;
            Vertex* vi_nex = vi->next(face); //v i+1
            Vertex* vi_prev = vi->next(face);//v i-1

            // Determine if adding next vertex will create a notch
            while(!IsNotch(vi_prev,vi,vi_nex) && !IsNotch(vi,vi_nex,v1) && !IsNotch(vi_nex,v1,v2) && L1.size()<n)
            {
                L1.push_back(vi_nex);
                vi_prev = vi;
                vi = vi_nex;
                vi_nex = vi->next(face);
            }
            set<Vertex*> Ls1 = set<Vertex*>(L1.begin(),L1.end());
            if(L1.size()!=P.size())
            {
                set<Vertex*> notches; //LPVS
                set<Vertex*> pl = SetDifference(P,Ls1);
                //rectangle part
                double xmin,ymin,xmax,ymax;
                xmin = ymin = INT_MAX;
                xmax = ymax = INT_MIN;
                for(auto &vertex:L1)
                {   
                    xmin = min(xmin,vertex->x);
                    ymin = min(ymin,vertex->y);
                    xmax = max(xmax,vertex->x);
                    ymax = max(ymax,vertex->y);
                }
                for(auto &vertex:pl)
                {
                    if(InsideRect(xmin,xmax,ymin,ymax,vertex) && IsNotch(vertex->prev(face),vertex,vertex->next(face)))
                        notches.insert(vertex);
                }
                // Remove vertices that are notches from the current polygon
                for(Vertex* a: notches)
                {
                    while(L1.size()>2 && VertexInside(a,L1))
                        L1.pop_back();
                }
            }
            if(L1.size()>2)
            {
                set<Vertex*> Ls1 = set<Vertex*>(L1.begin(),L1.end());
                P = SetDifference(P,Ls1);
                auto a = L1.front(),b = L1.back();
                P.insert(a);
                P.insert(b);
                // println(L1);
                n = n - L1.size() +2;
                face = dcel.SplitFace(a,b,face);
            }
            L = L1;
        }
    }

    /// @brief Merges redundant polygons to minimise the edges
    void MergePolygons()
    {
        for(int i = 0; i < dcel.diagonals.size(); i += 2)
        {
            Vertex* start = dcel.diagonals[i]->start;
            Vertex* end = dcel.diagonals[i]->end;

            int startFace = dcel.diagonals[i]->face, endFace = dcel.diagonals[i]->twin->face;

            Vertex* startPrev = start->prev(startFace);
            Vertex* startNext = start->next(endFace);

            Vertex* endPrev = end->prev(endFace);
            Vertex* endNext = end->next(startFace);

            if(!IsNotch(startPrev, start , startNext) && !IsNotch(endPrev, end , endNext))
            {
                start->Print();
                end->Print();
                // cout << diago
                dcel.MergeFace(start, end);
            }
        }
    }

    void PrintPolygons(fstream &fp)
    {

        fp << dcel.faces.size() << endl;
        
        for(auto &face:dcel.faces)
        {
            cout<<face<<":";
            dcel.Traverse(face, fp);
            dcel.TraverseFace(face);
        }
        cout << endl;
    }
};


void println(vector<Vertex*> & v)
{
    for(auto &x:v)
        cout<<"("<<x->x<<","<<x->y<<") --> ";
    cout<<"("<<v[0]->x<<","<<v[0]->y<<")\n";

}

vector<pair<int,int>> v = {
    {50, 423},
    {106,392},
    {8, 492},
    {285, 500},
    {476, 506},
    {672, 476},
    {392, 421},
    {286, 366},
    {299, 314},
    {464, 355},
    {888, 448},
    {916, 388},
    {861, 373},
    {980, 210},
    {828, 245},
    {820, 296},
    {747, 331},
    {699, 336},
    {541, 158},
    {570, 294},
    {547, 323},
    {389, 141},
    {655, 147},
    {753, 269},
vector<Vertex*> interpret(vector<pair<int,int>> &v)
{
    vector<Vertex*> ret;
    for(auto &x:v)
    {
        ret.push_back(new Vertex(x.first,x.second));
    }
    return ret;
}
    {884, 173},
    {998, 148},
    {827, 83},
    {857, 7},
    {805, 8},
    {681, 72},
    {57, 78},
    {178, 128},
    {305, 84},
    {362, 260},
    {284, 244},
    {173, 268},
   {83, 172},
   {42, 228},
   {94, 291},
};

int main()
{

    int n;
    ifstream fp;
    fp.open("input.txt");
    fp >> n;
    vector<Vertex*> verts;

    fstream fp2("outputNoMerge.txt", fstream::trunc | fstream::out);
    fstream fp1("outputMerge.txt", fstream::trunc | fstream::out);

    for(int i=0; i<n; i++)
    {
        double x, y;
        fp >> x >> y;
        verts.push_back(new Vertex(x,y));
    }

    PolygonDecomp p(verts);
    p.GenerateNotches(0);

    p.Decompose(verts[0]);

    p.PrintPolygons(fp1);

    p.MergePolygons();

    p.PrintPolygons(fp2);
}

