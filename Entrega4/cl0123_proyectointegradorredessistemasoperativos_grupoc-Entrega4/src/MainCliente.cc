/*///////////////////////////////////////////////////////

	Clase MainCliente.cc

	-Es la clase main del programa.
	-Maneja los menu que se le desplegaran al usuario para la interaccion
	con el mismo.
	-Despliega los resultados tabulares que el usuario solicite.

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include "../include/Socket.h"
#include "../include/HTTPSolicitud.h"

using namespace std;

/*
	Metodo imprimirRespuesta() imprime por consola la respuesta de la solicitud de canton
	Param: int tipo, char* param
	Return: void
*/
void imprimirRespuesta(int tipo, char* param) {
	char temp[2048];
	strcpy(temp, param);
	char* token;
	char* saveptr;
	if (tipo == 1) {
		printf("%-25s%-15s%-15s%-17s%-10s%-17s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%s\n\n", "Pais", "Total Casos", "+ Casos",
			"Total Muertes", "Muertes", "Total Recup", "Recuperados", "Activos", "Críticos", "Casos/mill", "Muertes/mill",
			"Total tests", "Tests/mill", "Población");

		token = strtok_r(temp, ";", &saveptr);

		for (int cont = 0; cont < 14; cont++) {

			if (token != NULL) {

				if (cont == 0) {
					printf("%-25s", token);
					}
				else if (cont == 3 || cont == 5) {
					printf("%-17s", token);
					}
				else if (cont == 4) {
					printf("%-10s", token);
					}
				else if (cont == 13) {
					printf("%s", token);
					}
				else {
					printf("%-15s", token);
					}
				}
			token = strtok_r(NULL, ";", &saveptr);
			}
		cout << "\n\n\n";
		}
	else if (tipo == 2) {
		printf("%-10s%-17s%-15s%-25s%-15s%-15s%-15s%-15s\n\n", "Código", "Provincia", "Código", "Cantón", "Positivos",
			"Activos", "Recuperados", "Fallecidos");

		token = strtok_r(temp, ";", &saveptr);
		for (int cont = 0; cont < 8; cont++) {

			if (token != NULL) {

				if (cont == 0) {
					printf("%-10s", token);
					}
				else if (cont == 1) {
					printf("%-17s", token);
					}
				else if (cont == 3) {
					printf("%-25s", token);
					}
				else if (cont == 7) {
					printf("%s", token);
					}
				else {
					printf("%-15s", token);
					}
				}
			token = strtok_r(NULL, ";", &saveptr);
			}
		cout << "\n\n\n";
		}
	else if (tipo == 3) {
		char* token;
		char* saveptr;
		token = strtok_r(temp, ",", &saveptr);
		cout << token << "\n\n" << endl;
		}
	}

/*
	Metodo imprimirEncabezadoMenuCliente() imprime el encabezado incial del menu del cliente
	Param:
	Return: void
*/
void imprimirEncabezadoMenuCliente() {
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "Hola! Soy el Cliente!" << endl;
	cout << "Puedo realizar solicitudes al Servidor Intermedio!" << endl << endl;
	cout << "Para realizar una solicitud de un Pais tienes que indicar:" << endl;
	cout << "  " << "nombre-región" << endl;
	cout << "  " << "Nombre-país" << endl;
	cout << "Para realizar una solicitud de un Canton tienes que indicar:" << endl;
	cout << "  " << "Nombre-Cantón" << endl;
	cout << "--------------------------------------------------------------------------" << endl << endl;
	}

/*
	Metodo imprimirMenuCliente() imprime el menu del cliente
	Param:
	Return: void
*/
void imprimirMenuCliente() {
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "Elija alguna de las opciones del menu" << endl;
	cout << "1. Consultar un País " << endl;
	cout << "2. Consultar un Cantón" << endl;
	cout << "3. Salir del Programa" << endl;
	cout << "--------------------------------------------------------------------------" << "\n\n";
	}

/*
	Metodo main() ejecuta el programa
	Param: int argc, char * argv[]
	Return: retorna 0 en el caso de que el programa se ejecute de manera correcta
*/
int main(int argc, char* argv[]) {
	HTTPSolicitud solicitud;
	imprimirEncabezadoMenuCliente();
	int opcion = 0;
	do {
		bool result = solicitud.verificarFecha();
		imprimirMenuCliente();
		cin >> opcion;
		cout << "\n";
		switch (opcion) {
			case 1:
			{
			Socket* s = new Socket(true, false);
			char buffer[2048];
			memset(buffer, 0, 2048);
			char valorIngresado[200] = { 0 };
			char regionIngresada[200] = { 0 };
			s->Connect("172.16.123.51", 8080);

			cout << "Por favor, digite el nombre de la región a consultar:\n\n";
			cin.ignore();
			cout << "\n";
			cin.getline(regionIngresada, 200);
			cout << "Por favor, digite el nombre del país a consultar:\n\n";
			cin.getline(valorIngresado, 200);
			cout << "\n";

			string regionSinFormato(regionIngresada);
			string sinFormato(valorIngresado);

			string formatoConsulta = "/region=" + regionSinFormato + "&pais=" + sinFormato;
			char pais[formatoConsulta.length()];
			strcpy(pais, formatoConsulta.c_str());
			int tam = sizeof(pais);
			s->Write(pais, tam);
			s->Read(buffer, 2048);
			int i = 0;
			cout << buffer << endl;
			memset(buffer, 0, 2048);
			s->Close();
			}
			break;

			case 2:
			{
			Socket* s = new Socket(true, false);
			char buffer[2048];
			memset(buffer, 0, 2048);
			char valorIngresado[200] = { 0 };
			s->Connect("172.16.123.51", 8080);

			cout << "Por favor, digite el nombre del Cantón a consultar:\n\n";
			cin.ignore();
			cin.getline(valorIngresado, 200);
			cout << "\n";
			string sinFormato(valorIngresado);
			string formatoConsulta = "country=Costa-Rica&canton=" + sinFormato;
			char canton[formatoConsulta.length()];
			strcpy(canton, formatoConsulta.c_str());
			int tam = sizeof(canton);
			s->Write(canton, tam);
			s->Read(buffer, 2048);
			int i = 0;
			cout << buffer << endl;
			memset(buffer, 0, 2048);
			s->Close();
			}
			break;

			case 4:
			{

			cout << "Se salió del programa.\n";
			}
			break;
			}
		} while (opcion != 3);


		return 0;
	}

