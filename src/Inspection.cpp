//
//  Inspection.cpp
//  ClawFree
//
//  Created by Lettiery on 01/09/16.
//  Copyright © 2016 Lettiery. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <list>
#include "Math.h"

using namespace std;
#include "Inspection.h"
#define EPS 0.0001


vector<Inspection::equacaoViolada> Inspection::inspectParametris(vector<vector<double> > grafo, float violacao, int sizeVioladas){
    int n = grafo.size();
    vector<Inspection::equacaoViolada > falhas (0);
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            for (int k = j+1; k < n; k++){
                for (int l = k+1; l < n; l++){
                    
                    float a1 = grafo[i][j];
                    float a2 = grafo[i][k];
                    float a3 = grafo[i][l];
                    
                    float a4 = grafo[j][k];
                    float a5 = grafo[j][l];
                    
                    float a6 = grafo[k][l];
                    
                    if( (a1 + a2 + a3) - (a4 + a5 + a6) > violacao + EPS){// i no centro
                        vector<int> furo(4);
                        furo[0] = i; furo[1] = j; furo[2] = k;furo[3] = l;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a1 + a2 + a3) - (a4 + a5 + a6);
                        falhas.push_back(e);
                    }else if( (a1 + a4 + a5) - (a2 + a3 + a6) > violacao+ EPS){// j no centro
                        vector<int> furo(4);
                        furo[0] = j; furo[1] = i; furo[2] = k;furo[3] = l;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a1 + a4 + a5) - (a2 + a3 + a6);
                        falhas.push_back(e);
                    }else if( (a2 + a4 + a6) - (a1 + a3 + a5) > violacao+ EPS){// k no centro
                        vector<int> furo(4);
                        furo[0] = k; furo[1] = j; furo[2] = i;furo[3] = l;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a2 + a4 + a6) - (a1 + a3 + a5);
                        falhas.push_back(e);
                    }else if( (a3 + a5 + a6) - (a1 + a2 + a4) > violacao+ EPS){// l no centro
                        vector<int> furo(4);
                        furo[0] = l; furo[1] = j; furo[2] = k;furo[3] = i;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a3 + a5 + a6) - (a1 + a2 + a4);
                        falhas.push_back(e);
                    }
                   
                    if(falhas.size() >= sizeVioladas){
                        return falhas;
                    }
                }
            }
        }
        
    }
    
    //cout << " - "<<falhas.size()<< endl;
    return falhas;
}

vector<Inspection::equacaoViolada> Inspection::inspect(vector<vector<double> > grafo){
    int n = grafo.size();
    vector<Inspection::equacaoViolada > falhas (0);
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            for (int k = j+1; k < n; k++){
                for (int l = k+1; l < n; l++){
                    
                    float a1 = grafo[i][j];
                    float a2 = grafo[i][k];
                    float a3 = grafo[i][l];
                    
                    float a4 = grafo[j][k];
                    float a5 = grafo[j][l];
                    
                    float a6 = grafo[k][l];
                    
                    if( (a1 + a2 + a3) - (a4 + a5 + a6) > 2){// i no centro
                        vector<int> furo(4);
                        furo[0] = i; furo[1] = j; furo[2] = k;furo[3] = l;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a1 + a2 + a3) - (a4 + a5 + a6);
                        falhas.push_back(e);
                    }else if( (a1 + a4 + a5) - (a2 + a3 + a6) > 2){// j no centro
                        vector<int> furo(4);
                        furo[0] = j; furo[1] = i; furo[2] = k;furo[3] = l;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a1 + a4 + a5) - (a2 + a3 + a6);
                        falhas.push_back(e);
                    }else if( (a2 + a4 + a6) - (a1 + a3 + a5) > 2){// k no centro
                        vector<int> furo(4);
                        furo[0] = k; furo[1] = j; furo[2] = i;furo[3] = l;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a2 + a4 + a6) - (a1 + a3 + a5);
                        falhas.push_back(e);
                    }else if( (a3 + a5 + a6) - (a1 + a2 + a4) > 2){// l no centro
                        vector<int> furo(4);
                        furo[0] = l; furo[1] = j; furo[2] = k;furo[3] = i;
                        equacaoViolada e;
                        e.vertices = furo;
                        e.valorEquacao = (a3 + a5 + a6) - (a1 + a2 + a4);
                        falhas.push_back(e);
                    }
                    
                }
            }
        }
        
    }
    
    Inspection::equacaoVioladaCompare compare;
    sort(falhas.begin(), falhas.end(), compare);
    
    int corteFalhas = n*n;
    if(falhas.size() > corteFalhas){
        falhas.resize(corteFalhas);
    }
    //cout << " - "<<falhas.size()<< endl;
    return falhas;
}

