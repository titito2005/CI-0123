/*///////////////////////////////////////////////////////

    Clase HTTPRequest.cc

	-Esta clase realiza un manejo de los request que se realizaran al servidor
	para solicitar los datos. 
	-Realiza un manejo de los archivos .csv, 
	en los cuales se parsearan los datos solicitados.
	-Realiza un manejo de errores de los source code siempre que se soliciten
	los datos mediante un HTTP Request.

/////////////////////////////////////////////////////////*/

#include "../include/HTTPRequest.h"
using namespace std;

/*
	Metodo HTTPRequest() constructor vacío

*/
HTTPRequest::HTTPRequest()
{
	parser = new Parser();
	fecha = new Fecha();
}

/*
	Metodo HTTPRequest() destructor vacío

*/
HTTPRequest::~HTTPRequest()
{
}
/*
	Metodo sourceCodeErrors() indica el mensaje correspondiente al source code
	Param: int code
	Return: el mensaje del source code correspondiente
*/
string HTTPRequest::sourceCodeErrors(int code)
{
	string message = "";

	switch (code)
	{
	case 200:
		message = "Ok";
		break;

	case 400:
		message = "La petición es inválida";
		break;

	case 404:
		message = "No se encontró el recurso solicitado";
		break;

	case 501:
		message = "No implementado";
		break;

	case 505:
		message = "La versión de HTTP no puede ser soportada";
		break;
	}

	cout << "El archivo correspondiente al dia de hoy no se encuentra disponible" << endl;
	cout << "Se procedera a mostrar un archivo con la fecha anterior mas reciente\n"
		 << endl;

	return message;
}

/*
	Metodo requestSourceCode() extrae el source code del head cuando se ejecuta el HTTP request
	Param: 
	Return: source code del request
*/
int HTTPRequest::requestSourceCode()
{
	Socket s(true, false);
	int size = 512;
	char a[size];
	int aux = 0;
	s.Connect("201.196.149.103", 80);
	string head = fecha->getHead();
	char *request = const_cast<char *>(head.c_str());
	s.Write((char *)request, 180);
	memset(a, 0, size);
	s.Read(a, 245);

	string code = "";
	for (int i = 0; i < 12; i++)
	{
		if (i > 8 && i < 12)
		{
			code += a[i];
		}
	}
	int codigo = stoi(code);
	return codigo;
}

/*
	Metodo requestData() realiza los request de los datos al servidor
	Param: string r, char* nombre
	Return: void
*/
void HTTPRequest::requestData(string r, char *nombre)
{
	Socket s(true, false);
	int size = 512;
	char a[size];
	int aux = 0;

	s.Connect("201.196.149.103", 80);

	FILE *archivo;
	archivo = fopen(nombre, "wt");

	char *request = const_cast<char *>(r.c_str());

	s.Write((char *)request, 180);

	memset(a, 0, size);
	s.Read(a, 245);

	memset(a, 0, size);
	aux = s.Read(a, size);
	while (aux != 0)
	{
		fwrite(a, aux, 1, archivo);
		aux = 0;
		memset(a, 0, size);
		aux = s.Read(a, size);
	}
	fclose(archivo);
}

/*
	Metodo verificarFecha() verifica la fecha de un archivo para revisar si ya se realizo una consulta hoy al servidor
	Param:
	Return: Devuelve 0 si no se ha realizado o un 1 si ya se realizo la consulta
*/
bool HTTPRequest::verificarFecha()
{
	bool resp = false;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;

	FILE *archivo1;
	archivo1 = fopen("../BaseDatos/Actualizado.csv", "r");
	chars = getline(&line, &tam, archivo1);
	fclose(archivo1);
	if (line == fecha->generarNombre())
	{
		resp = true;
	}
	return resp;
}

