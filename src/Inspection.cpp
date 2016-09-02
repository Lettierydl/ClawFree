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

using namespace std;
#include "Inspection.h"

int indiceVertice(int a1, int a2){
    int am = min(a1, a2);
    return (a1+a2)+am;
}

vector<vector<int> > Inspection::inspect(vector<vector<double> > grafo){
    int n = 5;// calcular numero de vertices apartir do numero de arestas
    int a = grafo.size();
    
    vector<vector<int>> falhas (0,vector<int>());
    
    for (int i = 0; i < a; i++){
        for (int j = i+1; j < a; j++){
            for (int k = j+1; k < a; k++){
                for (int l = k+1; l < a; l++){
                    
                    float v1 = grafo[i][j];
                    float v2 = grafo[i][k];
                    float v3 = grafo[i][l];
                    
                    float v4 = grafo[j][k];
                    float v5 = grafo[j][l];
                    
                    float v6 = grafo[k][l];
                    
                    if( (v1 + v2 + v3) - (v4 + v5 + v6) > 2){// i no centro
                        vector<int> furo(4);
                        furo[0] = i; furo[1] = j; furo[2] = k;furo[3] = l;
                        falhas.push_back(furo);
                    }else if( (v1 + v4 + v5) - (v2 + v3 + v6) > 2){// j no centro
                        vector<int> furo(4);
                        furo[0] = j; furo[1] = i; furo[2] = k;furo[3] = l;
                        falhas.push_back(furo);
                    }else if( (v2 + v4 + v6) - (v1 + v3 + v5) > 2){// k no centro
                        vector<int> furo(4);
                        furo[0] = k; furo[1] = j; furo[2] = i;furo[3] = l;
                        falhas.push_back(furo);
                    }else if( (v3 + v5 + v6) - (v1 + v2 + v4) > 2){// l no centro
                        vector<int> furo(4);
                        furo[0] = l; furo[1] = j; furo[2] = k;furo[3] = i;
                        falhas.push_back(furo);
                    }
                    
                }
            }
        }
        
    }
    
    return falhas;
}
