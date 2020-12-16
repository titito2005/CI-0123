/*///////////////////////////////////////////////////////

    Clase HTTPSolicitud.cc

	-Esta clase realiza un manejo de los request que se realizaran al servidor
	para solicitar los datos. 
	-Realiza un manejo de los archivos .csv, 
	en los cuales se parsearan los datos solicitados.
	-Realiza un manejo de errores de los source code siempre que se soliciten
	los datos mediante un HTTP Request.

/////////////////////////////////////////////////////////*/

#include "../include/HTTPSolicitud.h"

/* 
  Constructor de la clase HTTPSolicitud
  Param: 
  Return: 
 */
HTTPSolicitud::HTTPSolicitud()
{
	parser = new Parser();
	fecha = new Fecha();
}

/*
  Destructor de la clase HTTPSolicitud
  Param:
  Return:
*/
HTTPSolicitud::~HTTPSolicitud()
{
}


/*
	Metodo solicitarCodigo() extrae el source code del head cuando se ejecuta el HTTP request
	Param: 
	Return: source code del request
*/
int HTTPSolicitud::solicitarCodigo()
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
	Metodo solicitarDatosServidor() realiza los request de los datos al servidor
	Param: string r, char* nombre
	Return: void
*/
void HTTPSolicitud::solicitarDatosServidor(string r, char *nombre)
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
	Metodo obtenerDatosSolicitudPais() verifica si los datos de la solicitud de un pais son validos
	Param: string nombre
	Return: devuelve 1 si es valido, 0 lo contrario
*/
bool HTTPSolicitud::obtenerDatosSolicitudPais(string nombre)
{
	bool paisEncontrado = false;

	bool verificacionNombre = verificarNombrePais(nombre);
	if (verificacionNombre)
	{
		bool verificacionFecha = verificarFechaPais();
		if (verificacionFecha)
		{
			bool revisarCSVPais = verificarPaisCSV(nombre);
			if (revisarCSVPais)
			{
				paisEncontrado = true;
			}
			else
			{
				int codigoRespuesta = solicitarCodigoPaises(nombre);
				if (codigoRespuesta == 200)
				{
					solicitarDatosPaises(nombre);
					parser->parsearPais();
					paisEncontrado = true;
				}
			}
		}
		else
		{
			int codigoRespuesta = solicitarCodigoPaises(nombre);
			if (codigoRespuesta == 200)
			{
				solicitarDatosPaises(nombre);
				parser->parsearPais();
				paisEncontrado = true;
			}
		}
	}
	else
	{
		paisEncontrado = false;
	}
	return paisEncontrado;
}

/*
	Metodo obtenerDatosSolicitudCanton() verifica si los datos de la solicitud de un canton son validos
	Param: string nombre
	Return: devuelve 1 si es valido, 0 lo contrario
*/
bool HTTPSolicitud::obtenerDatosSolicitudCanton(string nombre)
{
	bool cantonEncontrado = false;
	string nombreEncontrado= verificarNombreCanton(nombre);
	if (nombreEncontrado!="0")
	{
		bool verifiFecha = verificarFecha();
		if (!verifiFecha)
		{
			int codigo = solicitarCodigo();
			if (codigo == 200)
			{
				solicitarDatos();
				cantonEncontrado = true;
			}
			else
			{
				cantonEncontrado = true;
				//aqui hay que ver como mandar que es info vieja
			}
		}
		else
		{
			cantonEncontrado = true;
		}
	}
	return cantonEncontrado;
}

/*
	Metodo verificarFecha() verifica la fecha de un archivo para revisar si ya se realizo una consulta hoy al servidor
	Param:
	Return: Devuelve 0 si no se ha realizado o un 1 si ya se realizo la consulta el en dia actual
*/
bool HTTPSolicitud::verificarFecha()
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
	Metodo verificarFechaPais() verifica la fecha del documento .csv de paises
	Param:
	Return: Devuelve 0 si no se ha realizado o un 1 si ya se realizo la consulta el en dia actual
