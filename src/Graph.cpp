#include "Graph.h"
#include <iostream>

using namespace std;

Graph::Graph() : edit(0), Nv(Graph::la)
{
}

void Graph::print()
{
   cout << "EDIT: " << this->edit << endl;
   for(int i=0; i< Graph::order; ++i)
   {
      cout << "N("<< i << "): ";
      for(int nv : Nv[i])
	 cout << nv << " ";
      cout << endl;
   }
}

int Graph::order;
int **Graph::w; 
vector<list<int>> Graph::la; 
