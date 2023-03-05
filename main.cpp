#include<iostream>
#include <limits.h>
#include <algorithm>
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

class PolygonDecomp
{
    public:
    /// @brief DCEL of the polygon
    DCEL dcel;
    /// @brief Array stores all the notches in the polygon
    vector<Vertex*> notches;    
    /// @brief Set of all vertices in the polygon
    set<Vertex*> P;
    
    PolygonDecomp(vector<Vertex*> v)
    {
        dcel=DCEL(v);
        for(auto &vertex:dcel.vertices)
        {
            P.insert(vertex);
        }
    }

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

    double CrossProduct(Vertex* A,Vertex* B,Vertex* C)
    {
        double X1 = (A->x - B->x);
    
        double Y1 = (A->y - B->y);
    
        double X2 = (C->x - B->x);
    
        double Y2 = (C->y - B->y);
        return (X1 * Y2 - Y1 * X2);
    }

    bool IsNotch(Vertex* a,Vertex* b,Vertex* c)
    {
        double d = CrossProduct(a,b,c);
        return d<0;
    }

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

    bool InsideRect(double xmin,double xmax,double ymin,double ymax,Vertex* v)
    {
        return v->x<=xmax && v->x>=xmin && v->y<=ymax && v->y>=ymin;
    }

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

    void MergePolygons()
    {

    }

    void PrintPolygons()
    {
        cout << dcel.faces.size() << endl;
        for(auto &face:dcel.faces)
        {
            // cout<<face<<":";
            dcel.Traverse(face);
        }
    }
};


void println(vector<Vertex*> & v)
{
    for(auto &x:v)
        cout<<"("<<x->x<<","<<x->y<<") --> ";
    cout<<"("<<v[0]->x<<","<<v[0]->y<<")\n";

}

int main()
{
    vector<Vertex*> verts = {
        new Vertex(50, 423),
        new Vertex(106, 392),
        new Vertex(8, 492),
        new Vertex(285, 500),
        new Vertex(476, 506),
        new Vertex(672, 476),
        new Vertex(392, 421),
        new Vertex(286, 366),
        new Vertex(299, 314),
        new Vertex(464, 355),
        new Vertex(888, 448),
        new Vertex(916, 388),
        new Vertex(861, 373),
        new Vertex(980, 210),
        new Vertex(828, 245),
        new Vertex(820, 296),
        new Vertex(747, 331),
        new Vertex(699, 336),
        new Vertex(541, 158),
        new Vertex(570, 294),
        new Vertex(547, 323),
        new Vertex(389, 141),
        new Vertex(655, 147),
        new Vertex(753, 269),
        new Vertex(884, 173),
        new Vertex(998, 148),
        new Vertex(827, 83),
        new Vertex(857, 7),
        new Vertex(805, 8),
        new Vertex(681, 72),
        new Vertex(57, 78),
        new Vertex(178, 128),
        new Vertex(305, 84),
        new Vertex(362, 260),
        new Vertex(284, 244),
        new Vertex(173, 268),
        new Vertex(83, 172),
        new Vertex(42, 228),
        new Vertex(94, 291)


        // new Vertex(8, 0),
        // new Vertex(7, 10),
        // new Vertex(6, 0),
        // new Vertex(5,10),
        // new Vertex(4, 0),
        // new Vertex(3, 10),
        // // new Vertex(2, 0),
        // new Vertex(2,0),
        // new Vertex(1, 10),
        // new Vertex(0, 0),
        // new Vertex(4, -2)
    };

    // reverse(verts.begin(), verts.end());

    PolygonDecomp p(verts);
    p.Decompose(verts[0]);
    p.PrintPolygons();
    // p.GenerateNotches(0);
    // p.dcel.PrintDiagonals();

    // p.dcel.TryRemoveDiagonal()
}

