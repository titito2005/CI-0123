#include <stdio.h>
#include <string>
#include <iostream>
#include "../include/Socket.h"
#include "../include/Servidor.h"

using namespace std;

int main()
{
	Servidor s;
	s.atenderSolicitudes();
	return 0;
}
