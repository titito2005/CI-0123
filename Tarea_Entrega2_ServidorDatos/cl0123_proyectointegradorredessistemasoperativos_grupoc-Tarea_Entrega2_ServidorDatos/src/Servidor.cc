#include "../include/Servidor.h"
Servidor::Servidor()
{
	s1 = new Socket(true, false);
	thread_count = 10;
	s1->Bind(8080);
	s1->Listen(5);
}

void *hilo(void *param)
{
	char a[512];
	Socket *s3 = (Socket *)param;
	long leerSolicitud;
	string html = "";
	string line = "";
	int bytes;
	ifstream leerHtml ("Plantilla.html");
	char buffer[30000] = {0};
	leerSolicitud = read(s3->getIDSocket(), buffer, 30000);
	if(leerHtml.is_open()){
		while(getline(leerHtml, line)){
			html += line;
		}
		leerHtml.close();
	}
	const char * contenidoFinal = html.c_str();
	bytes = strlen(contenidoFinal);
	string bytesF = to_string(bytes);
	string respuestaInicial = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length:" + bytesF + "\n\n\n" + html;
	const char *respuestaServidor = respuestaInicial.c_str();
	printf("%s/n", buffer);
	write(s3->getIDSocket(), respuestaServidor, strlen(respuestaServidor));
	//s3->Read(a, 512 ); // Read a string from client
	//s3->Write(a, 512 );	// Write it back to client, this is the mirror function
	s3->Close();
	return NULL;
}

Servidor::~Servidor()
{
	s1->Shutdown(SHUT_RDWR);
	s1->Close();
	free(thread_handles);
}

void Servidor::atenderSolicitudes()
{
	thread_handles = (pthread_t *)calloc(thread_count, sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++)
	{
		s2 = s1->Accept();
		pthread_create(&thread_handles[thread], NULL, hilo, (void *)s2);
	}

	for (thread = 0; thread < thread_count; thread++)
	{
		pthread_join(thread_handles[thread], NULL);
	}
}
