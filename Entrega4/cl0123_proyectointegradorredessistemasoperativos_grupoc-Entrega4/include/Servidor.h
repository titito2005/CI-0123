/*///////////////////////////////////////////////////////

    Clase Servidor.h

    -Define los metodos y los atributos correspondientes de la clase
    Servidor.cc

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "Socket.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "../include/Socket.h"
#include "../include/HTTPSolicitud.h"
#include "fort.hpp"

#ifndef Servidor_h
#define Servidor_h

using namespace std;

class Servidor
{

public:
	Servidor();
	~Servidor();
	void atenderSolicitudes();
	string leerArchivoHTML(string);
	string obtenerBytesDeHTML(string);
	char *respuestasBrowserHTML(string);
	char *respuestasConsola(int);
	string parserSolicitudBrowser(string);
	string parserSolicitud(string);
	int contadorSlash(string);
	string imprimirEnTabla(char *, int);
	void generarBroadcast();
	bool esPais(string);

private:
	int thread_count;
	pthread_t *thread_handles;
	int childpid;
	long thread;
};
#endif
