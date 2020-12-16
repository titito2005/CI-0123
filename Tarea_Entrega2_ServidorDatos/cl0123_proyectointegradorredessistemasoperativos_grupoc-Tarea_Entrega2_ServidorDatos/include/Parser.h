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
	vector<string> obtengaDatosActualizado(vector<string> vector, char *nombre);
	vector<string> obtengaDatosPais(vector<string> vector);
	void actualizarActualizado(char *nombre, vector<string> activos, vector<string> positivos,
							   vector<string> fallecidos, vector<string> recuperados);
	void actualizarPais(vector<string> vect);

private:
	Fecha *fecha;
};
#endif
