#ifndef __CALLBACKS_HPP__
#define __CALLBACKS_HPP__

#include "Inspection.h"
#include <ilcplex/ilocplex.h>
#include <math.h>

int nivel =0;
ILOLAZYCONSTRAINTCALLBACK4(PrgDin, IloArray <IloBoolVarArray >&, x,const int&,  tam_v,
                           vector<vector <double> >, mat, int&, numCuts){
    
    for(int i=0; i<tam_v; i++) {
        for(int j=i+1; j<tam_v; j++) {
            mat[i][j] =  ((double)(IloNum(getValue(x[i][j]))));
        }
    }
    
    
    vector<Inspection::equacaoViolada> falhas = Inspection::inspect(mat);
    vector<Inspection::equacaoViolada> newFalhas = Inspection::inspectNewLevel(falhas, mat);
    for(int i = 0 ; i < newFalhas.size(); i++){
         falhas.push_back(newFalhas[i]);
    }
    newFalhas = Inspection::inspectNewLevel(newFalhas, mat);
        for(int i = 0 ; i < newFalhas.size(); i++){
            falhas.push_back(newFalhas[i]);
        }
    
    
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
    return;
}

ILOUSERCUTCALLBACK4(PrgDin2, IloArray <IloBoolVarArray >&, x,const int&, tam_v,
                    vector<vector <double> >&, mat, int&, numCuts){
    
    for(int i=0; i<tam_v; i++) {
        for(int j=i+1; j<tam_v; j++) {
            mat[i][j] =  ((double)(IloNum(getValue(x[i][j]))));
        }
    }
    
    vector<Inspection::equacaoViolada> falhas = Inspection::inspect(mat);
    
    for(int f = 0; f< falhas.size(); f++){
        int c = falhas[f].vertices[0];
        int i = falhas[f].vertices[1];
        int j = falhas[f].vertices[2];
        int k = falhas[f].vertices[3];
        IloExpr sumX(getEnv());
        for(int g = 1; g <= 3;g++ ){
            sumX += x[min(c,falhas[f].vertices[g])][max(c,falhas[f].vertices[g])];
        }
        
        for(int g = 1; g <= 3;g++ ){
            for(int h = g+1; h <= 3;h++ ){
                sumX -= x[min(falhas[f].vertices[g],falhas[f].vertices[h])]
                [max(falhas[f].vertices[g],falhas[f].vertices[h])];
            }
        }
        //cout << sumX<< endl<< endl;
        add(sumX <= 2);
    }
    
    
    
    return;
}

#endif //__CALLBACKS_HPP__

