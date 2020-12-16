#include "syscall.h"

void main() {
	int id;
	char a[300];
	char respuestaUsuario[200];
	char* imprimirEncabezadoMenuCliente = "--------------------------------------------------------------------------\nHola! Soy el Cliente!\nPuedo realizar solicitudes al Servidor Intermedio!\n\nPara realizar una solicitud de un Pais tienes que indicar:\n\tNombre-Pais\nPara realizar una solicitud de un Canton tienes que indicar:\n\tNombre-Canton\n--------------------------------------------------------------------------\n\n";
	char* imprimirMenuCliente = "--------------------------------------------------------------------------\nElija alguna de las opciones del menu\n1. Consultar un País\n2. Consultar un Cantón\n3. Salir del Programa\n--------------------------------------------------------------------------\n\n";
	Write(imprimirEncabezadoMenuCliente, 386, 1);
	

	char resp;

	 
	 

	 do {
	 	id = Socket( AF_INET_NachOS, SOCK_STREAM_NachOS );
	 	Connect( id, "127.0.0.1", 8080 );
		char respuestaUsuario[200];
		Write(imprimirMenuCliente, 263, 1);

		Read(respuestaUsuario, 200, 0);
		resp = respuestaUsuario[0];
		int i = 0, j = 0, chars = 0;
		int x = 0;
		int p = 0;
		int tam= 0;
		char* casoPais = "\nPor favor, digite el nombre del País a consultar:\n\n";
		char* casoCanton = "\nPor favor, digite el nombre del Cantón a consultar:\n\n";
		char sinFormato[200];
		char* formateado;
		char* formatoPais1 = "CON /";
		char* formato2 = " \n";
		char* formatoCanton = "CON /Costa-Rica/";
		switch (resp) {
			case '1':
			{
			Write(casoPais, 55, 1);
			x = Read(sinFormato, 200, 0);

			for (i = 0;i < 5;i++) {
				formateado[i] = formatoPais1[i];
				}
			for (j = 0;j < x;j++) {
				formateado[i] = sinFormato[j]; 
				i++;
				}

			for (p = 0;p< 2;p++) {
				formateado[i] = formato2[p];
				i++;
				}

			Write(formateado, i, id);
		 	chars = Read( a, 200, id );
		 	while(chars >0){
			 	Write(a, 200, 1);
			 	chars = Read( a, 200, id );	
		 	}
		 	
		 	Close(id);
			break;
			}

			case '2':
			{
			Write(casoCanton, 57, 1);
			x = Read(sinFormato, 200, 0);

			for (i = 0;i < 16;i++) {
				formateado[i] = formatoCanton[i];
				}
			for (j = 0;j < x;j++) {
				formateado[i] = sinFormato[j]; 
				i++;
				}

			for (p = 0;p< 2;p++) {
				formateado[i] = formato2[p];
				i++;
				}

			Write(formateado, i, id);
		 	chars = Read( a, 200, id );
		 	while(chars >0){
			 	Write(a, 200, 1);
			 	chars = Read( a, 200, id );	
		 	}
		 	
		 	Close(id);
			break;

			}

			case '3':
			{
			char* casoSalir = "\nSe salió del programa.\n\n";
			Write(casoSalir, 27, 1);
			break;
			}


			default:
			{
			char* casoDefault = "\nPor favor digite un dato valido.\n\n";
			Write(casoDefault, 38, 1);
			}
			}


		} while (resp != '3');

		Close( id );
		Exit(0);
	}