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

#ifndef Servidor_h
#define Servidor_h

using namespace std;

class Servidor
{

public: //publico
	Servidor();
	~Servidor();
	void atenderSolicitudes();

private: //privado
	Socket *s1;
	Socket *s2;
	int thread_count;
	pthread_t *thread_handles;
	int childpid;
	long thread;
};
#endif
