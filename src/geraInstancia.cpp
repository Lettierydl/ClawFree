#include <iostream>
#include <vector>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define IN_PATH "entrada/"
#define OUT_PATH "saida/"

using namespace std;

int main(int argc, char** argv)
{
   if(argc !=4)
   {
      cout << "usage: <n> <T> <t>" << endl;
      cout << "n : número de vertices" << endl;
      cout << "T : intervalo de tempo geral" << endl;
      cout << "t : intervalo de tempo máximo de cada atividade" << endl;
      exit(0);
   }
	

    	int n = atoi(argv[1]);
	int Tmax = atoi(argv[2]);
	int t = atoi(argv[3]);


	if(Tmax <= t)
	{
	   cout << "Intervalo t, deve ser menor que o intervalo geral T." << endl;
	   exit(0);
	}

	srand(time(NULL));


	vector< pair<int, int> > task(n);
	int tIni, tEnd;
	for(int i=0; i < n; ++i)
	{
	   tIni = rand() % (Tmax-t);
	   tEnd = tIni + (rand() % t);
	   if(tIni == tEnd)
	      ++tEnd;
	   task[i].first = tIni;
	   task[i].second = tEnd;

	   cout << "N:" << i << " Ini:" << tIni << " Fim:" << tEnd << endl;
	}

	vector< vector<bool> > M = vector< vector<bool> >(n);
	for(int i=0; i < n; ++i)
		M[i] = vector<bool>(n, false);

	//Transformando os intervalos de tempo das atividades em grafo de intervalo (matriz de adjacencia)
	for(int i=0; i < n; ++i)
	{
	   for(int j=0; j < n; ++j)
	   {
	      if(j != i)
	      {
		 if((task[i].second >= task[j].first) 
		       && ( task[i].first <= task[j].second))
		 {
		    M[i][j] = true;
		    M[j][i] = true;
		 }
	      }
	   }
	}
	
	//impressao
	for(int i=0; i < n; ++i)
	{
	   for(int j=0; j < n; ++j)
	      if(M[i][j])
	      	cout << "1 ";
	      else
	      	cout << "0 ";
	   cout << endl;
	}

	FILE *pf;
	char *nn = new char[100];
	sprintf(nn, "%s%d_%d_%d%s", OUT_PATH, n, Tmax, t, ".txt");
	if((pf = fopen(nn,"a")) ==NULL){
	   printf("\nNao consigo abrir o arquivo %s!", nn);
	   exit(1);
	}

	fprintf(pf, "%d\n", n);
	for(int i=0; i < n; ++i)
	{
	   for(int j=i+1; j < n; ++j)
	   {
	      if(M[i][j])
		fprintf(pf, "1 ");
	      else
		fprintf(pf, "0 ");
	   }
	   fprintf(pf, "\n");
	}
}

