/*///////////////////////////////////////////////////////

    Clase Parser.h

    -Define los metodos y los atributos correspondientes de 
	la clase Parser.cc

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stddef.h>
#include <ctime>
#include <cstring>
#include <algorithm>
#include "../include/Fecha.h"

#ifndef Parser_h
#define Parser_h

using namespace std;

class Parser
{

public:
	Parser();
	~Parser();
	vector<string> obtenerDatosActualizado(vector<string> , char *);
	vector<string> obtenerDatosPais(vector<string>);
	void actualizarActualizado(vector<string>, vector<string>, vector<string>, vector<string>);
	void actualizarPais(vector<string>);
	void parsearPais();

private:
	Fecha *fecha;
};
#endif
