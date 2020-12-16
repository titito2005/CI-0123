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

	string getGen();
	string getRecup();
	string getActivos();
	string getPositivos();
	string getFallecidos();

	string getHead();

	string getDia();
	string getMes();

private:
	time_t now = time(0);
	tm *ltm = localtime(&now);

	string dia;
	string mes;

	string gen;
	string recup;
	string activos;
	string positivos;
	string fallecidos;

	string param;

	string head;
};
#endif
