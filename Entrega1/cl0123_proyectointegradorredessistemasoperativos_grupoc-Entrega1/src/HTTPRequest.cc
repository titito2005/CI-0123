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
	Metodo sourceCodeErrors() indica el mensaje correspondiente al source code
	Param: int code
	Return: el mensaje del source code correspondiente
*/
string HTTPRequest::sourceCodeErrors(int code) {
	string message = "";

	switch(code) {
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

	cout << "El archivo correspondiente al dia de hoy no se encuentra disponible" <<endl;
	cout << "Se procedera a mostrar un archivo con la fecha anterior mas reciente\n" <<endl;

	return message;
}

/*
	Metodo requestSourceCode() extrae el source code del head cuando se ejecuta el HTTP request
	Param: 
	Return: source code del request
*/
int HTTPRequest::requestSourceCode() {
	Socket s( true, false );
	int size=512;
	char a[size];
	int aux=0;
	formatoDia();
	formatoMes();
	s.Connect( "201.196.149.103", 80 );
	string fecha = "HEAD /oges/archivos_covid/"+mes+"_"+dia+"/"+mes+"_"+dia+"_CSV_GENERAL.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	char * request= const_cast<char*>(fecha.c_str());
	s.Write( (char *) request, 180);
	memset( a, 0, size);
    s.Read( a, 245);
	
	string code = "";
	for (int i = 0; i < 12; i++) {
		if(i > 8 && i < 12){
			code += a[i];
		}
	}
	int codigo = stoi(code);
	return codigo;
}

/*
	Metodo generarStrings() genera los strings para hacer los gets al server
	Param: 
	Return: void
*/
void HTTPRequest::generarStrings (){
	formatoDia();
	formatoMes();
	
	this->gen = "GET /oges/archivos_covid/"+this->mes+"_"+this->dia+"/"+this->mes+"_"+this->dia+"_CSV_GENERAL.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	this->recup = "GET /oges/archivos_covid/"+this->mes+"_"+this->dia+"/"+this->mes+"_"+this->dia+"_CSV_RECUP.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	this->activos = "GET /oges/archivos_covid/"+this->mes+"_"+this->dia+"/"+this->mes+"_"+this->dia+"_CSV_ACTIVOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	this->positivos = "GET /oges/archivos_covid/"+this->mes+"_"+this->dia+"/"+this->mes+"_"+this->dia+"_CSV_POSITIVOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
	this->fallecidos = "GET /oges/archivos_covid/"+this->mes+"_"+this->dia+"/"+this->mes+"_"+this->dia+"_CSV_FALLECIDOS.csv HTTP/1.1\r\nhost:geovision.uned.ac.cr\r\n\r\n";
}

/*
	Metodo formatoDia() escribe en el string dia la fecha del dia actual
	Param: 
	Return: void
*/
void HTTPRequest::formatoDia(){
	int temp = (ltm->tm_mday);
	if(ltm->tm_mday<10){
		this->dia = "0"+to_string(temp);
	}
	else{
		this->dia = to_string(temp);
	}
}

/*
	Metodo formatoMes() escribe en el string mes la fecha del mes actual
	Param: 
	Return: void
*/
void HTTPRequest::formatoMes(){
	int temp2 = (1+ltm->tm_mon);	
	if((1+ltm->tm_mon)<10){
		this->mes = "0"+to_string(temp2);
	}
	else{
		this->mes = to_string(temp2);
	}
}

/*
	Metodo requestData() realiza los request de los datos al servidor
	Param: string r, char* nombre
	Return: void
*/
void HTTPRequest::requestData(string r, char* nombre){
	Socket s( true, false );
	int size=512;
	char a[size];
	int aux=0;

	s.Connect( "201.196.149.103", 80 );

	FILE *archivo;
	archivo= fopen(nombre,"wt");

	char * request= const_cast<char*>(r.c_str());

	s.Write( (char *) request, 180);

	memset( a, 0, size);
	s.Read( a, 245);
 
 	memset( a, 0, size);
 	aux=s.Read( a, size);
 	while(aux!=0){
 	fwrite(a, aux,1,archivo);
 	aux=0;
 	memset(a, 0, size);
 	aux=s.Read(a, size);
 	}  
	fclose(archivo);
}

/*
	Metodo generarNombre() genera un string con el formato de mes + dia
	Param:
	Return: el string con el formato mes-dia
*/
string HTTPRequest::generarNombre(){
	formatoDia();
	formatoMes();
	string param = this->mes+"_"+this->dia+"\n";
	return param;
}

/*
	Metodo actualizar() actualiza el archivo de las provincias con los datos mas recientes
	Param: char* nombre, vector<string> acti, vector<string> posi, vector<string> falle, vector<string> recu
	Return: void
*/
void HTTPRequest::actualizar(char* nombre,vector<string> acti,vector<string> posi,vector<string> falle, vector<string> recu){
	fstream archivo1;

   	archivo1.open("../BaseDatos/Actualizado.csv");
	archivo1 << generarNombre();
	archivo1 << "Provincia, Cantón, Positivos, Activos, Recuperados, Fallecidos\n";  
	FILE *archivo;
	char *line;
	char * token;
	char *saveptr;
	size_t tam=1024;
	line = (char *)calloc( 1024, 1 );
	int chars=0;
	char *last;

	archivo = fopen( nombre, "r" );
	if ( NULL == archivo ) {
		perror( "El archivo no se pudo abrir");
		exit( 2 );
	}
   	
   	chars = getline( &line, &tam, archivo);
   	for(int i=0; i<82; i++){
   	
   		chars = getline( &line, &tam, archivo);			
		token = strtok_r( line, ",", &saveptr);
	
		int j=0;
  		while ( NULL != token ) {
			if(j==1){
  			archivo1<<token<<","<<"";
  			}
  			
  			if(j==3){
  			archivo1<<token<<","<<"";
  			}
  			token = strtok_r( NULL, ",", &saveptr);
  			j++;
   		}
   		archivo1<<posi[i]<<","<<acti[i]<<","<<recu[i]<<","<<falle[i]<<"\n";
    }
    
    fclose(archivo);  	
    archivo1.close();
}

/*
	Metodo actualizarPais() actualiza el archivo de paises con los datos mas recientes
	Param: vector<string> vect
	Return: void
*/
void HTTPRequest::actualizarPais(vector<string> vect){
 	fstream archivo1;
 	archivo1.open("../BaseDatos/Paises.csv");
 	archivo1 << generarNombre();
	archivo1<<"Pais,Positivos,NuevosPositivos,Fallecidos,NuevosFallecidos,UCI,NuevosUCI,Recuperados,NuevosRecuperados,Muestras,Activos\n";
	archivo1<<"CR";
	for(int i=0; i<10; i++){
		archivo1<<","<<vect[i];
	}
	archivo1<<"\n";
	archivo1.close();	
}

/*
	Metodo verificarFecha() verifica la fecha de un archivo para revisar si ya se realizo una consulta hoy al servidor
	Param:
	Return: Devuelve 0 si no se ha realizado o un 1 si ya se realizo la consulta
*/
bool HTTPRequest::verificarFecha(){
	bool resp = false;
	char *line;
	char * token;
	char *saveptr;
	size_t tam=1024;
	line = (char *)calloc( 1024, 1 );
	int chars=0;

	FILE* archivo1;
	archivo1 = fopen("../BaseDatos/Actualizado.csv", "r");
	chars = getline( &line, &tam, archivo1);
	fclose(archivo1);
	if(line == generarNombre()){
		resp = true;
	}
	return resp;
}

/*
	Metodo parser() parsea los datos que se obtuvieron del servidor y los guarda en los vectores
	Param: vector<string> vector, char* nombre
	Return: el vector con los datos del archivo parseados
*/
vector<string> HTTPRequest::parser(vector<string> vector, char* nombre){
	FILE *archivo;
	char *line;
	char * token;
	char *saveptr;
	size_t tam=1024;
	line = (char *)calloc( 1024, 1 );
	int chars=0;
	char *last;

	archivo = fopen( nombre, "r" );
	if ( NULL == archivo ) {
		perror( "El archivo no se pudo abrir");
		exit( 2 );
   	}
   	
   	chars = getline( &line, &tam, archivo);
   	for(int i=0; i<82; i++){	
   	chars = getline( &line, &tam, archivo);			
	token = strtok_r( line, ",", &saveptr);
  		while ( NULL != token ) {
			last=token;
      			token = strtok_r( NULL, ",", &saveptr);
   		}
   		char* saveptr1;
   		token= strtok_r(last,"\r", &saveptr1 ); 		
  		vector[i]=last;
        }
    
    fclose(archivo);  	
	return vector;	    
}

/*
	Metodo parsearPais() parsea los datos que se obtuvieron del servidor y los guarda en el vector de pais
	Param: vector<string> vector
	Return: el vector con los datos del archivo pais parseados
*/
vector<string> HTTPRequest::parserPais(vector<string> vector){
	formatoDia();
	formatoMes();
	
	FILE *archivo;
	char *line;
	char * token;
	char *saveptr;
	size_t tam=1024;
	line = (char *)calloc( 1024, 1 );
	int chars=0;

	archivo = fopen( "../BaseDatos/General.csv", "r" );
	if ( NULL == archivo ) {
		perror( "El archivo no se pudo abrir");
		exit( 2 );
   	}
   	
   	chars = getline( &line, &tam, archivo);

   	do{	
		chars = getline( &line, &tam, archivo);			
		token = strtok_r( line, ",", &saveptr);
		
		char *saveptr1;
		token=strtok_r( token, "/", &saveptr1);
		
		if(token==dia){
			char* token2;
			token2=strtok_r( NULL, "/", &saveptr1);
			if(token2==mes){
				int cont=0;
				for(int i=0; i<130; i++){
					token=strtok_r( NULL, ",", &saveptr);					
					if(i==0||i==1||i==18||i==19||i==32||i==33||i==37||i==38||i==48||i==54){
						char* saveptr2;
						token= strtok_r(token,"\r", &saveptr2 );
						vector[cont]=token;
						cont++;
					}
				}
			}
		}
	}while(chars>0);

	fclose(archivo);  	
	return vector;	    
}

/*
	Metodo getGen() devuelve el numero de casos generales de los archivos .csv
	Param:
	Return: devuelve el numero de casos generales
*/
string HTTPRequest::getGen(){
	return gen;
}

/*
	Metodo getRecup() devuelve el numero de casos recuperados de los archivos .csv
	Param:
	Return: devuelve el numero de casos recuperados
*/
string HTTPRequest::getRecup(){
	return recup;
}

/*
	Metodo getActivos() devuelve el numero de casos activos de los archivos .csv
	Param:
	Return: devuelve el numero de casos activos
*/
string HTTPRequest::getActivos(){
	return activos;
}

/*
	Metodo getPositivos() devuelve el numero de casos positivos de los archivos .csv
	Param:
	Return: devuelve el numero de casos positivos
*/
string HTTPRequest::getPositivos(){
	return positivos;
}

/*
	Metodo getFallecidos() devuelve el numero de casos fallecidos de los archivos .csv
	Param:
	Return: devuelve el numero de casos falecidos
*/
string HTTPRequest::getFallecidos(){
	return fallecidos;
}
