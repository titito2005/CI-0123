/*///////////////////////////////////////////////////////

	Clase Tabla.cc

	-Esta clase implementa el uso de una tabla para
	el manejo de las regiones + ip de los servidores
	de datos correspondientes.

/////////////////////////////////////////////////////////*/

#include "../include/Tabla.h"

/*
	Contructor de la clase Tabla()
*/
Tabla::Tabla(){
}

/*
	Destructor de la clase Tabla()
*/
Tabla::~Tabla(){
}

/*
	Metodo buscarIP() el cual verifica que exista la IP de la region en la tabla
	Param: string ip
	Return: Devuelve el 1 si la encuentra, 0 si no esta en la tabla
*/
bool Tabla::buscarIP(string ip){
  bool encontrado=false;
  for (vector<TablaDirecciones>::iterator it = tablaDirecciones.begin() ; it != tablaDirecciones.end(); ++it){
  	if(it->ipv4==ip){
  		encontrado=true;
  	}
  }
  return encontrado;
}

/*
	Metodo agregarDireccion() el cual agrega una ip + region en la tabla
	Param: TablaDirecciones direccion
	Return: void
*/
void Tabla::agregarDireccion(TablaDirecciones direccion){
	tablaDirecciones.push_back(direccion);
}

/*
	Metodo imprimir() el cual vimprime la tabla de direcciones
	Param: 
	Return: void
*/
void Tabla::imprimir(){
	for (vector<TablaDirecciones>::iterator it = tablaDirecciones.begin() ; it != tablaDirecciones.end(); ++it){
  		struct TablaDirecciones tabla = *it;
  		cout<<"Tabla:"<<tabla.ipv4<<"Region:"<<tabla.region<<endl;
  	}
}

/*
	Metodo buscarRegion() el cual verifica que exista la IP de la region en la tabla
	Param: string region
	Return: Devuelve el ip, 0 si no esta en la tabla
*/
string Tabla::buscarRegion(string region) {
	string ip = "0";
	for (vector<TablaDirecciones>::iterator it = tablaDirecciones.begin() ; it != tablaDirecciones.end(); ++it){
  	if(it->region==region){
  		ip = it->ipv4;
  	}
  }
  
  return ip;
}
