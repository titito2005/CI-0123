/*///////////////////////////////////////////////////////

    Clase Servidor.cc

	-Esta clase realiza un manejo del programa con una
	funcion de servidor de dato.
	-Ejecuta solitudes de un servidor de datos y se los
	provee al cliente cuando este lo solicite.

/////////////////////////////////////////////////////////*/

#include "../include/Servidor.h"

/* 
  Constructor de la clase Servidor
  Param: 
  Return: 
 */
Servidor::Servidor()
{
	s1 = new Socket(true, false);
	thread_count = 10;
	s1->Bind(8080);
	s1->Listen(5);
}

/*
  Destructor de la clase Servidor
  Param:
  Return:
*/
Servidor::~Servidor()
{
	s1->Shutdown(SHUT_RDWR);
	s1->Close();
	free(thread_handles);
}

/*
	Metodo leerArchivoHTML() lee el archivo html de paises
	Param: string archivoHTML
	Return: devuelve el archivo HTML de paises
*/
string Servidor::leerArchivoHTML(string archivoHTML)
{
	string html = "";
	string line = "";
	ifstream lecturaHTML(archivoHTML);
	if (lecturaHTML.is_open())
	{
		while (getline(lecturaHTML, line))
		{
			html += line;
		}
		lecturaHTML.close();
	}
	return html;
}

/*
	Metodo obtenerBytesDeHTML() obtiene los bytes que requiere para manejar el archivo html
	Param: string html
	Return: devuelve un string con el numero de bytes
*/
string Servidor::obtenerBytesDeHTML(string html)
{
	const char *htmlTemp = html.c_str();
	int bytesTemp = strlen(htmlTemp);
	string bytes = to_string(bytesTemp);
	return bytes;
}

/*
	Metodo respuestasBrowserHTML() obtiene los source code de las solicitudes de paises
	Param: string archivo, int codigo
	Return: devuelve un char* con el source code
*/
char *Servidor::respuestasBrowserHTML(string archivo)
{
	string html = leerArchivoHTML(archivo); 
	string bytes = obtenerBytesDeHTML(archivo);
	string respuesta = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length:" + obtenerBytesDeHTML(html) + "\n\n\n" + html;
	char *respFinal;
	respFinal = new char[respuesta.length() + 1];
	strcpy(respFinal, respuesta.c_str());
	return respFinal;
}

/*
	Metodo respuestasConsola() genera las respuestas por consola
	Param: 
	Return: string con las respuestas
*/
char * Servidor::respuestasConsola(int codigo)
{
	string cod404 = "\nError! Recurso no encontrado\n\n$";
	string cod400 = "\nError! Solicitud inválida\n\n$";
	char *respFinal;
	switch (codigo)
	{
	case 400:
		respFinal = new char[cod400.length()];
		strcpy(respFinal, cod400.c_str());
		break;
	case 404:
		respFinal = new char[cod404.length()];
		strcpy(respFinal, cod404.c_str());
		break;
	}
	return respFinal;
}

/*
	Metodo contadorSlash() cuenta los slash de la solicitud para verificar si se solicita un pais o un canton
	Param: string encabezado
	Return: el numero de slash
*/
int Servidor::contadorSlash(string encabezado)
{
	int contadorSlash = 0;
	encabezado.erase(0, encabezado.find("/"));
	encabezado = encabezado.substr(0, encabezado.find(" "));
	string encabezadoTemp = "";
	for (int i = 0; i < encabezado.size(); ++i)
	{
		if (encabezado[i] == '/')
		{
			contadorSlash++;
		}
	}
	if (contadorSlash == 1)
	{
		encabezadoTemp = encabezado.substr(encabezado.find("/") + 1, encabezado.find(" "));
		if (encabezadoTemp == "favicon.ico") //encabezado muchas veces tiraba una solicitud de favicon.ico entonces aqui se ignora
		{
			contadorSlash = 0;
		}
	}
	return contadorSlash;
}

