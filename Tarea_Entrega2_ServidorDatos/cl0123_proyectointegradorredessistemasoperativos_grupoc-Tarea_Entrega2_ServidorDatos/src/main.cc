/*///////////////////////////////////////////////////////

    Clase ssltest.cc

	-Es la clase main del programa.
	-Maneja los menu que se le desplegaran al usuario para la interaccion
	con el mismo.
	-Despliega los resultados tabulares que el usuario solicite.

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include "../include/Socket.h"
#include "../include/HTTPRequest.h"
using namespace std; //usando espacio de nombres

/*
	Metodo main() ejecuta el programa
	Param: int argc, char * argv[]
	Return: retorna 0 en el caso de que el programa se ejecute de manera correcta
*/
int main(int argc, char *argv[])
{
	HTTPRequest request;
	int opcion = 0;
	do
	{
		bool result = request.verificarFecha();
		cout << "Elija alguna de las opciones del menu\n";
		cout << "1. Ver opciones de Provincias\n 2. Ver casos generales del pais\n 3. Salir del programa\n";
		cin.clear();
		cin >> opcion;
		switch (opcion)
		{
		case 1:
		{
			if (result == false)
			{
				int code = request.requestSourceCode();
				if (code == 200)
				{
					request.solicitarDatos();
				}
				else
				{
					request.sourceCodeErrors(code);
				}
			}

			int opcion2 = 0;
			do
			{
				cout << "Elija una de las provincias\n 1. San José \n 2. Alajuela \n 3. Cartago \n 4. Heredia \n 5. Guanacaste \n 6. Puntarenas \n 7. Limón\n 8. Volver al menú principal \n";
				cin.clear();
				cin >> opcion2;
				switch (opcion2)
				{
				case 1:
					request.imprimirProvincia("San José");
					break;

				case 2:
					request.imprimirProvincia("Alajuela");
					break;
				case 3:
					request.imprimirProvincia("Cartago");
					break;
				case 4:
					request.imprimirProvincia("Heredia");
					break;

				case 5:
					request.imprimirProvincia("Guanacaste");
					break;

				case 6:
					request.imprimirProvincia("Puntarenas");
					break;

				case 7:
					request.imprimirProvincia("Limón");
					break;

				case 8:
					cout << "Volviendo al menú principal" << endl;
					break;

				default:
					cout << "Ingresó una opción inválida" << endl;
				}

			} while (opcion2 != 8);
		}
		break;

		case 2:
		{
			if (result == false)
			{
				int code = request.requestSourceCode();
				if (code == 200)
				{
					request.solicitarDatos();
				}
				else
				{
					request.sourceCodeErrors(code);
				}
			}

			request.imprimirPais();
		}
		break;

		case 3:
		{
			cout << "Se salió del programa\n";
		}
		break;

		default:
			cout << "Ingresó una opción inválida\n";
		}
	} while (opcion != 3);

	return 0;
}
