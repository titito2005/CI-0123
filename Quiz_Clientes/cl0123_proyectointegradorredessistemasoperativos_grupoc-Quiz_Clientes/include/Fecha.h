/*///////////////////////////////////////////////////////

    Clase Fecha.h

    -Define los metodos y los atributos correspondientes de la clase
    Fecha.cc

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#ifndef Fecha_h
#define Fecha_h

using namespace std;

class Fecha
{

public:
	Fecha();
	~Fecha();
	string generarNombre();
	string getHead();
	string getGeneral();
	string getRecuperados();
	string getActivos();
	string getPositivos();
	string getFallecidos();
	string getDia();
	string getMes();
	string getdiaPais();
	string getmesPais();

private:
	time_t now = time(0);
	tm *ltm = localtime(&now);
	string dia;
	string mes;
	string diaPais;
	string mesPais;
	string general;
	string recuperados;
	string activos;
	string positivos;
	string fallecidos;
	string param;
	string head;
};
#endif