/*
	Metodo parserSolicitud() parsea la solicitud del pais o canton generada al servidor de datos
	Param: string encabezado
	Return: el resultado del parsear la solicitud
*/
string Servidor::parserSolicitud(string encabezado)
{
	int numSlashes = contadorSlash(encabezado);
	string resultado = "";
	string tipoSolicitud = encabezado.substr(0, encabezado.find(" "));
	if (tipoSolicitud == "GET" || tipoSolicitud == "CON"){
		if (numSlashes == 1)
		{
			encabezado.erase(0, encabezado.find("/"));
			encabezado = encabezado.substr(1, encabezado.find(" "));
			for (int i = 0; i < encabezado.size(); ++i)
			{
				if (encabezado[i] != '-')
				{
					resultado += encabezado[i];
				}
				else
				{
					resultado += ' ';
				}
			}
			if (tipoSolicitud == "GET") {
				resultado.pop_back();
			}

			if (resultado.size() == 0)
			{
				resultado = "400";
			}
		}
		else if (numSlashes == 2)
		{
			encabezado.erase(0, encabezado.find("/"));
			encabezado = encabezado.substr(1, encabezado.find(" "));
			string pais = encabezado.substr(0, encabezado.find("/"));
			if (pais == "Costa-Rica")
			{
				encabezado.erase(0, encabezado.find("/") + 1);
				for (int i = 0; i < encabezado.size(); ++i)
				{
					if (encabezado[i] != '-')
					{
						resultado += encabezado[i];
					}
					else
					{
						resultado += ' ';
					}
				}
				if (tipoSolicitud == "GET") {
					resultado.pop_back();
				}
			}
			else
			{
				resultado = "400";
			}
			if (resultado.size() == 0)
			{
				resultado = "400";
			}
		}
	}
	else{
		resultado = "501";
	}
	return resultado;
}
string Servidor::imprimirEnTabla(char * respFinal, int caso){
	string respuesta = "";
	fort::char_table table;
	table.set_border_style(FT_SOLID_STYLE);
	char *token;
	char *saveptr;
	char temp[2048];
	strcpy(temp, respFinal);
	switch(caso){
		case 1:
			{
				string casos[]= {"Pais", "Total Casos","+ Casos",
				"Total Muertes", "Muertes", "Total Recup", "Recuperados", "Activos", "Criticos", "Casos/mill", "Muertes/mill",
				"Total tests", "Tests/mill", "Poblacion"};
				token = strtok_r(temp, ";", &saveptr);
				for (int cont = 0; cont < 14; cont++)
				{
					table << fort::header<< casos[cont] << token << fort::endr;
					token = strtok_r(NULL, ";", &saveptr);
				}
				table<<fort::endr;
				table.column(1).set_cell_text_align(fort::text_align::center);
				table.column(3).set_cell_text_align(fort::text_align::center);
				respuesta = table.to_string();
			}
		break;
		case 2:
			{
				table << fort::header << "Codigo"<< "Provincia"<< "Codigo"<< "Canton"<< "Positivos"<<
				"Activos"<< "Recuperados"<< "Fallecidos" << fort::endr;
				token = strtok_r(temp, ";", &saveptr);
				for (int cont = 0; cont < 8; cont++)
				{
					table << token;
					token = strtok_r(NULL, ";", &saveptr);
				}
				table << fort::endr;
				table.column(1).set_cell_text_align(fort::text_align::center);
				table.column(3).set_cell_text_align(fort::text_align::center);
				respuesta = table.to_string();
			}
		break;
	}
	return respuesta;
}

