//
//  Inspection.h
//  ClawFree
//
//  Created by Lettiery on 01/09/16.
//  Copyright © 2016 Lettiery. All rights reserved.
//

#ifndef Inspection_h
#define Inspection_h

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <list>

using namespace std;

class Inspection{
    
public:
    struct equacaoViolada{
        vector<int> vertices;//vertices[0]centro, vertices[1..3] positivos, vertices[4..n] negativos
        double valorEquacao;//soma das arestas
    };
    
    static vector<equacaoViolada> inspect(vector<vector<double> > grafo);
    static vector<equacaoViolada> inspectParametris(vector<vector<double> > grafo, float violacao, int sizeVioladas);//violacao é o valor da equacao, sizeVioladas criterio de parada das equacoes violadas
    static vector<equacaoViolada> inspectNewLevel(vector<equacaoViolada> falhar, vector<vector<double> > grafo);
    static vector<vector<int> > combinacoesVertices(int centro, int ordem, int n);
    static vector< list <int> > vizinhos(vector<vector< double > >grafo);
    
    static double calcularValorEquacao(equacaoViolada e, int newVertice, vector<vector<double> > grafo);
    static double calcularValorEquacao(int i, int k, int l, int j, vector<vector<double> > grafo);
    
   
    
    struct equacaoVioladaCompare { //comparacao
        bool operator()(equacaoViolada const&e1, equacaoViolada const&e2) const {
            //return e1.valorEquacao < e2.valorEquacao;
            return e2.valorEquacao < e1.valorEquacao;
        }
    };
    
};



#endif /* Inspection_h */
