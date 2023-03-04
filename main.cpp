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
    DCEL lis;
    vector<Vertex*> notches;    
    set<Vertex*> P;

    PolygonDecomp(vector<Vertex*> v)
    {
        lis=DCEL(v);
        for(auto &x:lis.vertices)
        {
            P.insert(x);
        }
    }

    static Vertex* getCentroid(vector<Vertex*> &L)
    {
        double sumx = 0,sumy = 0;
        for(auto &a:L)
        {
            sumx += a->x;
            sumy += a->y;
        }
        sumx /= L.size();
        sumy /= L.size();
        return new Vertex(sumx,sumy);
    }

    static bool vertexInside(Vertex* p,vector<Vertex*> &poly)
    {
        auto c = getCentroid(poly);
        int l = 0;
        int n = poly.size();
        while(l<n)
        {
            int r = (l+1)%n;
            Line L(poly[l],poly[r]);
            if(L.differentSide(p,c))
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

    bool isNotch(Vertex* a,Vertex* b,Vertex* c)
    {
        double d = CrossProduct(a,b,c);
        return d<0;
    }

    void generateNotches(int face)
    {
        vector<Vertex*> notches;
        for(auto &x:lis.vertices)
        {
            if(isNotch(x->prev(face),x,x->next(face)))
                notches.push_back(x);
            P.insert(x);
        }

        for(auto &x:notches)
        {
            cout<<"Notch : "<<x->x<< " "<<x->y<<endl;
        }
    }

    set<Vertex*> diff(set<Vertex*> &a,set<Vertex*> &b)
    {
        set<Vertex*> c;
        for(auto &x:a)
        {
            if(b.find(x) == b.end())
                c.insert(x);
        }      
        return c;
    }

    bool insideRect(double xmin,double xmax,double ymin,double ymax,Vertex* p)
    {
        return p->x<=xmax && p->x>=xmin && p->y<=ymax && p->y>=ymin;
    }

    void decompose(Vertex* p)
    {
        vector<Vertex*> L;
        L.push_back(p);
        int n = P.size();
        vector<Vertex*> L1; 
        int face = 0;
        while(n>=3)
        {
            auto v1 = L.back(); auto v2 = v1->next(face);
            L1 = {v1,v2};
            Vertex* vi = v2;
            Vertex* vi_nex = vi->next(face); //v i+1
            Vertex* vi_prev = vi->next(face);//v i-1

            while(!isNotch(vi_prev,vi,vi_nex) && !isNotch(vi,vi_nex,v1) && !isNotch(vi_nex,v1,v2) && L1.size()<n)
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
                set<Vertex*> pl = diff(P,Ls1);
                //rectangle part
                double xmin,ymin,xmax,ymax;
                xmin = ymin = INT_MAX;
                xmax = ymax = INT_MIN;
                for(auto &x:L1)
                {   
                    xmin = min(xmin,x->x);
                    ymin = min(ymin,x->y);
                    xmax = max(xmax,x->x);
                    ymax = max(ymax,x->y);
                }
                for(auto &x:pl)
                {
                    if(insideRect(xmin,xmax,ymin,ymax,x) && isNotch(x->prev(face),x,x->next(face)))
                        notches.insert(x);
                }
                //L.back()
                for(Vertex* a: notches)
                {
                    while(L1.size()>2 && vertexInside(a,L1))
                        L1.pop_back();
                }
            }
            if(L1.size()>2)
            {
                set<Vertex*> Ls1 = set<Vertex*>(L1.begin(),L1.end());
                P = diff(P,Ls1);
                auto a = L1.front(),b = L1.back();
                P.insert(a);
                P.insert(b);
                n = n - L1.size() +2;
                face = lis.SplitFace(a,b,face);
            }
            L = L1;
        }
    }

    void printPoly()
    {
        for(auto &x:lis.faces)
        {
            cout<<x<<":";
            lis.Traverse(x);
        }
    }
};

//merge part

void println(vector<Vertex*> & v)
{
    for(auto &x:v)
        cout<<"("<<x->x<<","<<x->y<<") --> ";
    cout<<"("<<v[0]->x<<","<<v[0]->y<<")\n";

}

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

    PolygonDecomp p(verts);
    p.decompose(verts[0]);
    p.printPoly();

}

