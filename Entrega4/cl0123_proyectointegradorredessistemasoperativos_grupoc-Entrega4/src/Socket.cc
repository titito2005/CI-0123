/*///////////////////////////////////////////////////////

    Clase Socket.cc

    -Efectua la comunicacion con el servidor.
    -Mediante una solicitud, permite leer y escribir, esto 
    con el fin de obtener datos provenientes de un servidor 
    a partir de una solicitud.  

/////////////////////////////////////////////////////////*/

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../include/Socket.h"

/* 
  Constructor de la clase Socket
  Param: int id
  Return: 
 */
Socket::Socket(int id)
{
  this->id = id;
}

/*
  Contructor de la clase Socket
  Param: bool tipo (true->"stream", false->"datagram"), bool ipv6 (Socket ipv6 o no)
  Return:
*/
Socket::Socket(bool tipo, bool ipv6)
{
  if (ipv6)
  { //IPv6
    if (!tipo)
    {
      id = socket(AF_INET6, SOCK_DGRAM, 0);
    }
    else
    {
      id = socket(AF_INET6, SOCK_STREAM, 0);
    }
  }
  else
  { //IPv4
    if (!tipo)
    {
      id = socket(AF_INET, SOCK_DGRAM, 0);
    }
    else
    {
      id = socket(AF_INET, SOCK_STREAM, 0);
    }
  }

  if (-1 == (id))
  {
    printf("Hubo un error en el constructor principal");
    exit(0);
  }

  this->SSLContext = nullptr;
  this->SSLStruct = nullptr;
  int on = 1;
  int st = setsockopt(id, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on));
  if (-1 == st)
  {
    perror("Socket::Socket, set socket options");
    exit(2);
  }
}

/*
  Destructor de la clase Socket
  Param:
  Return:
*/
Socket::~Socket()
{
  Close();

  // SSL destroy
  if (nullptr != this->SSLContext)
  {
    SSL_CTX_free((SSL_CTX *)this->SSLContext);
  }
  if (nullptr != this->SSLStruct)
  {
    SSL_free((SSL *)this->SSLStruct);
  }
}

/*
  Metodo Close() se encarga de cerrar el socket
  Param:
  Return: void
*/
void Socket::Close()
{
  int resp;
  resp = close(id);

  if (-1 == resp)
  {
    printf("Error en el destructor");
    exit(0);
  }
}

/*
  Metodo Connect() realiza una conexion con el servidor (IPv4)
  Param: const char* hostip(direccion del servidor), int port (Numero del puerto)
  Return: Devuelve -1 si da error la conexion 
 */
int Socket::Connect(const char *hostip, int port)
{
  int resp;
  int largo;
  struct sockaddr_in servidor;
  servidor.sin_family = AF_INET;
  inet_aton(hostip, &servidor.sin_addr);
  servidor.sin_port = htons(port);
  largo = sizeof(servidor);
  resp = connect(id, (struct sockaddr *)&servidor, largo);

  if (-1 == resp)
  {
    printf("Hubo un error en el connect");
    Close();
    exit(0);
  }

  return resp;
}

/*
  Metodo Connect() realiza una conexion con el servidor (IPv6)
  Param: const char* hostip(direccion del servidor), const char* service (Servicio ej HTTP)
  Return: Devuelve -1 si da error la conexion 
 */
int Socket::Connect(const char *host, const char *service)
{
  size_t len;
  int resp;
  struct addrinfo hints, *result, *rp;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0; /* Any protocol */

  resp = getaddrinfo(host, service, &hints, &result);
  for (rp = result; rp; rp = rp->ai_next)
  {
    resp = connect(id, rp->ai_addr, rp->ai_addrlen);
    if (-1 == resp)
    {
      perror("Error conectando");
      Close();
      exit(0);
    }
  }

  freeaddrinfo(result);
  return resp;
}

/*
  Metodo Read() se encarga de recibir la informacion del servidor
  Param: char* text, int len
  Return: Devuelve -1 si da error 
*/
int Socket::Read(char *text, int len)
{
  int resp;

  resp = recv(id, (void *)text, len,0);
  if (-1 == resp)
  {
    printf("Hubo un error en el read");
    exit(0);
  }

  return resp;
}

/*
  Metodo Write() se encarga de solicitar la informacion del servidor
  Param: char* text, int len
  Return: Devuelve -1 si da error  
*/
int Socket::Write(char *text, int len)
{
  int resp;

  resp = write(id, (const void *)text, len);
  if (-1 == resp)
  {
    printf("Hubo un error en el write \n");
    Close();
    exit(0);
  }

  return resp;
}

/*
  Metodo Listen() obtiene una respuesta de conexion con el servidor
  Param: int queue
  Return: Devuelve -1 si da error 
*/
int Socket::Listen(int queue)
{
  int resp;

  resp = listen(id, queue);
  if (-1 == resp)
  {
    printf("Hubo un error en el listen \n");
    Close();
    exit(0);
  }

  return resp;
}

/*
  Metodo Bind() asocia el socket con el puerto indicado
  Param: int port(puerto)
  Return: Devuelve -1 si da error  
 */
int Socket::Bind(int port)
{
  int resp;
  int len;
  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);
  len = sizeof(server_addr);

  resp = bind(id, (const sockaddr *)&server_addr, len);
  if (resp == -1)
  {
    printf("Error en el bind \n");
    Close();
    exit(0);
  }

  return resp;
}

/*
  Metodo Accept() acepta las conexiones desde el cliente
  Param:
  Return: Devuelve una nueva instancia de la clase Socket para manejar la conexion de un cliente
 */
