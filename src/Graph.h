#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>

using namespace std;

struct Graph{

   Graph();
   //Representacao de uma solucao
   int edit;
   vector<list<int>> Nv;

   void print();
   /* Dados da entrada */
   //Dimensao do grafo
	static int order;
	static int **w; 
	static vector<list<int>> la;
};

#endif
