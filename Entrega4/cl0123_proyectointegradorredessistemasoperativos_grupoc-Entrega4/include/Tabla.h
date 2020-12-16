/*///////////////////////////////////////////////////////

    Clase Tabla.h

    -Define los metodos y los atributos correspondientes de la clase
    Tabla.cc

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
#include <vector>

#ifndef TABLA_h
#define TABLA_h

using namespace std;

/*
	Struct TablaDirecciones para manejar las regiones + ip en el servidor intermedio
*/
struct TablaDirecciones {
	string region;
	string ipv4;
};

class Tabla{

	public:
		Tabla();
		~Tabla();
		bool buscarIP(string);
		void agregarDireccion(TablaDirecciones);
		void imprimir();
		string buscarRegion(string);

	private:
		vector<TablaDirecciones> tablaDirecciones;	
};
#endif
