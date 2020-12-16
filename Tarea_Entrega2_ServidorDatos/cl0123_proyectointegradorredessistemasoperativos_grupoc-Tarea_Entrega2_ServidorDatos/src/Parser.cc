#include "../include/Parser.h"

/*
	Metodo Parser() constructor vacío

*/
Parser::Parser()
{
	fecha = new Fecha();
}

/*
	Metodo ~Parser() destructor vacío

*/
Parser::~Parser()
{
}

/*
	Metodo actualizarActualizado() actualiza el archivo de las provincias con los datos mas recientes
	Param: char* nombre, vector<string> acti, vector<string> posi, vector<string> falle, vector<string> recu
	Return: void
*/
void Parser::actualizarActualizado(char *nombre, vector<string> activos, vector<string> positivos,
								   vector<string> fallecidos, vector<string> recuperados)
{

	fstream archivo1;

	archivo1.open("../BaseDatos/Actualizado.csv");
	archivo1 << fecha->generarNombre();
	archivo1 << "Provincia; Cantón; Positivos; Activos; Recuperados; Fallecidos\n";

	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	char *last;

	archivo = fopen(nombre, "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	for (int i = 0; i < 82; i++)
	{

		chars = getline(&line, &tam, archivo);
		token = strtok_r(line, ";", &saveptr);

		int j = 0;
		while (NULL != token && j < 4)
		{
			if (j == 1)
			{
				archivo1 << token << ";";
			}

			if (j == 3)
			{
				archivo1 << token << ";";
			}
			token = strtok_r(NULL, ";", &saveptr);
			j++;
		}
		archivo1 << positivos[i] << ";" << activos[i] << ";" << recuperados[i] << ";" << fallecidos[i] << "\n";
	}

	fclose(archivo);
	archivo1.close();
}

/*
	Metodo actualizarPais() actualiza el archivo de paises con los datos mas recientes
	Param: vector<string> vect
	Return: void
*/
void Parser::actualizarPais(vector<string> vect)
{
	fstream archivo1;
	archivo1.open("../BaseDatos/Paises.csv");
	archivo1 << fecha->generarNombre();
	archivo1 << "Pais;Positivos;+Positivos;Fallecidos;+Fallecidos;UCI;+UCI;Recuperados;+Recuperados;Muestras;+Activos\n";
	archivo1 << "CR";
	for (int i = 0; i < 10; i++)
	{
		archivo1 << ";" << vect[i];
	}
	archivo1 << "\n";
	archivo1.close();
}

/*
	Metodo obtengaDatosActualizado() parsea los datos que se obtuvieron del servidor y los guarda en los vectores
	Param: vector<string> vector, char* nombre
	Return: el vector con los datos del archivo parseados
*/
vector<string> Parser::obtengaDatosActualizado(vector<string> vector, char *nombre)
{
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	char *last;
	archivo = fopen(nombre, "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}
	chars = getline(&line, &tam, archivo);
	for (int i = 0; i < 82; i++)
	{
		chars = getline(&line, &tam, archivo);
		token = strtok_r(line, ";", &saveptr);
		while (NULL != token)
		{
			last = token;
			token = strtok_r(NULL, ";", &saveptr);
			if (token != NULL)
			{
			}
		}
		char *saveptr1;
		token = strtok_r(last, "\r", &saveptr1);
		vector[i] = last;
	}
	fclose(archivo);
	return vector;
}

/*
	Metodo obtengaDatosPais() parsea los datos que se obtuvieron del servidor y los guarda en el vector de pais
	Param: vector<string> vector
	Return: el vector con los datos del archivo pais parseados
*/
vector<string> Parser::obtengaDatosPais(vector<string> vector)
{
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	archivo = fopen("../BaseDatos/General.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}
	chars = getline(&line, &tam, archivo);

	do
	{
		chars = getline(&line, &tam, archivo);
		token = strtok_r(line, ";", &saveptr);

		char *saveptr1;
		token = strtok_r(token, "/", &saveptr1);
		string dia = fecha->getDia();
		if (token == dia)
		{
			char *token2;
			token2 = strtok_r(NULL, "/", &saveptr1);
			string mes = fecha->getMes();
			if (token2 == mes)
			{
				int cont = 0;
				for (int i = 0; i < 55; i++)
				{
					token = strtok_r(NULL, ";", &saveptr);
					if (i == 0 || i == 1 || i == 18 || i == 19 || i == 32 || i == 33 || i == 37 || i == 38 || i == 48 || i == 54)
					{
						char *saveptr2;
						token = strtok_r(token, "\r", &saveptr2);
						vector[cont] = token;
						cont++;
					}
				}
			}
		}
	} while (chars > 0);
	fclose(archivo);
	return vector;
}
