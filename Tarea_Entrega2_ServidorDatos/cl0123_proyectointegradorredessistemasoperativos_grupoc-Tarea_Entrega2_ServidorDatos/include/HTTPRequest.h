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
#include "../include/Fecha.h"
#include "../include/Parser.h"

#ifndef HTTPRequest_h
#define HTTPRequest_h

using namespace std;

class HTTPRequest
{
public:
    HTTPRequest();
    ~HTTPRequest();
    string sourceCodeErrors(int);
    void solicitarDatos();
    int requestSourceCode();
    void requestData(string, char *);
    bool verificarFecha();
    void imprimirProvincia(string);
    void imprimirPais();
    int checkArray(vector<int> arr);

private:
    Parser *parser;
    Fecha *fecha;

    vector<string> activos;
    vector<string> fallecidos;
    vector<string> positivos;
    vector<string> recuperados;
    vector<string> general;
};

#endif
