/*///////////////////////////////////////////////////////

    Clase HTTPSolicitud.h

    -Define los metodos y los atributos correspondientes de 
    la clase HTTPSolicitud.cc

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../include/Socket.h"
#include "../include/Fecha.h"
#include "../include/Parser.h"

#ifndef HTTPSolicitud_h
#define HTTPSolicitud_h

using namespace std;

class HTTPSolicitud
{
public:
    HTTPSolicitud();
    ~HTTPSolicitud();
    void solicitarDatos();
    int solicitarCodigo();
    void solicitarDatosServidor(string, char *);
    bool verificarFecha();
    int solicitarCodigoPaises(string);
    void solicitarDatosPaises(string);
    void actualizarPlantillaPais(string);
    void actualizarPlantillaCanton(string);
    bool verificarFechaPais();
    bool verificarNombrePais(string);
    string verificarNombreCanton(string);
    bool verificarPaisCSV(string);
    string obtenerDatosSolicitud(string);
    char *getPais(string);
    char *getCanton(string);
    bool obtenerDatosSolicitudPais(string);
    bool obtenerDatosSolicitudCanton(string);

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
