/*
Luis Eduardo Rojas Carrillo - B86875.
MIRRORCLIENTTEST.C
MAIN PARA EJECUTAR CLIENTE.
*/
#include "../include/Socket.h"
#include <stdio.h>

int main(int argc, char **argv)
{
   Socket s(true, false); // Crea un socket de IPv4, tipo "stream".
   char buffer[512];      //Buffer para recibir.

   s.Connect("10.0.2.15", 8080); // Same port as server.
   s.Write("Hola", 512);         // Send first program argument to server.
   s.Read(buffer, 512);          // Read the answer sent back from server.
   printf("%s \n", buffer);      // Print the string.
}