*/
bool HTTPSolicitud::verificarFechaPais()
{ //método nuevo 2.0
	bool actualizado = false;
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	bool encontrado = false;

	archivo = fopen("../BaseDatos/PaisesCompletos.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	fclose(archivo);

	if (line == fecha->generarNombre())
	{
		actualizado = true;
	}
	else
	{
		actualizado = false;
		ofstream ofs;
		ofs.open("../BaseDatos/PaisesCompletos.csv", ofstream::out | ofstream::trunc);
		ofs.close();

		fstream csvPaises("../BaseDatos/PaisesCompletos.csv", std::fstream::in | std::fstream::out | std::fstream::app);
		csvPaises << fecha->generarNombre();
		csvPaises.close();
	}
	return actualizado;
}

/*
	Metodo verificarNombrePais() verifica si el nombre del pais es valido
	Param: string pais
	Return: Devuelve 1 si es valido, 0 de lo contrario
*/
bool HTTPSolicitud::verificarNombrePais(string pais)
{ //Método nuevo2.0
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	bool encontrado = false;

	archivo = fopen("../BaseDatos/SolicitudPaises.txt", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	do
	{

		token = strtok_r(line, ",", &saveptr);
		token = strtok_r(NULL, ",", &saveptr);
		char *saveptr1;
		char *token1;
		token1 = strtok_r(token, "\r", &saveptr1);
		string tokenAux = token1;

		if (pais == tokenAux)
		{
			encontrado = true;
		}

		chars = getline(&line, &tam, archivo);

	} while (chars > 0 && encontrado == false);

	return encontrado;
}

/*
	Metodo verificarPaisCSV() verifica si el pais ya se encuentra en el .csv de paises
	Param: string pais
	Return: Devuelve 1 si lo encontro, 0 de lo contrario
*/
bool HTTPSolicitud::verificarPaisCSV(string pais)
{ //Método nuevo 2.0
	FILE *archivo;
	char *line;
	char *token;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	bool encontrado = false;

	archivo = fopen("../BaseDatos/PaisesCompletos.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	do
	{

		token = strtok_r(line, ";", &saveptr);
		string tokenAux = token;

		if (pais == tokenAux)
		{
			encontrado = true;
		}

		chars = getline(&line, &tam, archivo);

	} while (chars > 0 && encontrado == false);

	return encontrado;
}

/*
	Metodo solicitarCodigoPaises() devuelve el source code del request de paises
	Param: string pais
	Return: Devuelve el source code de la solicitud
*/
int HTTPSolicitud::solicitarCodigoPaises(string pais) //MÉTODO NUEVO*--------------------*
{
	Socket s(true, false);
	s.InitSSL();
	char a[1024];
	memset(a, 0, 1024);
	string paisSolicitud = "";
	for (int i = 0; i < pais.size(); ++i)
	{
		if (pais[i] != ' ')
		{
			paisSolicitud += pais[i];
		}
		else
		{
			paisSolicitud += "%20";
		}
	}
	char *os = (char *)"os.ecci.ucr.ac.cr";
	string request = "HEAD /covid/corona.cgi?country=" + paisSolicitud + " HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
	char *reqOS = const_cast<char *>(request.c_str());

	s.SSLConnect(os, (char *)"https");
	s.SSLWrite((void *)reqOS, strlen(reqOS));
	s.SSLRead(a, 1024);

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
	Metodo solicitarDatosPaises() realiza la solicitud al servidor del pais que se indica
	Param: string pais
	Return: void
*/
void HTTPSolicitud::solicitarDatosPaises(string pais)
{ 
	fstream archivo;
	archivo.open("../BaseDatos/DatosPaises.txt");
	Socket s(true, false);
	s.InitSSL();
	char a[1024];
	memset(a, 0, 1024);
	char *os = (char *)"os.ecci.ucr.ac.cr";
	string paisSolicitud = "";
	for (int i = 0; i < pais.size(); ++i)
	{
		if (pais[i] != ' ')
		{
			paisSolicitud += pais[i];
		}
		else
		{
			paisSolicitud += "%20";
		}
	}
	string request = "GET /covid/corona.cgi?country=" + paisSolicitud + " HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
	char *reqOS = const_cast<char *>(request.c_str());
	s.SSLConnect(os, (char *)"https");
	s.SSLWrite((void *)reqOS, strlen(reqOS));

	s.SSLRead(a, 1024);
	archivo << a;
	memset(a, 0, 1024);

	s.SSLRead(a, 1024);
	archivo << a;
	memset(a, 0, 1024);
	archivo.close();
}

/*
	Metodo solicitarDatos() solicita los datos al servidor
	Param:
	Return: void
*/
void HTTPSolicitud::solicitarDatos()
{

	activos.resize(82);
	fallecidos.resize(82);
	positivos.resize(82);
	recuperados.resize(82);
	general.resize(10);

	//REQUESTS
	solicitarDatosServidor(fecha->getRecuperados(), "../BaseDatos/Recuperados.csv");
	solicitarDatosServidor(fecha->getActivos(), "../BaseDatos/Activos.csv");
	solicitarDatosServidor(fecha->getFallecidos(), "../BaseDatos/Fallecidos.csv");
	solicitarDatosServidor(fecha->getPositivos(), "../BaseDatos/Positivos.csv");
	solicitarDatosServidor(fecha->getGeneral(), "../BaseDatos/General.csv");
	//PARSERS

	activos = parser->obtenerDatosActualizado(activos, "../BaseDatos/Activos.csv");
	fallecidos = parser->obtenerDatosActualizado(fallecidos, "../BaseDatos/Fallecidos.csv");
	positivos = parser->obtenerDatosActualizado(positivos, "../BaseDatos/Positivos.csv");
	recuperados = parser->obtenerDatosActualizado(recuperados, "../BaseDatos/Recuperados.csv");
	general = parser->obtenerDatosPais(general);

	//ACTUALIZA
	parser->actualizarPais(general);
	parser->actualizarActualizado(activos, positivos, fallecidos, recuperados);
}

/*
	Metodo actualizarPlantillaPais() actualiza la PlantillaPaisFinal.html segun el pais indicado
	Param: string nombrePais
	Return: void
*/
void HTTPSolicitud::actualizarPlantillaPais(string nombrePais)
{
	FILE *archivo;
	char *line;
	char *token;
	char *tokenAux;
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	string tokensAnt[14];

	//Recojo los datos del pais deseado --------------
	archivo = fopen("../BaseDatos/PaisesCompletos.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo); //Se brinca la primera linea

	token = strtok_r(line, ";", &saveptr); //PRIMER STRTOK de la nueva linea
	while (token != NULL)
	{

		if (token == nombrePais)
		{
			for (int i = 0; i < 14; i++)
			{
				if (token != NULL)
				{
					tokensAnt[i] = token;
				}
				token = strtok_r(NULL, ";", &saveptr); //Siguiente valor del token
			}
			token = NULL;
		}
		else
		{
			chars = getline(&line, &tam, archivo); //Se brinca la linea
			token = strtok_r(line, ";", &saveptr); //PRIMER STRTOK de la nueva linea
		}
	}
	fclose(archivo);

	//Leo los valores de la plantilla ------------------
	fstream plantillaPais("../BaseDatos/PlantillaPais.html", std::fstream::in | std::fstream::out | std::fstream::app);
	//Limpio el archivo
	ofstream ofs;
	ofs.open("../BaseDatos/PlantillaPaisFinal.html", ofstream::out | ofstream::trunc);
	ofs.close();
	//Abro el archivo
	fstream plantillaPaisFinal("../BaseDatos/PlantillaPaisFinal.html", std::fstream::in | std::fstream::out | std::fstream::app);

	//Voy escribiendo el nuevo archivo
	string linea;
	for (int i = 0; i < 24; i++)
	{
		getline(plantillaPais, linea, '\n');
		plantillaPaisFinal << linea << endl;
	}
	//Completar el resto de datos
	for (int i = 0; i < 14; i++)
	{
		plantillaPaisFinal << "<th>" + tokensAnt[i] + "</th>" << endl;
	}
	plantillaPaisFinal << "</tr>" << endl;
	plantillaPaisFinal << "</table>" << endl;
	plantillaPaisFinal << "</html>" << endl;

	plantillaPais.close();
	plantillaPaisFinal.close();
}

/*
	Metodo actualizarPlantillaCanton() actualiza la PlantillaCantonFinal.html segun el canton indicado
	Param: string nombreCanton
	Return: void
*/
void HTTPSolicitud::actualizarPlantillaCanton(string nombreCanton)
{
	FILE *archivo;
	char *line;
	char *token;
	char *retorno;
	string retornoAux = "";
	char *saveptr;
	string tokensAnt[4];
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	bool encontro = false;
	vector<string> tokens;

	archivo = fopen("../BaseDatos/Actualizado.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	chars = getline(&line, &tam, archivo);

	for (int i = 0; i < 82 && encontro == false; i++)
	{
		chars = getline(&line, &tam, archivo);

		token = strtok_r(line, ",", &saveptr); //PRIMER STRTOK SACA CODIGO DE PROVINCIA.
		tokens.push_back(token);
		for (int i = 0; i < 3; i++)
		{

			token = strtok_r(NULL, ",", &saveptr); //SEGUNDO SACA NOMBRE DE CANTON.
			tokens.push_back(token);
		}

		string cantonAux = token;

		if (nombreCanton == cantonAux)
		{
			encontro = true;
			token = strtok_r(NULL, ",", &saveptr);
			do
			{
				tokens.push_back(token);
				token = strtok_r(NULL, ",", &saveptr);
			} while (token != NULL);
		}

		if (encontro == false)
		{
			tokens.clear();
		}
	}
	fclose(archivo);


	//Leo los valores de la plantilla ------------------
	fstream plantillaCanton("../BaseDatos/PlantillaCanton.html", std::fstream::in | std::fstream::out | std::fstream::app);
	//Limpio el archivo
	ofstream ofs;
	ofs.open("../BaseDatos/PlantillaCantonFinal.html", ofstream::out | ofstream::trunc);
	ofs.close();
	//Abro el archivo
	fstream plantillaCantonFinal("../BaseDatos/PlantillaCantonFinal.html", std::fstream::in | std::fstream::out | std::fstream::app);

	//Voy escribiendo el nuevo archivo
	string linea;
	for (int i = 0; i < 19; i++)
	{
		getline(plantillaCanton, linea, '\n');
		plantillaCantonFinal << linea << endl;
	}

	for (std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		plantillaCantonFinal << "<th>" + *it + "</th>" << endl;
	}
	plantillaCantonFinal << "</tr>" << endl;
	plantillaCantonFinal << "</table>" << endl;
	plantillaCantonFinal << "</html>" << endl;

	plantillaCanton.close();
	plantillaCantonFinal.close();
}

/*
	Metodo verificarNombreCanton() verifica si encuentra el canton ingresado con el .csv de cantones
	Param: string canton
	Return: Devuelve string con el nombre del cantón con tilde si encontró el canton, 0 de lo contrario
*/
string HTTPSolicitud::verificarNombreCanton(string canton)
{ //MÉTODO NUEVO 3.0
	string nombreEncontrado="";
	
	FILE *archivo;
	char *line;
	char *token;
	char *retorno;
	string retornoAux = "";
	char *saveptr;
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	bool encontrado= false;

	archivo = fopen("../BaseDatos/cantonesTildes.txt", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	for (int i = 0; i < 82 && encontrado == false; i++)
	{
		chars = getline(&line, &tam, archivo);

		token = strtok_r(line, ",", &saveptr); //PRIMER STRTOK SACA CODIGO DE PROVINCIA.
		string tokenAux=token;
		if(tokenAux==canton){
			token = strtok_r(NULL, "\n", &saveptr);
			nombreEncontrado=token;
			encontrado=true;
		}
	}
	
	if(encontrado==false){
		nombreEncontrado="0";
	}
	return nombreEncontrado;
}

/*
	Metodo getCanton() devuelve un char* con el canton en el .csv de cantones
	Param: string canton
	Return: Devuelve el char* del canton
*/
char *HTTPSolicitud::getCanton(string canton)
{ //MÉTODO NUEVO 3.0
	FILE *archivo;
	char *line;
	char *token;
	char *retorno;
	string retornoAux = "";
	char *saveptr;
	string tokensAnt[4];
	size_t tam = 1024;
	line = (char *)calloc(1024, 1);
	int chars = 0;
	bool encontro = false;
	vector<string> tokens;

	archivo = fopen("../BaseDatos/Actualizado.csv", "r");
	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	chars = getline(&line, &tam, archivo);

	for (int i = 0; i < 82 && encontro == false; i++)
	{
		chars = getline(&line, &tam, archivo);

		token = strtok_r(line, ",", &saveptr); //PRIMER STRTOK SACA CODIGO DE PROVINCIA.
		tokens.push_back(token);
		for (int i = 0; i < 3; i++)
		{

			token = strtok_r(NULL, ",", &saveptr); //SEGUNDO SACA NOMBRE DE CANTON.
			tokens.push_back(token);
		}

		string cantonAux = token;

		if (canton == cantonAux)
		{
			encontro = true;
			token = strtok_r(NULL, ",", &saveptr);
			do
			{
				tokens.push_back(token);
				token = strtok_r(NULL, ",", &saveptr);
			} while (token != NULL);
		}

		if (encontro == false)
		{
			tokens.clear();
		}
	}
	if (encontro == true)
	{
		for (std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			string aux = *it;
			char *cstr = new char[aux.length() + 1];
			strcpy(cstr, aux.c_str());
			char *token2;
			char *saveptr;
			token2 = strtok_r(cstr, "\n", &saveptr);
			aux = token2;
			retornoAux += aux + ";";
		}
	}
	else
	{
		retornoAux = "Error, solicitud no encontrada";
	}

	retorno = new char[retornoAux.length() + 1];
	strcpy(retorno, retornoAux.c_str());

	fclose(archivo);
	return retorno;
}

/*
	Metodo getPais() devuelve un char* con el pais en el .csv de paises
	Param: string pais
	Return: Devuelve el char* del pais
*/
char *HTTPSolicitud::getPais(string pais)
{ //Método nuevo 3.0
	FILE *archivo;
	char *line;
	char *token;
	char *retorno;
	string retornoAux = "";
	string aux = "";
	char *saveptr;
	size_t tam = 1024;
	line = (char*) calloc(1024, 1);
	bool encontro = false;
	int chars = 0;
	vector<string> tokens;

	archivo = fopen("../BaseDatos/PaisesCompletos.csv", "r");

	if (NULL == archivo)
	{
		perror("El archivo no se pudo abrir");
		exit(2);
	}

	chars = getline(&line, &tam, archivo);
	chars = getline(&line, &tam, archivo);

	while (chars != 0 && encontro == false)
	{
		token = strtok_r(line, ";", &saveptr);
		aux = token;
		if (aux == pais)
		{
			encontro = true;
			tokens.push_back(token);
			token = strtok_r(NULL, ";", &saveptr);
			for (int i = 0; i < 13; i++)
			{
				tokens.push_back(token);
				token = strtok_r(NULL, ";", &saveptr);
			}
		}
		chars = getline(&line, &tam, archivo);
	}

	for (std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		string aux = *it;
		retornoAux += aux + ";";
	}

	retorno = new char[retornoAux.length() + 1];
	strcpy(retorno, retornoAux.c_str());

	fclose(archivo);
	return retorno;
}
