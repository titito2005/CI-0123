/*///////////////////////////////////////////////////////

	Clase MainIntermedio.cc

	-Ejecuta el servidor intermedio para recibir las consultas
	del cliente y enviarselas al servidor de datos.

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include "../include/Socket.h"
#include "../include/ServidorIntermedio.h"

using namespace std;

/*
	Metodo main() del servidor intermedio
	Param:
	Return: Devuelve 0, -1 de lo contrario
*/
int main(){
	ServidorIntermedio intermedio;
	intermedio.crearHilos();
}
