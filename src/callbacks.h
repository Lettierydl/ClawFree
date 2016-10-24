#ifndef __CALLBACKS_HPP__
#define __CALLBACKS_HPP__

#include "Inspection.h"
#include <ilcplex/ilocplex.h>
#include <math.h>
#include <sys/time.h>

float violacao = 2.00;
vector<Inspection::equacaoViolada> callcack(vector<vector <double> > mat, int n){
    struct timeval tempoAntes;
    struct timeval tempoDepois;
    
    srand(time(NULL));
    gettimeofday(&tempoAntes, NULL);
    
    //int n = mat.size();
    //int n = mat.size()*mat.size();
    //int n = INT_MAX;
    vector<Inspection::equacaoViolada> falhas = Inspection::inspectParametris(mat, violacao, n);
    
    if(falhas.size() < n && n!= INT_MAX){
        violacao -= 0.5;
        if(violacao< 2){
            violacao = 2;
        }
        vector<Inspection::equacaoViolada> newfalhas = Inspection::inspectParametris(mat, violacao, mat.size());
        for(int i = 0 ; i < newfalhas.size(); i++){
            falhas.push_back(newfalhas[i]);
        }
    }
    
    gettimeofday(&tempoDepois, NULL);
    
    timeval dif;
    timersub(&tempoDepois, &tempoAntes, &dif);
    long tempo;
    if(dif.tv_usec != 0){
        tempo = dif.tv_sec*1000 + dif.tv_usec/1000;
    }else{
        tempo = dif.tv_sec*1000;
    }
    printf("\tTime Inspect: %d\n\tViolacao: %.2f\n\tFalhas: %i\n", tempo, violacao, falhas.size());
    return falhas;
}






ILOLAZYCONSTRAINTCALLBACK4(PrgDin, IloArray <IloBoolVarArray >&, x,const int&,  tam_v,
                           vector<vector <double> >, mat, int&, numCuts){
   
    for(int i=0; i<tam_v; i++) {
        for(int j=i+1; j<tam_v; j++) {
            mat[i][j] =  ((double)(IloNum(getValue(x[i][j]))));
        }
    }
    printf("Begin Inspect------- INT\n");
    int n = INT_MAX;

    vector<Inspection::equacaoViolada> falhas = callcack(mat, n);
    
    for(int f = 0; f< falhas.size(); f++){
        
        IloExpr sumX(getEnv());
        
        int c = falhas[f].vertices[0];
        for(int i = 1; i < falhas[f].vertices.size(); i++){
            sumX += x[min(c,falhas[f].vertices[i])][max(c,falhas[f].vertices[i])];
        }
        
        for(int i = 1; i < falhas[f].vertices.size(); i++){
            for(int j = i+1; j < falhas[f].vertices.size(); j++){
                sumX -= x[min(falhas[f].vertices[i],falhas[f].vertices[j])]
                [max(falhas[f].vertices[i],falhas[f].vertices[j])];
            }
        }
        
        add(sumX <= 2);
    }
    
    printf("Finishi Inspect------- INT\n\n\n");
    
    return;
}

ILOUSERCUTCALLBACK4(PrgDin2, IloArray <IloBoolVarArray >&, x,const int&, tam_v,
                    vector<vector <double> >&, mat, int&, numCuts){
    
    for(int i=0; i<tam_v; i++) {
        for(int j=i+1; j<tam_v; j++) {
            mat[i][j] =  ((double)(IloNum(getValue(x[i][j]))));
        }
    }
    printf("Begin Inspect-------\n");
    int n = INT_MAX;
    vector<Inspection::equacaoViolada> falhas = callcack(mat,n);
    
    
    for(int f = 0; f< falhas.size(); f++){
        
        IloExpr sumX(getEnv());
        
        int c = falhas[f].vertices[0];
        for(int i = 1; i < falhas[f].vertices.size(); i++){
            sumX += x[min(c,falhas[f].vertices[i])][max(c,falhas[f].vertices[i])];
        }
        
        for(int i = 1; i < falhas[f].vertices.size(); i++){
            for(int j = i+1; j < falhas[f].vertices.size(); j++){
                sumX -= x[min(falhas[f].vertices[i],falhas[f].vertices[j])]
                [max(falhas[f].vertices[i],falhas[f].vertices[j])];
            }
        }
        
        add(sumX <= 2);
    }
    
    printf("Finishi Inspect-------\n\n\n");
    
    return;
}

#endif //__CALLBACKS_HPP__

