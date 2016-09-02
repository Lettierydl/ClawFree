#include <iostream>
#include <fstream>
#include "FileGraph.h"
#include "Graph.h"


using namespace std;

void FileGraph::loadFile(char *fileName){
   ifstream Instancia(fileName, ios::in);
   int rSize;

   if (!Instancia) {
      cerr << "Arquivo nao pode ser aberto" << endl;
      //return NULL;
   }
   //string header;
   //std::getline(Instancia, header);

   int n;
   Instancia >> n; // numero de vertices


   //Alocando estruturas
   Graph::order = n;
   Graph::la.resize(n);
   Graph::w = new int*[n];
   for (int i = 0; i < n; i++) 
      Graph::w[i] = new int[n];

   //Lendo dados de entrada
   for (int i = 0; i < n; i++) 
   {
      for (int j = i+1; j < n; j++) 
      {
	 int data;
	 Instancia >> data;
	 if(data == 1){
	    Graph::w[i][j] = 1;
	    Graph::w[j][i] = 1;
	    Graph::la[i].push_back(j);
	    Graph::la[j].push_back(i);
	 }else
	 {
	    Graph::w[i][j] = 0;
	    Graph::w[j][i] = 0;
	 }
      }//printf("\n");
   }
   Instancia.close();
}

