/*///////////////////////////////////////////////////////

    Clase ServidorIntermedio.h

    -Define los metodos y los atributos correspondientes de la clase
    ServidorIntermedio.cc

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
#include "../include/Tabla.h"
#include "fort.hpp"
#include <vector>

#ifndef ServidorIntermedio_h
#define ServidorIntermedio_h

using namespace std;

class ServidorIntermedio{

public:
	ServidorIntermedio();
	~ServidorIntermedio();
	void crearHilos();
	void generarBroadcast();

private:
	int thread_count;
	pthread_t *hilosIniciales;
	int childpid;
	long thread;
};
#endif