Socket *Socket::Accept()
{
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  addr_size = sizeof(their_addr);
  int resp;

  resp = accept(id, (struct sockaddr *)&their_addr, &addr_size);
  if (-1 == resp)
  {
    printf("Error en el accept");
    Close();
    exit(0);
  }
  Socket *respSocket = new Socket(resp);

  return respSocket;
}

/*
  Metodo ShutDown() cierra parcialmente la conectividad del socket
  Param: int mode (Tipo de cierre)
  Return: Devuelve -1 si da error  
 */
int Socket::Shutdown(int mode)
{
  int resp;

  resp = shutdown(id, mode);
  if (-1 == resp)
  {
    printf("Error apagando servidor \n");
    Close();
    exit(0);
  }

  return resp;
}

/*
  Metodo SetIDSocket() modifica la variable de la instancia
  Param: int id
  Return: void
 */
void Socket::SetIDSocket(int id)
{
  this->id = id;
}

/*   SSL Methods Section   */

/*
  Metodo InitSSLContext() inicializa una solicitud SSL
  Param:
  Return: void
 */
void Socket::InitSSLContext()
{
  const SSL_METHOD *method = TLS_client_method();

  SSL_CTX *context = SSL_CTX_new(method);
  if (nullptr == context)
  {
    perror("Socket::InitSSLContext");
    exit(23);
  }

  this->SSLContext = (void *)context;
}

/*
  Metodo InitSSL() inicializa una solicitud SSL
  Param:
  Return: void
 */
void Socket::InitSSL()
{
  this->InitSSLContext();

  SSL *ssl = SSL_new(((SSL_CTX *)this->SSLContext));
  if (nullptr == ssl)
  {
    perror("Socket::InitSSL");
    exit(23);
  }

  this->SSLStruct = (void *)ssl;
}

/*
  Metodo SSLConnect() realiza una conexion con el servidor 
  Param: const char* hostip(direccion del servidor), int port (Numero del puerto)
  Return: void
 */
void Socket::SSLConnect(char *host, int port)
{
  int resultado;

  this->Connect(host, port);
  SSL_set_fd((SSL *)this->SSLStruct, this->id);
  resultado = SSL_connect((SSL *)this->SSLStruct);
  if (-1 == resultado)
  {
    perror("Socket::SSLConnect");
    exit(23);
  }
}

/*
  Metodo SSLConnect() realiza una conexion con el servidor 
  Param: const char* hostip(direccion del servidor), const char* service (Servicio ej HTTP)
  Return: void 
 */
void Socket::SSLConnect(char *host, char *service)
{
  int resultado;

  this->Connect(host, service);
  SSL_set_fd((SSL *)this->SSLStruct, this->id);
  resultado = SSL_connect((SSL *)this->SSLStruct);
  if (-1 == resultado)
  {
    perror("Socket::SSLConnect");
    exit(23);
  }
}

/*
  Metodo SSLRead() recibe la solicitud del servidor y la almacena
  Param: void* buffer, int size
  Return: Devuelve -1 si da error 
 */
int Socket::SSLRead(void *buffer, int size)
{
  int resultado;

  resultado = SSL_read((SSL *)this->SSLStruct, buffer, size);
  if (-1 == resultado)
  {
    perror("Socket::SSLRead");
    exit(23);
  }

  return resultado;
}

/*
  Metodo SSLWrite() se encarga de solicitar la informacion del servidor
  Param: const void* buffer, int size
  Return: Devuelve -1 si da error  
*/
int Socket::SSLWrite(const void *buffer, int size)
{
  int resultado;

  resultado = SSL_write((SSL *)this->SSLStruct, buffer, size);
  if (-1 == resultado)
  {
    perror("Socket::SSLWrite");
    exit(23);
  }

  return resultado;
}

/*
  Metodo getIDSocket() devuelve el id asociado al socket
  Param: 
  Return: Devuelve el id del socket  
*/
int Socket::getIDSocket()
{
  return this->id;
}

/*
  Metodo sendTo() para enviar mensajes atraves de un socket mediante UDP
  Param: const void* buffer, int size, void *other
  Return: Devuelve -1 si da error  
*/
int Socket::sendTo( const void *buffer, int size, void *other){
  int resultado;
  socklen_t len= sizeof(struct sockaddr);
  resultado = sendto (id, (const char*)buffer, size, MSG_CONFIRM, (struct sockaddr*) other, len);
  if (-1 == resultado){
        perror("Socket::sendTo");
        exit(0);
    }
  return resultado;
}

/*
  Metodo recvFrom() para recibir mensajes atraves de un socket mediante UDP
  Param: const void* buffer, int size, void *other
  Return: Devuelve -1 si da error  
*/
int Socket::recvFrom( void *buffer, int size, void * other){
  int resultado;
  socklen_t len= sizeof(struct sockaddr);
  resultado= recvfrom (id, (char*)buffer, size, MSG_WAITALL, (struct sockaddr*)other, &len);
  if (-1 == resultado){
        perror("Socket::recvFrom");
        exit(0);
    }
  return resultado;
}

/*
  Metodo broadcast() el cual envia un broadcast atraves de un socket
  Param: char* msg, char* ip, int port
  Return: void
*/
void Socket::broadcast(char* msg, char* ip, int port){
  int broadcast = 1;
  char buffer[1024];
  struct sockaddr_in servaddr;

  setsockopt( this->id, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof( broadcast ) );

  memset( &servaddr, 0, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons( port );

  servaddr.sin_addr.s_addr = inet_addr(ip); 

  sendto(this->id, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
  printf("%s%s\n","Broadcast message sent to ", ip);

}