/*
	Metodo hilo() el cual se crea para manejar las solicitudes al servidor mientras este sigue recibiendo nuevas
	Param: void *param
	Return: void
*/
void *hilo(void *param)
{
	HTTPSolicitud solicitud;
	Socket *s3 = (Socket *)param;
	Servidor server;
	string solicitudCliente = "";
	char buffer[2048];
	memset(buffer, 0, 2048);
	s3->Read(buffer, 2048);
	for (int i = 0; i < 3; ++i)
	{
		solicitudCliente += buffer[i];
	}

	if (solicitudCliente == "GET")
	{
		int i = 0;
		solicitudCliente = "";
		while (buffer[i] != '\n')
		{
			solicitudCliente += buffer[i];
			++i;
		}
		string solicitudFinal = server.parserSolicitud(solicitudCliente);
		int slashesCuenta = server.contadorSlash(solicitudCliente);
		if (solicitudFinal != "400")
		{
			if (slashesCuenta == 1)
			{ //pais
				bool estaPais = solicitud.obtenerDatosSolicitudPais(solicitudFinal);
				if (estaPais)
				{
					solicitud.actualizarPlantillaPais(solicitudFinal);
					char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaPaisFinal.html");
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
				else
				{
					char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod404.html");
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
			}
			else
			{ // canton
				bool estaCanton = solicitud.obtenerDatosSolicitudCanton(solicitudFinal);
				if (estaCanton)
				{
					string cantonTilde = solicitud.verificarNombreCanton(solicitudFinal);
					solicitud.actualizarPlantillaCanton(cantonTilde);
					char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCantonFinal.html");
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
				else{
					char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod404.html");
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
			}
		}
		else if(solicitudFinal == "400")
		{
			char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod400.html");
			write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
		}
		else if(solicitudFinal == "501")
		{
			char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod501.html");
			write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
		}
		else if(solicitudFinal == "505")
		{
			char *respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod505.html");
			write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
		}
	}
	else
	{ // consola
		int i = 0;
		solicitudCliente = "";
		while (buffer[i] != '\n')
		{
			solicitudCliente += buffer[i];
			++i;
		}
		string solicitudFinal = server.parserSolicitud(solicitudCliente);
		int slashesCuenta = server.contadorSlash(solicitudCliente);
		if (solicitudFinal != "400")
		{
			if (slashesCuenta == 1)
			{
				bool estaPais = solicitud.obtenerDatosSolicitudPais(solicitudFinal);
				if (estaPais)
				{
					char *respuestaTemp = solicitud.getPais(solicitudFinal);
					string respTabulada = server.imprimirEnTabla(respuestaTemp, 1);
					respTabulada = respTabulada + "$"; 
					const char * respuestaFinal = respTabulada.c_str();
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
				else
				{
					char *respuestaFinal = server.respuestasConsola(404);
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
			}
			else if (slashesCuenta == 2)
			{
				bool estaCanton = solicitud.obtenerDatosSolicitudCanton(solicitudFinal);
				if (estaCanton)
				{
					string cantonTilde = solicitud.verificarNombreCanton(solicitudFinal);
					char *respuestaTemp = solicitud.getCanton(cantonTilde);
					string respTabulada = server.imprimirEnTabla(respuestaTemp, 2);
					respTabulada = respTabulada + "$"; 
					const char * respuestaFinal = respTabulada.c_str();
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
				else
				{
					char *respuestaFinal = server.respuestasConsola(404);
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
			}
		}
		else if(solicitudFinal == "400")
		{
			char *respuestaFinal = server.respuestasConsola(400);
			write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
		}
	}
	s3->Close();
	return NULL;
}

/*
	Metodo atenderSolicitudes() el cual atiende las solicitudes que recibe el servidor
	Param: 
	Return: void
*/
void Servidor::atenderSolicitudes()
{
	thread_handles = (pthread_t *)calloc(thread_count, sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++)
	{
		s2 = s1->Accept();
		pthread_create(&thread_handles[thread], NULL, hilo, (void *)s2);
	}

	for (thread = 0; thread < thread_count; thread++)
	{
		pthread_join(thread_handles[thread], NULL);
	}
}
