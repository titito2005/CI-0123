/*///////////////////////////////////////////////////////

    Clase ServidorIntermedio.cc

    -Efectua la comunicacion entre el cliente y el servidor
	de datos.
	- Filtra la solicitud del cliente, verifica la informacion
	que tiene en su tabla de direcciones y se comunica con el
	servidor de datos.

/////////////////////////////////////////////////////////*/

#include "../include/ServidorIntermedio.h"
#include "../include/Mutex.h"
Tabla tablaIP;
Mutex *mutex;
Mutex *mutex2;

/*
	Contructor de ServidorIntermedio
*/
ServidorIntermedio::ServidorIntermedio() {
	generarBroadcast();
}

/*
	Destructor de ServidorIntermedio
*/
ServidorIntermedio::~ServidorIntermedio() {
	free(hilosIniciales);
}

/*
	Metodo generarBroadcast() el cual envia un broadcast desde el servidor intermedio
	Param:
	Return: void
*/
void ServidorIntermedio::generarBroadcast() {
	Socket* socketBroadcast = new Socket(false, false);
	char* mensaje = "1,172.16.123.51";
	string direccionesBroadcast[8] = { "172.16.123.15", "172.16.123.31", "172.16.123.47", "172.16.123.63", "172.16.123.79", "172.16.123.95", "172.16.123.111", "172.16.123.127" };
	
	for (int i = 0; i < 8; i++) {
		socketBroadcast->broadcast(mensaje, (char*)direccionesBroadcast[i].c_str(), 51000);
	}

	socketBroadcast->Close();
}

/*
	Metodo hiloUDP() el cual crea un pthread hilo UDP para manejar las consultas del protocolo PIRO
	Param: void* parametro
	Return: void*
*/
void* hiloUDP(void* parametro) {
	cout << "SOY UN HILO UDP" << endl;
	char* buffer = (char*)parametro;
	int tam = strlen(buffer);
	char* line;
	char* token;
	char* saveptr;
	int chars = 0;

	string input(buffer);
	stringstream input_stringstream(input);
	getline(input_stringstream, input, ',');
	if (input.compare("0") == 0) {
		string token2;
		string token3;
		string aux;
		getline(input_stringstream, aux, ',');
		if (!tablaIP.buscarIP(aux)) {
			struct TablaDirecciones direccion;
			getline(input_stringstream, token2, ',');

			direccion.ipv4 = aux;
			direccion.region = token2;
			mutex->Lock();
			tablaIP.agregarDireccion(direccion);
			mutex->Unlock();
			
			}
		}
	
	tablaIP.imprimir();
	return NULL;
}

/*
	Metodo hiloTCP() el cual crea un pthread hiloTCP parsa manejar las consultas el protocolo HTTP
	Param: void* parametro
	Return: void*
*/
void* hiloTCP(void* parametro) {
	cout << "SOY UN HILO TCP" << endl;
	char buffer[512];
	char respuesta[2048];

	memset(buffer, 0, 512);
	memset(respuesta, 0, 2048);
	Socket* s3 = (Socket*)parametro;
	//Leo la consulta del cliente
	s3->Read(buffer, 512);
	//Parseo la consulta del cliente
	string temp = "";
	string solicitud(buffer);

	for (int i = 0; i < 3; i++) {
		temp += solicitud[i];
	}

	if (temp == "GET") {
		temp = solicitud;
		temp = temp.erase(0, temp.find("?")+1);
        temp = temp.substr(0, temp.find(" "));
    } else {
		temp = solicitud;
	}
	//Parseo la region
	string region = temp;
	region = region.substr(0, region.find("&"));
	region.erase(0, region.find("=")+1);
	
	bool esCanton = false;
	string canton = temp.erase(0, temp.find("&")+1);
	canton = temp.substr(0, temp.find("="));
	if(canton == "canton"){
		esCanton = true;
	}
	
	if(esCanton){
		Socket* s4 = new Socket(true, false);
		s4->Connect("172.16.123.51", 8080);
		s4->Write((char *)temp.c_str(), strlen((char *)temp.c_str()));
		s4->Read(respuesta, 2048);
		s3->Write(respuesta, strlen(respuesta));
		s4->Close();		
	
	}else{

		//Buscar en la tabla que la region se encuentre
		mutex2->Lock();
		string ip = tablaIP.buscarRegion(region);
		mutex2->Unlock();
		
		if (ip != "0") {
			Socket* s4 = new Socket(true, false);
			s4->Connect((char *)ip.c_str(), 8080);
			s4->Write((char *)temp.c_str(), strlen((char *)temp.c_str()));
			s4->Read(respuesta, 2048);

			s3->Write(respuesta, strlen(respuesta));
			s4->Close();
		} else {
			char * mensaje = "El servidor con la region solicitada no se encuentra disponible";
			s3->Write(mensaje, strlen(mensaje));
		}	
	}

	s3->Close();
	return NULL;
}

