#include "FileGraph.h"
#include "Graph.h"
#include "Grafico.h"
#include "callbacks.h"
#include <ilcplex/ilocplex.h>
#include <algorithm>
#include <iostream>
#include <sys/time.h>

using namespace std;

double OptimizeClaw(int n, int **w, bool separacaoInequacoes);
double OptimizeClawVertex(int n, int **w);
void geraInstancia();


int main(int argc, char** argv){
    struct timeval tempoAntes;
    struct timeval tempoDepois;
    
    //geraInstancia();
    //return 0;
    
    /*lendo parametros*/
    string instancia;
    int problem;
    bool separacaoInequacoes;
    
    if (argc == 1) {
        //instancia = "instances/1000_5000_2500.txt";
        //instancia = "instances/500_2000_1500.txt";
        //instancia = "instances/400_2000_1000.txt";
        //instancia = "instances/300_1000_600.txt";
        instancia = "instances/200_1000_500.txt";
        //instancia = "instances/150_1000_500.txt";
        //instancia = "instances/100_1000_500.txt";
        //instancia = "instances/50_500_125.txt";
        //instancia = "instances/5_50_22.txt";
        problem = 0;
        separacaoInequacoes = true;
    }else if(argc > 3){
        instancia.append(argv[1]);
        problem = atoi(argv[2]);
        if(atoi(argv[3]) == 1){
            separacaoInequacoes = true;
        }else{
            separacaoInequacoes = false;
        }
    }else{
        printf("Parameters");
    }
    FileGraph::loadFile(instancia);
    cout << "Instance: "<<instancia << endl <<
    "Problem: "<< problem << " (0 - Edges, 1 - Vertex)"<< endl <<
    "Branch and Cut: "<< separacaoInequacoes << " (1 - True, 0 - False)"<< endl<< endl;
    
    srand(time(NULL));
    gettimeofday(&tempoAntes, NULL);
    
    
    //Grafico::plotarGrafoEntrada();
    //return 0;
    
    int sol;
    if(problem == 0){
        OptimizeClaw(Graph::order, Graph::w, separacaoInequacoes);
    }
    if(problem == 1){
        OptimizeClawVertex(Graph::order, Graph::w);
    }
    /*if(problem == 2)
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
double OptimizeClaw(int n, int **w, bool separacaoInequacoes){
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
    
    
    if(!separacaoInequacoes){
        cout << "Cagando..."<< endl;
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
    }else if (false){
        vector<int> eViolada (n, n);
        cout << "Iniciando..."<< endl;
        for (int i = 0; i < n; i++){
            if(eViolada[i] == 0){
                continue;
            }
            for (int j = i+1; j < n; j++){
            
                if(eViolada[j] == 0){
                    continue;
                }
                
                for (int k = j+1; k < n; k++){
                    if(eViolada[k] == 0){
                        continue;
                    }
                    
                    
                    for (int l = k+1; l < n; l++){
                        if(eViolada[l] == 0){
                            continue;
                        }
                        
                        
                        if((w[i][j] + w[i][k] + w[i][l]) - (w[j][k] + w[j][l] + w[k][l]) > 2){//i
                            eViolada[i]--;
                            modelo.add( x[i][j] + x[i][k] + x[i][l] <= x[j][k] + x[j][l] + x[k][l] + 2);
                        }else if( (w[i][j] + w[j][k] + w[j][l]) - (w[i][k] + w[i][l] + w[k][l]) > 2){//j
                            eViolada[j]--;
                            modelo.add( x[i][j] + x[j][k] + x[j][l] <= x[i][k] + x[i][l] + x[k][l] + 2);
                        }else if( (w[i][k] + w[j][k] + w[k][l]) - (w[i][j] + w[i][l] + w[j][l]) > 2){//k
                            eViolada[k]--;
                            modelo.add( x[i][k] + x[j][k] + x[k][l] <= x[i][j] + x[i][l] + x[j][l] + 2);
                        }else if( (w[i][l] + w[j][l] + w[k][l]) - (w[i][j] + w[i][k] + w[j][k]) > 2){//l
                            eViolada[l]--;
                            modelo.add( x[i][l] + x[j][l] + x[k][l] <= x[i][j] + x[i][k] + x[j][k] + 2);
                        }
                        
                    }
                }
            }
        }
        cout << "Finalizado..."<< endl;
        
    }
    // Restricoes 1 - Proibi a formação de P4
    /*for (int i = 0; i < n; i++)
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
     }*/
    
    IloCplex CLAW(modelo);
    
    
    //Ativando B&C
    int userCuts = 0;
    vector< vector<double> > mat(n);
    for(int i=0; i < n; ++i){
        mat[i] = vector<double>(n);
    }
    if(separacaoInequacoes){
        CLAW.use(PrgDin(env, x, n, mat, userCuts));
        CLAW.use(PrgDin2(env, x, n, mat, userCuts));
    }
    CLAW.exportModel("CLAW.lp");
    
    // CLAW.setOut(env.getNullStream());
    
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


void geraInstancia(){
    string IN_PATH ="entrada/";
    string OUT_PATH ="instances/";
    
    //cout << "usage: <n> <T> <t>" << endl;
    //cout << "n : número de vertices" << endl;
    //cout << "T : intervalo de tempo geral" << endl;
    //cout << "t : intervalo de tempo máximo de cada atividade" << endl;
    //exit(0);
    
    int n = 150;
    int t = 500;
    int Tmax = 1000;
    
    if(Tmax <= t){
        cout << "Intervalo t, deve ser menor que o intervalo geral T." << endl;
        exit(0);
    }
    
    srand(time(NULL));
    
    
    vector< pair<int, int> > task(n);
    int tIni, tEnd;
    for(int i=0; i < n; ++i){
        tIni = rand() % (Tmax-t);
        tEnd = tIni + (rand() % t);
        if(tIni == tEnd){
            ++tEnd;
        }
        task[i].first = tIni;
        task[i].second = tEnd;
        cout << "N:" << i << " Ini:" << tIni << " Fim:" << tEnd << endl;
    }
    
    vector< vector<bool> > M = vector< vector<bool> >(n);
    for(int i=0; i < n; ++i){
        M[i] = vector<bool>(n, false);
    }
    
    //Transformando os intervalos de tempo das atividades em grafo de intervalo (matriz de adjacencia)
    for(int i=0; i < n; ++i){
        for(int j=0; j < n; ++j){
            if(j != i){
                if((task[i].second >= task[j].first)
                   && ( task[i].first <= task[j].second)){
                    M[i][j] = true;
                    M[j][i] = true;
                }
            }
        }
    }
    
    //impressao
    for(int i=0; i < n; ++i){
        for(int j=0; j < n; ++j)
            if(M[i][j]){
                cout << "1 ";
            }else{
                cout << "0 ";
            }
        cout << endl;
    }
    
    FILE *pf;
    char *nn = new char[100];
    sprintf(nn, "%s%d_%d_%d%s", OUT_PATH.c_str(), n, Tmax, t, ".txt");
    if((pf = fopen(nn,"a")) ==NULL){
        printf("\nNao consigo abrir o arquivo %s!", nn);
        exit(1);
    }
    
    fprintf(pf, "%d\n", n);
    for(int i=0; i < n; ++i){
        for(int j=i+1; j < n; ++j){
            if(M[i][j]){
                fprintf(pf, "1 ");
            }else{
                fprintf(pf, "0 ");
            }
        }
        fprintf(pf, "\n");
    }
}

