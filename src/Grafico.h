/*
 * Grafico.h
 *
 *  Created on: 13/03/2015
 *      Author: Leo
 */

#include <unistd.h>
#include <vector>
#include <list>

#include <sys/types.h>
#include <unistd.h>


#ifndef GRAFICO_H_
#define GRAFICO_H_

using namespace std;

class Grafico {
public:

    static void plotarGrafoEntrada();

	Grafico();
	virtual ~Grafico();
};


#endif /* GRAFICO_H_ */
