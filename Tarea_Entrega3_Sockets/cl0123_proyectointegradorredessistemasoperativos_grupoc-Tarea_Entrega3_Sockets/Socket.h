/*///////////////////////////////////////////////////////

    Clase Socket.h

    -Define los metodos y los atributos correspondientes de la clase
    Socket.cc

/////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifndef Socket_h
#define Socket_h

class Socket
{

public:
  Socket(bool, bool = false);
  Socket(int);
  ~Socket();
  int Connect(const char *, int);
  int Connect(const char *, const char *);
  void Close();
  int Read(char *, int);
  int Write(char *, int);
  int Write(char *);
  int Listen(int);
  int Bind(int);
  Socket *Accept();
  int Shutdown(int);
  void SetIDSocket(int);
  int getIDSocket();
  int sendTo( const void *, int, void * );//Método nuevo
  int recvFrom( void *, int, void * );//Método nuevo

  // SSL public methods
  void InitSSL();
  void SSLConnect(char *, int);
  void SSLConnect(char *, char *);
  int SSLRead(void *, int);
  int SSLWrite(const void *, int);

private:
  int id;
  bool ipv6;
  int port;//VARIABLE NUEVA
  // SSL private methods
  void InitSSLContext();
  // SSL instance variables
  void *SSLContext;
  void *SSLStruct;
};
#endif
