/*///////////////////////////////////////////////////////

    Clase HTTPRequest.h

    -Define los metodos y los atributos correspondientes de la clase
    HTTPRequest.cc

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../include/Socket.h"

#ifndef HTTPRequest_h
#define HTTPRequest_h

using namespace std;

class HTTPRequest {
    public:
        string sourceCodeErrors(int);
        int requestSourceCode();
        void generarStrings ();
        string generarNombre();
        void requestData(string, char*);
        void actualizar(char*, vector<string> ,vector<string>,vector<string>,vector<string>);
        void actualizarPais(vector<string>);
        bool verificarFecha();
        vector<string> parser(vector<string> vector, char* nombre);
        vector<string> parserPais(vector<string> vector);

        void formatoMes();
        void formatoDia();

        string getGen();
        string getRecup();
        string getActivos();
        string getPositivos();
        string getFallecidos();

    private:
        //Atributos para manejar el dia actualizado.
        string dia;
        string mes;

        string gen;
        string recup;
        string activos;
        string positivos;
        string fallecidos;

        time_t now = time(0);
	    tm *ltm = localtime(&now);
};

#endif