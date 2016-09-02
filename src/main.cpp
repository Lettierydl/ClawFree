//
//  main.cpp
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
#include <math.h>
#include <ilcplex/ilocplex.h>
#include "Inspection.h"

using namespace std;

vector<vector<double> > gerarGrafoAleatorio(int n){
    vector<vector<double> > grafo(n, vector<double> (n , 0));
    for(int i = 0 ; i < n; i++){
        for(int j = i+1 ; j < n; j++){
            double random = ((double)rand() / (double)(RAND_MAX));
            grafo[i][j] = random;
            grafo[j][i] = random;
        }
    }
    return grafo;
}


int main(int argc, const char * argv[]) {
    int n = 5;
    int a = (pow(n,2) - n)/2;
    srand((unsigned) time(NULL));
    
    int sum = 0;
    for(int t = 0; t< 400; t++){
        
    
        vector<vector<double> > grafo = gerarGrafoAleatorio(n);
    
        for(int i = 0 ; i < n; i++){
            for(int j = 0 ; j < n; j++){
           //     cout << grafo[i][j] << " | ";
            }//cout << endl;
        }
        
        Inspection ins;
        vector<vector<int>> falhas = ins.inspect(grafo);
        if(falhas.size() > 0){
            cout << falhas.size() << endl;
        }
        sum+=falhas.size();
        for(int i = 0; i< falhas.size(); i++){
            for(int j = 0; j < falhas[i].size(); j++){
                cout << falhas[i][j] << " | ";
            }
            cout << endl;
        }
    }
    cout << sum << endl;
    
    return 0;
}