/*
	Metodo crearhiloTCP() el cual crea un pthread de hiloTCP
	Param: void* parametro
	Return: void*
*/
void* crearhiloTCP(void* parametro) {
	cout << "CREADOR HILO TCP" << endl;
	pthread_t* hilosTCP;
	hilosTCP = (pthread_t*)calloc(10, sizeof(pthread_t));
	int thread = 0;
	Socket* s1;
	Socket* s2;
	s1 = new Socket(true, false);
	s1->Bind(8080);
	s1->Listen(5);

	for (thread = 0; thread < 10; thread++) 	{
		s2 = s1->Accept();
		pthread_create(&hilosTCP[thread], NULL, hiloTCP, (void*)s2);
		}

	for (thread = 0; thread < 10; thread++) 	{
		pthread_join(hilosTCP[thread], NULL);
		}
	s1->Shutdown(SHUT_RDWR);
	s1->Close();
	free(hilosTCP);
	return NULL;
}

/*
	Metodo crearhiloUDP() el cual crea un pthread de hiloUDP
	Param: void* parametro
	Return: void*
*/
void* crearhiloUDP(void* parametro) {
	cout << "CREADOR HILO UDP" << endl;
	pthread_t* hilosUDP;
	hilosUDP = (pthread_t*)calloc(10, sizeof(pthread_t));
	int thread = 0;
	
	
	for (thread = 0; thread < 10; thread++) 	{
		Socket* s1 = new Socket(false, false);
		s1->Bind(50000);
		char* buffer[512];
		//struct sockaddr_in other;
		struct sockaddr_in other;
		memset(&other, 0, sizeof(other));
		
		other.sin_family = AF_INET;
		other.sin_port = htons(50000);
		other.sin_addr.s_addr = INADDR_ANY;
		
		s1->recvFrom(buffer, 512, (void*)&other);
		cout << "RECIBI MENSAJE: " ;
		pthread_create(&hilosUDP[thread], NULL, hiloUDP, (void*)buffer);
		s1->Close();
	}

	for (thread = 0; thread < 10; thread++) {
		pthread_join(hilosUDP[thread], NULL);
	}

	free(hilosUDP);
	return NULL;
}

/*
	Metodo crearHilos() el cual crea ulos hilos iniciales 
	Param: 
	Return: void
*/
void ServidorIntermedio::crearHilos() {
	int cont = 0;
	mutex= new Mutex();
	mutex2= new Mutex();
	hilosIniciales = (pthread_t*)calloc(2, sizeof(pthread_t));


	pthread_create(&hilosIniciales[cont], NULL, crearhiloTCP, (void*)cont);
	cont++;

	pthread_create(&hilosIniciales[cont], NULL, crearhiloUDP, (void*)cont);


	for (cont = 0; cont < 2; cont++) 	{
		pthread_join(hilosIniciales[cont], NULL);
	}
}

	



