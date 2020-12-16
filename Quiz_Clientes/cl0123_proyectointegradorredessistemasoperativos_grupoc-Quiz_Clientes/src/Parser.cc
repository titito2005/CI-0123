/*///////////////////////////////////////////////////////

    Clase Parser.cc

	-Esta clase realiza un manejo de los datos del proyecto
	para asi realizar los parser necesarios.
	-Modifica archivos .csv y plantillas HTML necesarias
	para el almacenamiento y visualizacion de los datos.

/////////////////////////////////////////////////////////*/

#include "../include/Parser.h"

/* 
  Constructor de la clase Parser
  Param: 
  Return: 
 */
Parser::Parser()
{
	fecha = new Fecha();
}

/*
  Destructor de la clase Socket
  Param:
  Return:
*/
Parser::~Parser()
{
}

/*
	Metodo actualizarActualizado() actualiza el archivo de las provincias y cantones con los datos mas recientes
	Param: vector<string> activos, vector<string> positivos, vector<string> fallecidos, vector<string> recuperados
	Return: void
*/
void Parser::actualizarActualizado(vector<string> activos, vector<string> positivos,
								   vector<string> fallecidos, vector<string> recuperados)
{
	fstream archivo1;
	archivo1.open("../BaseDatos/Actualizado.csv");
	archivo1 << fecha->generarNombre();
	archivo1 << "Código Provincia, Provincia, Código Cantón, Cantón, Positivos, Activos, Recuperados, Fallecidos\n";

	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	char *last;

	archivo = fopen("../BaseDatos/CodigoProvincias.txt", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	for (int i = 0; i < 82; i++)
	{

		chars = getline(&line, &tam, archivo);
		token = strtok_r(line, ",", &saveptr);

		for (int i = 0; i < 3; i++)
		{
			archivo1 << token << ",";
			token = strtok_r(NULL, ",", &saveptr);
			last = token;
		}

		char *saveptr1;
		token = strtok_r(last, "\n", &saveptr1);
		archivo1 << token << ",";
		archivo1 << positivos[i] << "," << activos[i] << "," << recuperados[i] << "," << fallecidos[i] << "\n";
	}

	fclose(archivo);
	archivo1.close();
}

/*
	Metodo parsearPais() el cual parsea los datos de paises y los almacena en un .csv
	Param: 
	Return: void
*/
void Parser::parsearPais()
{ //MÉTODO NUEVO*------------*********
	FILE *archivo;
	fstream csvPaises("../BaseDatos/PaisesCompletos.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;

	archivo = fopen("../BaseDatos/DatosPaises.txt", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	if (!csvPaises.is_open())
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	for (int i = 0; i < 10; i++)
	{
		chars = getline(&line, &tam, archivo);
	}

	chars = getline(&line, &tam, archivo);
	token = strtok_r(line, " ", &saveptr);
	token = strtok_r(NULL, " ", &saveptr);
	token = strtok_r(NULL, "<", &saveptr);
	char *token2;
	char *saveptr2;
	token2 = strtok_r(token, " ", &saveptr2);
	while (token2 != NULL)
	{
		csvPaises << token2;
		token2 = strtok_r(NULL, " ", &saveptr2);
		if (token2 != NULL)
		{
			csvPaises << " ";
		}
	}
	csvPaises << ";";
	
	for(int i=0; i<13; i++){
		token = strtok_r(NULL, "<", &saveptr);
		token = strtok_r(NULL, "<", &saveptr);
		
		char *token3;
		char *saveptr3;
		
		token3= strtok_r(token,">",&saveptr3);
		token3= strtok_r(NULL,">",&saveptr3);
		
		string tokenAux=token3;
		string valorFinal="";
		
		for(int i=0; i< tokenAux.size(); i++){
			if((tokenAux[i]!=' ') && (tokenAux[i]!='\n')){
				valorFinal+=tokenAux[i];
			}
		}
		if(valorFinal.size()>=1){
			csvPaises << valorFinal << ";";
		}else{
			csvPaises << "0" << ";";
		}
		
	}

	csvPaises << "\n";

	fclose(archivo);
	csvPaises.close();
}

/*
	Metodo actualizarPais() actualiza el archivo de paises con los datos del pais que recibe
	Param: vector<string> vect
	Return: void
*/
void Parser::actualizarPais(vector<string> vect)
{
	fstream archivo1;
	archivo1.open("../BaseDatos/Paises.csv");
	archivo1 << fecha->generarNombre();
	archivo1 << "Pais,Positivos,+Positivos,Fallecidos,+Fallecidos,UCI,+UCI,Recuperadosl,+Recuperados,Muestras,+Activos\n";
	archivo1 << "CR";
	for (int i = 0; i < 10; i++)
	{
		archivo1 << "," << vect[i];
	}
	archivo1 << "\n";
	archivo1.close();
}

/*
	Metodo obtenerDatosActualizado() parsea los datos que se obtuvieron del servidor y los guarda en los vectores
	Param: vector<string> vector, char* nombre
	Return: el vector con los datos del archivo parseados
*/
vector<string> Parser::obtenerDatosActualizado(vector<string> vector, char *nombre)
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
		token = strtok_r(line, ",", &saveptr);
		while (NULL != token)
		{
			last = token;
			token = strtok_r(NULL, ",", &saveptr);
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
	Metodo obtenerDatosPais() parsea los datos que se obtuvieron del servidor y los guarda en el vector de pais
	Param: vector<string> vector
	Return: el vector con los datos del archivo pais parseados
*/
vector<string> Parser::obtenerDatosPais(vector<string> vector)
{
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	string mes = fecha->getmesPais();
	string dia = fecha->getdiaPais();
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
		token = strtok_r(line, ",", &saveptr);

		char *saveptr1;
		token = strtok_r(token, "/", &saveptr1);

		if (token != NULL)
		{
		}
		if (token == dia)
		{
			char *token2;
			token2 = strtok_r(NULL, "/", &saveptr1);
			if (token2 == mes)
			{
				int cont = 0;
				for (int i = 0; i < 55; i++)
				{
					token = strtok_r(NULL, ",", &saveptr);
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
