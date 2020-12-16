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
	s.atenderSolicitudes();
	return 0;
}