vector<Inspection::equacaoViolada> Inspection::inspectNewLevel(vector<equacaoViolada> falhas, vector<vector<double> > grafo){
    vector<equacaoViolada> newLevelFalhas(0);
    for(int f = 0; f < falhas.size();f++){
        for(int i = 0; i < grafo.size(); i++){
            bool jaExiste = false;
            for(int j = 0; j < falhas[f].vertices.size(); j++){
                if(falhas[f].vertices[j] == i){
                    jaExiste = true;
                    break;continue;
                }
            }
            if(jaExiste){
                continue;
            }
            double newValor = calcularValorEquacao(falhas[f], i, grafo);
            if(newValor > falhas[f].valorEquacao){
                vector<int> newVertices(falhas[f].vertices.size());
                copy( falhas[f].vertices.begin(), falhas[f].vertices.end(), newVertices.begin() );
                newVertices.push_back(i);
                equacaoViolada e;
                e.valorEquacao =newValor;
                e.vertices = newVertices;
                newLevelFalhas.push_back(e);
                //cout << "add"<<endl;
            }
            
        }
    }
    int corteFalhas = grafo.size()*grafo.size();
    if(newLevelFalhas.size() > corteFalhas){
        cout << newLevelFalhas.size()<< endl;
        newLevelFalhas.resize(corteFalhas);
        cout << newLevelFalhas.size()<< endl;
    }
    return newLevelFalhas;
}


double Inspection::calcularValorEquacao(equacaoViolada e, int newVertice, vector<vector<double> > grafo){
    double calculo = e.valorEquacao;
   // printf("calculo: %.2f\n",calculo);
    //printf("grafo[%i][%i]: %.2f\n", min(e.vertices[0], newVertice),max(e.vertices[0], newVertice),
     //      grafo[min(e.vertices[0], newVertice)][max(e.vertices[0], newVertice)]);
    calculo+= grafo[min(e.vertices[0], newVertice)][max(e.vertices[0], newVertice)];
    //printf("calculo: %.2f\n\n",calculo);
    
    
    for(int i = 1; i < e.vertices.size(); i++){
        calculo -= grafo[min(e.vertices[i], newVertice)][max(e.vertices[i], newVertice)];
      //  printf("grafo[%i][%i]: %.2f, ", min(e.vertices[i], newVertice),max(e.vertices[i], newVertice),
       //        grafo[min(e.vertices[i], newVertice)][max(e.vertices[i], newVertice)]);
        //printf("calculo: %.2f\n",calculo);
    }
    
    return calculo;
}


double Inspection::calcularValorEquacao(int c, int j, int k, int l, vector<vector<double> > grafo){
    double arestasPositivas = 0;
    double arestasNegativas = 0;
    
    double calculo = 0;
    calculo += grafo[min(c, j)][max(c, j)];
    calculo += grafo[min(c, k)][max(c, k)];
    calculo += grafo[min(c, l)][max(c, l)];
    
    
    calculo -= grafo[min(j, k)][max(k, j)];
    calculo -= grafo[min(j, l)][max(j, l)];
    calculo -= grafo[min(k, l)][max(k, l)];
    //printf("c %i, j %i, k %i, l %i\n", c,j,k,l);
    //printf("a1 %.2f, a2 %.2f, a3 %.2f, a4 %.2f, a5 %.2f, a6 %.2f\n", a1,a2,a3,a4,a5,a6);
    
    //return (a1+a2+a3) - (a4+a5+a6);
    //return arestasPositivas - arestasNegativas;
    return calculo;
}



vector< list <int> > Inspection::vizinhos(vector<vector< double > >grafo){
    vector< list <int> > vizinhos(grafo.size(),list<int>());
    for (int i = 0; i < grafo.size(); i++) {
        for (int j = i+1; j < grafo.size(); j++){
            if(grafo[i][j] > 0){// ver se é vizinho 0,001 ta considerando vizinho
                vizinhos[i].push_back(j);
                vizinhos[j].push_back(i);
            }
        }
    }
    return vizinhos;
}

