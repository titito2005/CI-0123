/*///////////////////////////////////////////////////////

    Clase Fecha.cc

	-Esta clase realiza un manejo de los formatos de fecha
	para el resto de clases que requieran de su implemen-
	tacion.

/////////////////////////////////////////////////////////*/

#include "../include/Fecha.h"

/* 
  Constructor de la clase Fecha
  Param: 
  Return: 
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
  Destructor de la clase Fecha
  Param:
  Return:
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
	Metodo getGeneral() devuelve el formato para la solicitud de numero de casos generales de los archivos .csv
	Param:
	Return: devuelve el formato de casos generales
*/
string Fecha::getGeneral()
{
	this->general = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_GENERAL.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n\0";

	return general;
}

/*
	Metodo getRecup() devuelve el formato para la solicitud de numero de casos recuperados de los archivos .csv
	Param:
	Return: devuelve el formato de casos recuperados
*/
string Fecha::getRecuperados()
{
	this->recuperados = "GET /oges/archivos_covid/" + mes + "_" + dia + "/" + mes + "_" + dia + "_CSV_RECUP.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n\0";
	return recuperados;
}

/*
	Metodo getActivos() devuelve el formato para la solicitud de numero de casos activos de los archivos .csv
	Param:
	Return: devuelve el formato de casos activos
*/
string Fecha::getActivos()
{
	this->activos = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_ACTIVOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n\0";
	return activos;
}

/*
	Metodo getPositivos() devuelve el formato para la solicitud de numero de casos positivos de los archivos .csv
	Param:
	Return: devuelve el formato de casos positivos
*/
string Fecha::getPositivos()
{
	this->positivos = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_POSITIVOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n\0";
	return positivos;
}

/*
	Metodo getFallecidos() devuelve el formato para la solicitud de numero de casos fallecidos de los archivos .csv
	Param:
	Return: devuelve el formato de casos falecidos
*/
string Fecha::getFallecidos()
{
	this->fallecidos = "GET /oges/archivos_covid/" + this->mes + "_" + this->dia + "/" + this->mes + "_" + this->dia + "_CSV_FALLECIDOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n\0";
	return fallecidos;
}

/*
	Metodo getHead() devuelve el formato para la solicitud de head de la solicitud HTTP
	Param:
	Return: el head de la solicitud HTTP
*/
string Fecha::getHead()
{
	this->head = "HEAD /oges/archivos_covid/" + mes + "_" + dia + "/" + mes + "_" + dia + "_CSV_GENERAL.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n\0";
	return head;
}

/*
	Metodo getDia() devuelve el dia actual
	Param:
	Return: el dia actual
*/
string Fecha::getDia()
{
	return dia;
}

/*
	Metodo getMes() devuelve el mes actual
	Param:
	Return: el mes actual
*/
string Fecha::getMes()
{
	return mes;
}

/*
	Metodo getDiaPais() devuelve el dia para un pais
	Param:
	Return: el dia actual
*/
string Fecha::getdiaPais()
{
	int temp = (ltm->tm_mday);
	this->diaPais = to_string(temp);
	return diaPais;
}

/*
	Metodo getMesPais() devuelve el mes para un pais
	Param:
	Return: el mes actual
*/
string Fecha::getmesPais()
{
	int temp2 = (1 + ltm->tm_mon);
	this->mesPais = to_string(temp2);
	return mesPais;
}
