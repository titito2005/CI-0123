#include "../include/Fecha.h"

/*
	Metodo Fecha() constructor vacío, genera las fechas actuales.

*/
Fecha::Fecha()
{

	int temp = (ltm->tm_mday);
	if (ltm->tm_mday < 10)
	{
		this->dia = "0" + to_string(temp);
	}
	else
	{
		this->dia = to_string(temp);
	}

	int temp2 = (1 + ltm->tm_mon);
	if ((1 + ltm->tm_mon) < 10)
	{
		this->mes = "0" + to_string(temp2);
	}
	else
	{
		this->mes = to_string(temp2);
	}
}

/*
	Metodo Fecha() destructor vacío

*/
Fecha::~Fecha()
{
}

/*
	Metodo generarNombre() genera un string con el formato de mes + dia
	Param:
	Return: el string con el formato mes-dia
*/
string Fecha::generarNombre()
{
	param = this->mes + "_" + this->dia + "\n";
	return param;
}

/*
	Metodo getGen() devuelve el numero de casos generales de los archivos .csv
	Param:
	Return: devuelve el numero de casos generales
*/
string Fecha::getGen()
{
	this->gen = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_GENERAL.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";

	return gen;
}

/*
	Metodo getRecup() devuelve el numero de casos recuperados de los archivos .csv
	Param:
	Return: devuelve el numero de casos recuperados
*/
string Fecha::getRecup()
{
	this->recup = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_RECUP.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	return recup;
}

/*
	Metodo getActivos() devuelve el numero de casos activos de los archivos .csv
	Param:
	Return: devuelve el numero de casos activos
*/
string Fecha::getActivos()
{
	this->activos = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_ACTIVOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	return activos;
}

/*
	Metodo getPositivos() devuelve el numero de casos positivos de los archivos .csv
	Param:
	Return: devuelve el numero de casos positivos
*/
string Fecha::getPositivos()
{
	this->positivos = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_POSITIVOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	return positivos;
}

/*
	Metodo getFallecidos() devuelve el numero de casos fallecidos de los archivos .csv
	Param:
	Return: devuelve el numero de casos falecidos
*/
string Fecha::getFallecidos()
{
	this->fallecidos = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_FALLECIDOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	return fallecidos;
}

string Fecha::getHead()
{
	this->head = "HEAD /oges/archivos_covid/" + mes + "_" + dia + "/" + mes + "_" + dia + "_CSV_GENERAL.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	return head;
}

string Fecha::getDia()
{
	return dia;
}

string Fecha::getMes()
{
	return mes;
}
