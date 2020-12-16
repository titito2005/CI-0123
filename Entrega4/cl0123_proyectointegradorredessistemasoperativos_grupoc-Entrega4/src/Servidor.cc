/*///////////////////////////////////////////////////////

	Clase Servidor.cc

	-Esta clase realiza un manejo del programa con una
	funcion de servidor de dato.
	-Ejecuta solitudes de un servidor de datos y se los
	provee al cliente cuando este lo solicite.

/////////////////////////////////////////////////////////*/

#include "../include/Servidor.h"
#include "../include/Mutex.h"
#include <sstream>

/*
  Constructor de la clase Servidor
  Param:
  Return:
 */
Servidor::Servidor() {
	generarBroadcast();
	thread_count = 10;
	}

/*
  Destructor de la clase Servidor
  Param:
  Return:
*/
Servidor::~Servidor() {
	free(thread_handles);
	}

/*
	Metodo leerArchivoHTML() lee el archivo html de paises
	Param: string archivoHTML
	Return: devuelve el archivo HTML de paises
*/
string Servidor::leerArchivoHTML(string archivoHTML) {
	string html = "";
	string line = "";
	ifstream lecturaHTML(archivoHTML);
	if (lecturaHTML.is_open()) 	{
		while (getline(lecturaHTML, line)) 		{
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
string Servidor::obtenerBytesDeHTML(string html) {
	const char* htmlTemp = html.c_str();
	int bytesTemp = strlen(htmlTemp);
	string bytes = to_string(bytesTemp);
	return bytes;
	}


bool Servidor::esPais(string solicitud){
	bool esPais = false;
	string temp = solicitud;
	temp.erase(0, temp.find("&")+1);
	temp = temp.substr(0, temp.find("="));
	if(temp == "pais"){
		esPais = true;
	}
	return esPais;
}

/*
	Metodo respuestasBrowserHTML() obtiene los source code de las solicitudes de paises
	Param: string archivo, int codigo
	Return: devuelve un char* con el source code
*/
char* Servidor::respuestasBrowserHTML(string archivo) {
	string html = leerArchivoHTML(archivo);
	string bytes = obtenerBytesDeHTML(archivo);
	string respuesta = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length:" + obtenerBytesDeHTML(html) + "\n\n\n" + html;
	char* respFinal;
	respFinal = new char[respuesta.length() + 1];
	strcpy(respFinal, respuesta.c_str());
	return respFinal;
	}



/*
	Metodo parserSolicitud() parsea la solicitud del pais o canton generada al servidor de datos
	Param: string encabezado
	Return: el resultado del parsear la solicitud
*/
string Servidor::parserSolicitud(string soli) {
	vector<string> temps;
	string temp = soli;
	temp.erase(0, temp.find("&")+1);
    temp.erase(0, temp.find("=")+1);
	int contGuion = 0;
	bool hayGuion = false;
	for(int i = 0; i < temp.size(); ++i){
		if(temp[i] == '-'){ 
			hayGuion = true;
			contGuion++;
		}
	}
	if(hayGuion){
		for(int i = 0; i < contGuion; ++i){
			temp[0] = toupper(temp[0]);
			temps.push_back(temp.substr(0, temp.find("-")));
			temp.erase(0, temp.find("-")+1);
		}
		temp[0] = toupper(temp[0]);
		temps.push_back(temp.substr(0, temp.size()));
		temp = "";
		temp += temps[0];
		temp += " ";
		for(int i = 1; i < temps.size(); ++i){
			temp += temps[i];
			temp += " ";
		}
		temp.pop_back();
		//temp.pop_back();
	}
	else{
		temp[0] = toupper(temp[0]);
	}
	return temp;
}



void* hiloUDP(void* param) {
	cout << "SOY HILO UDP" << endl;
	char* buffer = (char*)param;
	int tam = strlen(buffer);
	char* line;
	char* token;
	char* saveptr;
	int chars = 0;
	//token = strtok(",",buffer);

	string input(buffer);
	stringstream input_stringstream(input);
	getline(input_stringstream, input, ',');
	if (input.compare("1") == 0) {
		int i = 0;
		char* token2;
		char* saveptr2;
		//token2 = strtok(NULL,buffer);
		getline(input_stringstream, input, ',');

		Socket* socket = new Socket(false, false);

		char* mensaje = "0,172.16.123.51,eastern-europe";

		//socket->sendTo((const void*)mensaje, strlen(mensaje), &other);
		socket->broadcast(mensaje, (char*)input.c_str(), 50000);

	}
	return NULL;
	}

/*
	Metodo hilo() el cual se crea para manejar las solicitudes al servidor mientras este sigue recibiendo nuevas
	Param: void *param
	Return: void
*/
void* hiloTCP(void* param) {
	string paisesRegion [26]= {"Belarus", "Bulgaria", "Czechia", "Hungary", "Poland", "Moldova", "Romania", "Russia", "Slovakia", "Ukraine", "Albania", "Andorra", "Bosnia And Herzegovina", "Croatia", "Gibraltar", "Greece", "Vatican City", "Italy", "Malta", "Montenegro", "North Macedonia", "Portugal", "San Marino", "Serbia", "Slovenia", "Spain"};
	HTTPSolicitud solicitud;
	Socket* s3 = (Socket*)param;
	Servidor server;
	char buffer[2048];
	memset(buffer, 0, 2048);
	s3->Read(buffer, 2048);
	string solicitudCliente(buffer);
	string solicitudFinal = server.parserSolicitud(solicitudCliente);
	bool pais = false;
	pais = server.esPais(solicitudCliente); 

	if (pais) {
			bool paisNuestro = false;
			for(int i = 0; i < 26; ++i){
				if(solicitudFinal == paisesRegion[i]){
					paisNuestro = true;
				}
			}
			if(paisNuestro){
				bool estaPais = solicitud.obtenerDatosSolicitudPais(solicitudFinal);
				if (estaPais) 				{
					solicitud.actualizarPlantillaPais(solicitudFinal);
					char* respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaPaisFinal.html");
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
				else {
					char* respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod404.html");
					write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
			}
			else {
				char* respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod400.html");
				write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
			}
	}
	else{ // canton
			bool estaCanton = solicitud.obtenerDatosSolicitudCanton(solicitudFinal);
			if (estaCanton) 				{
				string cantonTilde = solicitud.verificarNombreCanton(solicitudFinal);
				solicitud.actualizarPlantillaCanton(cantonTilde);
				char* respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCantonFinal.html");
				write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
				}
			else {
				char* respuestaFinal = server.respuestasBrowserHTML("../BaseDatos/PlantillaCod404.html");
				write(s3->getIDSocket(), respuestaFinal, strlen(respuestaFinal));
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
		s1->Bind(51000);
		char* buffer[512];
		struct sockaddr other;
		memset(&other, 0, sizeof(other));
		
		s1->recvFrom(buffer, 512, (void*)&other);
		cout << "RECIBÍ MENSAJE" << endl;
		pthread_create(&hilosUDP[thread], NULL, hiloUDP, (void*)buffer);

		s1->Close();
		}

	for (thread = 0; thread < 10; thread++) 	{
		pthread_join(hilosUDP[thread], NULL);
		}

	free(hilosUDP);
	return NULL;
}

/*
	Metodo generarBroadcast() el cual envia un broadcast desde el servidor de datos
	Param:
	Return: void
*/
void Servidor::generarBroadcast() {
	Socket* socketBroadcast = new Socket(false, false);
	char* mensaje = "0,172.16.123.51,eastern-europe";
	string direccionesBroadcast[8] = { "172.16.123.15", "172.16.123.31", "172.16.123.47", "172.16.123.63", "172.16.123.79", "172.16.123.95", "172.16.123.111", "172.16.123.127" };
	for (int i = 0; i < 8; i++) {
		socketBroadcast->broadcast(mensaje, (char*)direccionesBroadcast[i].c_str(), 50000);
	}
	socketBroadcast->Close();
}

/*
	Metodo atenderSolicitudes() el cual atiende las solicitudes que recibe el servidor
	Param:
	Return: void
*/
void Servidor::atenderSolicitudes() {
	int cont = 0;
	thread_handles = (pthread_t*)calloc(2, sizeof(pthread_t));

	pthread_create(&thread_handles[cont], NULL, crearhiloTCP, (void*)cont);

	cont++;

	pthread_create(&thread_handles[cont], NULL, crearhiloUDP, (void*)cont);


	for (cont = 0; cont < 2; cont++) 	{
		pthread_join(thread_handles[cont], NULL);
	}
}
