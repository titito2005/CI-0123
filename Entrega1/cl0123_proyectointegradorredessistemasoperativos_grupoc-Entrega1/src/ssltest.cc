/*///////////////////////////////////////////////////////

    Clase ssltest.cc

	-Es la clase main del programa.
	-Maneja los menu que se le desplegaran al usuario para la interaccion
	con el mismo.
	-Despliega los resultados tabulares que el usuario solicite.

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stddef.h>
#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../include/Socket.h"
#include "../include/HTTPRequest.h"
using namespace std;

//Variables globales 
HTTPRequest request;
time_t now = time(0);
tm *ltm = localtime(&now);
void menuFiltro();
void filtradoProvincia();
void imprimirProv(string);
void solicitarDatos();
void imprimirPais();
int checkArray(vector<int>);

//Vectores auxiliares para guardar los datos
vector<string> acti;
vector<string> falle;
vector<string> posi;
vector<string> re;
vector<string> ge;

/*
	Metodo menuFiltro() despliega un menu con las opciones del programa
	Param:
	Return: void
*/
void menuFiltro(){
 	int opcion;
	bool result = request.verificarFecha();
	cout << "Elija alguna de las opciones del menu\n";
	cout <<"1. Ver opciones de Provincias\n 2. Ver casos generales del pais\n 3. Salir del programa\n";
	cin >> opcion;
	switch(opcion){
		case 1:
			if (result == false) {
				int code = request.requestSourceCode();
				if (code == 200) {
					solicitarDatos();
				} else {
					request.sourceCodeErrors(code);
				}
			}
			filtradoProvincia();
		break;
		
		case 2: 
			if (result == false) {
				int code = request.requestSourceCode();
				if (code == 200) {
					solicitarDatos();
				} else {
					request.sourceCodeErrors(code);
				}
			}
			imprimirPais();
		break;

		case 3:
			cout<< "Se salió del programa\n";
		    exit(0); 
		break;

		default: 
			cout<< "Ingresó una opción inválida\n";
			menuFiltro();
	}
}

/*
	Metodo solicitarDatos() solicita los datos al servidor
	Param:
	Return: void
*/
void solicitarDatos() {
	acti.resize(82);
	falle.resize(82);
	posi.resize(82);
	re.resize(82);
	ge.resize(10);
	request.generarStrings();
	//REQUESTS
	request.requestData(request.getRecup(),"../BaseDatos/Recuperados.csv");
	request.requestData(request.getActivos(),"../BaseDatos/Activos.csv");
	request.requestData(request.getFallecidos(),"../BaseDatos/Fallecidos.csv");
	request.requestData(request.getGen(),"../BaseDatos/General.csv");
	request.requestData(request.getPositivos(),"../BaseDatos/Positivos.csv");
	//PARSERS
	acti = request.parser(acti, "../BaseDatos/Activos.csv");
	falle = request.parser(acti, "../BaseDatos/Fallecidos.csv");
	posi = request.parser(acti, "../BaseDatos/Positivos.csv");
	re = request.parser(acti, "../BaseDatos/Recuperados.csv");
	ge = request.parserPais(ge);
	//ACTUALIZA
	request.actualizarPais(ge);
	request.actualizar("../BaseDatos/Activos.csv", acti, posi, falle, re);
}

/*
	Metodo filtradoProvincia() despliega un menu con las provincias disponibles
	Param:
	Return: void
*/
void filtradoProvincia(){
	int opcion;
	cout << "Elija una de las provincias\n 1. San José \n 2. Alajuela \n 3. Limón \n 4. Puntarenas \n 5. Guanacaste \n 6. Cartago \n 7. Heredia\n 8. Volver al menú principal \n 9. Salir del programa\n";
	cin >> opcion; 
	switch (opcion){
		case 1:
			imprimirProv("San José");
		break;

		case 2:
			imprimirProv("Alajuela");
		break;
		case 3:
			imprimirProv("Limón");
		break;
		case 4:
			imprimirProv("Puntarenas");
		break;

		case 5:
			imprimirProv("Guanacaste");
		break;

		case 6:
			imprimirProv("Cartago");
		break;

		case 7:
			imprimirProv("Heredia");
		break; 

		case 8:
			menuFiltro();
		break;
		case 9:
			cout << "Se sale del programa\n";
			exit(0);
		break;

		default:
			cout<< "Ingresó una opción inválida\n";
			filtradoProvincia();
	}
}

