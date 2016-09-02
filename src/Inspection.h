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

using namespace std;

class Inspection{
    
public:
    
    
    vector<vector<int> > inspect(vector<vector<double> > grafo);
    
};

#endif /* Inspection_h */