/*
	Metodo imprimirProv() imprime por consola los resultados por provincia de manera tabular
	Param: string prov
	Return: void
*/
void HTTPRequest::imprimirProvincia(string prov)
{
	int posi = 0;
	int activos = 0;
	int recuperados = 0;
	int fallecidos = 0;

	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	char *last;

	archivo = fopen("../BaseDatos/Actualizado.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}
	printf("%-20s%-20s%-20s%-20s%-20s%-20s\n\n", "Provincia", "Cantón", "Positivos", "Activos", "Recuperados", "Fallecidos");
	chars = getline(&line, &tam, archivo);
	chars = getline(&line, &tam, archivo);
	while (chars > 0)
	{
		vector<int> booleanos = {0, 0, 0};
		chars = getline(&line, &tam, archivo);
		token = strtok_r(line, ";", &saveptr);
		string provSubStr = prov.substr(0, 3);
		for (int i = 0; i < provSubStr.size(); ++i)
		{
			if (provSubStr[i] == token[i])
			{
				booleanos[i] = 1;
			}
		}
		int entra = checkArray(booleanos);
		if (prov == token || entra)
		{
			printf("%-20s", token);
			int cont = 0;
			while (NULL != token)
			{
				token = strtok_r(NULL, ";", &saveptr);
				if (token != NULL)
				{
					if (cont == 1)
					{
						posi += stoi(token);
					}
					if (cont == 2)
					{
						activos += stoi(token);
					}
					if (cont == 3)
					{
						recuperados += stoi(token);
					}
					if (cont == 4)
					{
						fallecidos += stoi(token);
					}

					printf("%-20s", token);
				}
				cont++;
			}
			cout << "\n";
		}
	};
	printf("%-20s%-20s%-20d%-20d%-20d%-20d\n\n", "Total", " ", posi, activos, recuperados, fallecidos);
	cout << "\n";
}

/*
	Metodo imprimirPais() imprime por consola los resultados por pais de manera tabular
	Param:
	Return: void
*/
void HTTPRequest::imprimirPais()
{
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	char *last;

	archivo = fopen("../BaseDatos/Paises.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}
	printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n\n", "País", "Posi", "+Posi", "Falle", "+Falle", "UCI", "+UCI", "Recup", "+Recup", "Muestras", "Activos");

	chars = getline(&line, &tam, archivo);
	chars = getline(&line, &tam, archivo);
	chars = getline(&line, &tam, archivo);
	token = strtok_r(line, ";", &saveptr);
	printf("%-12s", token);
	while (NULL != token)
	{
		token = strtok_r(NULL, "; \r", &saveptr);
		if (token != NULL)
		{
			printf("%-12s", token);
		}
	}
	cout << "\n";
}

/*
	Metodo checkArray() es para revisar si un substring posee n letras que coinciden con el nombre de una provincia
	Param: vector<int> arr
	Return: 1 para true o 0 para falso
*/
int HTTPRequest::checkArray(vector<int> arr)
{
	int n = 0;
	int contador = 0;
	for (int i = 0; i < arr.size(); ++i)
	{
		if (arr[i] != 0)
		{
			contador++;
		}
	}
	if (contador == arr.size())
	{
		n = 1;
	}
	return n;
}

/*
	Metodo solicitarDatos() solicita los datos al servidor
	Param:
	Return: void
*/
void HTTPRequest::solicitarDatos()
{
	activos.resize(82);
	fallecidos.resize(82);
	positivos.resize(82);
	recuperados.resize(82);
	general.resize(10);

	//REQUESTS
	requestData(fecha->getRecup(), "../BaseDatos/Recuperados.csv");
	requestData(fecha->getActivos(), "../BaseDatos/Activos.csv");
	requestData(fecha->getFallecidos(), "../BaseDatos/Fallecidos.csv");
	requestData(fecha->getPositivos(), "../BaseDatos/Positivos.csv");
	requestData(fecha->getGen(), "../BaseDatos/General.csv");
	//PARSERS
	activos = parser->obtengaDatosActualizado(activos, "../BaseDatos/Activos.csv");
	fallecidos = parser->obtengaDatosActualizado(fallecidos, "../BaseDatos/Fallecidos.csv");
	positivos = parser->obtengaDatosActualizado(positivos, "../BaseDatos/Positivos.csv");
	recuperados = parser->obtengaDatosActualizado(recuperados, "../BaseDatos/Recuperados.csv");
	general = parser->obtengaDatosPais(general);
	//ACTUALIZA
	parser->actualizarPais(general);
	parser->actualizarActualizado("../BaseDatos/Activos.csv", activos, positivos, fallecidos, recuperados);
}