/*
	Metodo checkArray() es para revisar si un substring posee n letras que coinciden con el nombre de una provincia
	Param: vector<int> arr
	Return: 1 para true o 0 para falso
*/
int checkArray(vector<int> arr){
	int n = 0;
	int contador = 0;
	for(int i = 0; i < arr.size(); ++i){
		if(arr[i] != 0){
			contador++;
		}
	}
	if (contador == arr.size()){
		n = 1;
	}
	return n;
}

/*
	Metodo imprimirProv() imprime por consola los resultados por provincia de manera tabular
	Param: string prov
	Return: void
*/
void imprimirProv(string prov){
	int posi=0;
	int activos=0;
	int recuperados=0;
	int fallecidos=0;
	
	FILE *archivo;
	char *line;
	char * token;
	char *saveptr;
	size_t tam=1024;
	line = (char *)calloc( 1024, 1 );
	int chars = 0;
	char *last;

	archivo = fopen( "../BaseDatos/Actualizado.csv", "r" );
	if ( NULL == archivo ) {
		perror( "El archivo no se pudo abrir");
		exit( 2 );
	}
	printf("%-20s%-20s%-20s%-20s%-20s%-20s\n\n", "Provincia", "Cantón", "Positivos", "Activos", "Recuperados", "Fallecidos"); 
	chars = getline( &line, &tam, archivo);	
	chars = getline( &line, &tam, archivo);	
	while(chars > 0){
		vector<int> booleanos = {0,0,0};
		chars = getline( &line, &tam, archivo);			
		token = strtok_r( line, ",", &saveptr);
		string provSubStr = prov.substr(0,3);
		for(int i = 0; i < provSubStr.size(); ++i){
			if(provSubStr[i] == token[i]){
				booleanos[i] = 1;
			}
		} 
		int entra = checkArray(booleanos);
		if(prov == token || entra){
			printf("%-20s", token);
			int cont=0;
			while ( NULL != token ) {
				token = strtok_r( NULL, ",", &saveptr);
				if (token != NULL){
					if (cont==1){
						posi+= stoi(token);
					}
					if (cont==2){
						activos+= stoi(token);
					}
					if (cont==3){
						recuperados+= stoi(token);
					}
					if (cont==4){
						fallecidos+= stoi(token);
					}
					
					printf("%-20s", token);
				}
			cont++;
			}
			cout << "\n";
		} 
	};
	printf("%-20s%-20s%-20d%-20d%-20d%-20d\n\n", "Total", " ", posi, activos, recuperados, fallecidos); 
	cout << "\n";
	menuFiltro();
}

/*
	Metodo imprimirPais() imprime por consola los resultados por pais de manera tabular
	Param:
	Return: void
*/
void imprimirPais(){
	FILE *archivo;
	char *line;
	char * token;
	char *saveptr;
	size_t tam=1024;
	line = (char *)calloc( 1024, 1 );
	int chars = 0;
	char *last;

	archivo = fopen( "../BaseDatos/Paises.csv", "r" );
	if ( NULL == archivo ) {
		perror( "El archivo no se pudo abrir");
		exit( 2 );
	}
	printf("%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s\n\n","País","Posi","+Posi","Falle","+Falle","UCI","+UCI","Recup","+Recup","Muestras","Activos"); 

	chars = getline( &line, &tam, archivo);	
	chars = getline( &line, &tam, archivo);	
	chars = getline( &line, &tam, archivo);		
	token = strtok_r( line, ",", &saveptr);
	printf("%-12s", token);
		while ( NULL != token ) {
			token = strtok_r( NULL, ", \r", &saveptr);
			if (token != NULL){
				printf("%-12s", token);
			}
		}
		cout << "\n";
		menuFiltro();
}

/*
	Metodo main() ejecuta el programa
	Param: int argc, char * argv[]
	Return: retorna 0 en el caso de que el programa se ejecute de manera correcta
*/
int main( int argc, char * argv[] ) {	
	menuFiltro();
	return 0;
}

 
 
 
