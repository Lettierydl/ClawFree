#include "FileGraph.h"
#include "Graph.h"
//#include "HC.h"
#include <ilcplex/ilocplex.h>
#include <algorithm>
#include <iostream>
#include <sys/time.h>

using namespace std;

double OptimizeClaw(int n, int **w);
double OptimizeClawVertex(int n, int **w);

int main_Modelo(int argc, char** argv){
	struct timeval tempoAntes;
    struct timeval tempoDepois;


	// Carregando dados de entrada
	if (argc != 3) {
		cout << "Parametros invalidos\n";
		cout << " ./claw [Instancia] [estrategia]" << endl;
		cout << " [estrategia] : 0 - MIP edge" << endl;
		cout << " [estrategia] : 1 - MIP vertex" << endl;
		//cout << " [estrategia] : 2 - HC edge" << endl;
		exit(1);
	}

   	// ----- Carregar Dados ----- //

   	// Abre arquivo

   	char *instancia;
    	instancia = argv[1];
	int alg = atoi(argv[2]);

	FileGraph::loadFile(instancia);



	srand(time(NULL));
	gettimeofday(&tempoAntes, NULL);


	int sol;
	if(alg == 0)
	   OptimizeClaw(Graph::order, Graph::w);
	if(alg == 1)
	   OptimizeClawVertex(Graph::order, Graph::w);
	/*if(alg == 2)
	{
	   HC hc;
	   hc.buildGraph();
	   cout << "Edit: " << hc.sol.edit << endl;
	}*/
	
	
	 gettimeofday(&tempoDepois, NULL);
	 
	 timeval dif;
         timersub(&tempoDepois, &tempoAntes, &dif);
         long tempo;
         if(dif.tv_usec != 0)
         	tempo = dif.tv_sec*1000 + dif.tv_usec/1000;
         else
                tempo = dif.tv_sec*1000;
	
	 //FileBicluster::printInstancia("exatos", instancia, sol, tempo, alg);
    return 0;
}

//Implementa o modelo inteiro do Claw-free com edicao de arestas
double OptimizeClaw(int n, int **w)
{
	IloEnv env;
	
	// Criando um modelo

	IloModel modelo(env);

	// Variáveis de decisão

	// Variável x
	IloArray <IloBoolVarArray > x(env, n);
	//IloArray <IloNumVarArray > x(env, P);

	for (int i = 0; i < n; ++i) {
		IloBoolVarArray vetor(env, n);
		//IloNumVarArray vetor(env, M, 0, 1);
		x[i] = vetor;
	}

	// Adicionando a variável x no modelo
	 char var[100];
    	for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
            	sprintf(var, "X(%d,%d)", i, j);
            	x[i][j].setName(var);
            	modelo.add(x[i][j]);
            }
    	}
    
    	IloExpr PosEdge(env);
    	IloExpr NegEdge(env);
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if(w[i][j] > 0)
				PosEdge += (1- x[i][j]);
			else
				NegEdge += x[i][j];
		}
	}
    

    	// Adicionando a FO
    	modelo.add(IloMinimize(env, PosEdge + NegEdge));
    	

	// Restricoes 1 - Proibi a formação de P4
	for (int i = 0; i < n; i++) 
	{
	   for (int j = i+1; j < n; j++) 
	   {
	      for (int k = j+1; k < n; k++) 
	      {
		 for (int l = k+1; l < n; l++)
		 {
		    modelo.add( x[i][j] + x[i][k] + x[i][l] <= x[j][k] + x[j][l] + x[k][l] + 2);
		    modelo.add( x[i][j] + x[j][k] + x[j][l] <= x[i][k] + x[i][l] + x[k][l] + 2);
		    modelo.add( x[i][k] + x[j][k] + x[k][l] <= x[i][j] + x[i][l] + x[j][l] + 2);
		    modelo.add( x[i][l] + x[j][l] + x[k][l] <= x[i][j] + x[i][k] + x[j][k] + 2);
		 }
	      }	
	   }	
	}

	IloCplex CLAW(modelo);
	//CLAW.exportModel("CLAW.lp");
	CLAW.solve();
   
	cout << "Nodes: " << CLAW.getNnodes() << endl;
	double cost = CLAW.getObjValue();
	cout << "OBJ: " << cost << endl;
	
	for (int i = 0; i < n; i++)
	{
	   for (int j = i+1; j < n; j++)
	   {
	      if((CLAW.getValue(x[i][j]) < 0.01) && (w[i][j] == 1))
	      	cout << "DELETE I: " << i << " J: " <<j << endl;
	      if((CLAW.getValue(x[i][j]) > 0.99) && (w[i][j] == 0))
	      	cout << "ADD I: " << i << " J: " <<j << endl;
	   }
	}

	return cost;
}

//Implementa o modelo inteiro do Claw-free com delecao de vertices
double OptimizeClawVertex(int n, int **w)
{
	IloEnv env;
	
	// Criando um modelo

	IloModel modelo(env);

	// Variáveis de decisão

	// Variável x
	IloBoolVarArray x(env, n);
	//IloArray <IloNumVarArray > x(env, P);

	/*for (int i = 0; i < n; ++i) {
		IloBoolVarArray vetor(env, n);
		//IloNumVarArray vetor(env, M, 0, 1);
		x[i] = vetor;
	}*/

	// Adicionando a variável x no modelo
	 char var[100];
    	for (int i = 0; i < n; i++) {
            //for (int j = i+1; j < n; j++) {
            	sprintf(var, "X(%d)", i);
            	x[i].setName(var);
            	modelo.add(x[i]);
            //}
    	}
    
    	IloExpr vertex(env);
	for (int i = 0; i < n; i++) {
		vertex += (1- x[i]);
	}
    

    	// Adicionando a FO
    	modelo.add(IloMinimize(env, vertex));
    	

	// Restricoes 1 -
	for (int i = 0; i < n; i++) 
	{
	   for (int j = i+1; j < n; j++) 
	   {
	      for (int k = j+1; k < n; k++) 
	      {
		 for (int l = k+1; l < n; l++)
		 {
		    modelo.add( w[i][j]*x[j] + w[i][k]*x[k] + w[i][l]*x[l] <= w[j][k] + w[j][l] + w[k][l] + 3 - x[i]);
		    modelo.add( w[i][j]*x[i] + w[j][k]*x[k] + w[j][l]*x[l] <= w[i][k] + w[i][l] + w[k][l] + 3 - x[j]);
		    modelo.add( w[i][k]*x[i] + w[j][k]*x[j] + w[k][l]*x[l] <= w[i][j] + w[i][l] + w[j][l] + 3 - x[k]);
		    modelo.add( w[i][l]*x[i] + w[j][l]*x[j] + w[k][l]*x[k] <= w[i][j] + w[i][k] + w[j][k] + 3 - x[l]);
		 }
	      }	
	   }	
	}

	IloCplex CLAW(modelo);
	//CLAW.exportModel("CLAW.lp");
	CLAW.solve();
   
	cout << "Nodes: " << CLAW.getNnodes() << endl;
	double cost = CLAW.getObjValue();
	cout << "OBJ: " << cost << endl;
	for (int i = 0; i < n; i++) 
	      cout << CLAW.getValue(x[i])  << " ";
	cout << endl;

	
	return cost;
}

