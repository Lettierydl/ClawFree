#ifndef FGRAPH_H
#define FGRAPH_H


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

using namespace std;

class FileGraph{
public:
	static void loadFile(string fileName);
};

#endif
