/*///////////////////////////////////////////////////////

    Clase MainServidor.cc

	-Es la clase MainServidor del programa.
	-Maneja los menu que se le desplegaran al usuario para la interaccion
	con el mismo.
	-Despliega los resultados tabulares que el usuario solicite.

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include "../include/Socket.h"
#include "../include/Servidor.h"

using namespace std;

/*
	Metodo main() ejecuta el programa
	Param: 
	Return: retorna 0 en el caso de que el programa se ejecute de manera correcta
*/
int main()
{
	Servidor s;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "Hola! Soy el Servidor Intermedio!" << endl;
	cout << "Me encuentro escuchando solicitudes atraves del puerto 8080" << endl << endl;
	cout << "Puedes realizar solicitudes usando el siguiente formato:" << endl;
	cout << "  " << "Para solicitar un pais tienes que indicar: " << endl;
	cout << "    " << "localhost:8080/ + Nombre-Pais" << endl;
	cout << "  " << "Para solicitar un canton tienes que indicar: " << endl;
	cout << "    " << "localhost:8080/ + Costa-Rica + / + Nombre-Canton" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	s.atenderSolicitudes();
	return 0;
}
