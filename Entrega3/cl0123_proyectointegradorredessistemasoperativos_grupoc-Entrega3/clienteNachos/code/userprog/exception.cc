// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "synch.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void returnFromSystemCall() {

	int pc, npc;
	pc = machine->ReadRegister(PCReg);
	npc = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PrevPCReg, pc);		// PrevPC <- PC
	machine->WriteRegister(PCReg, npc);			// PC <- NextPC
	machine->WriteRegister(NextPCReg, npc + 4); // NextPC <- NextPC + 4

	} // returnFromSystemCall

	/*
	 *  System call interface: Halt()
	 */
void NachOS_Halt() { // System call 0

	DEBUG('a', "Shutdown, initiated by user program.\n");
	interrupt->Halt();
	}

/*
 *  System call interface: void Exit( int )
 */
// void NachOS_Exit() { // System call 1
// 	int status = machine->ReadRegister(4);
// 	IntStatus oldLevel = interrupt->SetLevel(IntOff);
// 	if (status == 0) {
// 		printf("Proceso Termino con %d \n", status);
// 		}
// 	printf("Terminando hilo: %s.\n", currentThread->getName());
// 	currentThread->Finish();
// 	returnFromSystemCall();
// 	}


void NachOS_Exit() {    //System call # 1

	int status = machine->ReadRegister (4);

	currentThread->Yield();

	currentThread->tabla->del_thread();
	currentThread->tablaSC->del_thread();

	if (0 == currentThread->tabla->getUsage()) {
		NachOS_Halt();  //soy el ultimo, apague y vamonos
	}
	if (NULL != currentThread->space) {
		delete currentThread->space;
		currentThread->space = NULL;
	}

	//tengo que liberar los semaforos que yo como proceso habia creado
	// for (int i = 0; i < CANTSEMS; ++i) {
	// 	if (currentThread->m_semaphoreTable->isOpened (i)) {
	// 		semVec[i]->V();
	// 		openSems->Clear (i);
	// 		currentThread->m_semaphoreTable->Close (i);
	// 		currentThread->m_semaphoreTable->delThread();
	// 	}
	// }

	currentThread->Finish();    //termina el proceso
	returnFromSystemCall();
	DEBUG ('a', "Exiting Exit");
}//Nachos_Exit


void execAux (void* executableName) {
	// Tomado de StartProcess en progtest.cc

	OpenFile* p = (OpenFile*) executableName;
	if (p == NULL) {
		printf ("Unable to open file");
		return;
	}

	currentThread->space = new AddrSpace (p); // Crea el espacio para el ejecutable

	currentThread->space->InitRegisters(); // set the initial register values
	currentThread->space->RestoreState();  // load page table register
	machine->Run(); // jump to the user progam
	ASSERT (false);			// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
}

/*
 *  System call interface: SpaceId Exec( char * )
 */
void NachOS_Exec() {    //System call # 2
	char bufferExec_Aux[1024];

	int direccion = machine->ReadRegister (4);
	int character = 1;
	for (int i = 0; character != '\0'; ++i) {
		machine->ReadMem (direccion + i, 1, &character);
		bufferExec_Aux[i] = (char) character;
	} 

	// Trata de abrir la ruta indicada
	OpenFile* executable = fileSystem->Open (bufferExec_Aux);
	if (executable == NULL) {
		printf ("Unable to open file %s\n", bufferExec_Aux);
		machine->WriteRegister (2, -1);
		return;
	}

	Thread* newThread = new Thread ("New Executable");    //nuevo proceso hijo

	newThread->Fork (execAux, (void*) executable);

	machine->WriteRegister (2, -1);  // esto está mal, hay que arregarlo.
	returnFromSystemCall();

} //Nachos_Exec



/* Only return once the the user program "id" has finished.
 * Return the exit status.
 *
 * int Join(SpaceId id);
 */
void NachOS_Join() {    //System call # 3
	// SpaceId sID = machine->ReadRegister (4);

	// Semaphore* semaforo = new Semaphore ("Semaforo join", 0);
	// //currentThread->m_semaphoreTable->semOpen (sID, (long) semaforo);
	// //currentThread->m_semaphoreTable->addThread();
	// if (currentThread->tablaSC->get_unix_handle (sID) == -1) {
	// 	machine->WriteRegister (2, -1);
	// }
	// else {
	// 	machine->WriteRegister (2, 0);
	// }
	// semaforo->P();
	// returnFromSystemCall();

}//Nachos_Join





/*
 *  System call interface: void Create( char * )
 */
void NachOS_Create() { // System call 4
	char name[128] = "";

	int c = 1;

	int direc = machine->ReadRegister(4);

	int it = 0;

	do {
		machine->ReadMem(direc, 1, &c);
		name[it++] = c;
		direc++;
		} while (c != 0);

		int id = creat((const char*)name, O_CREAT | S_IRWXU);
		close(id);
		machine->WriteRegister(2, 0);
		returnFromSystemCall();
	}

/*
 *  System call interface: OpenFileId Open( char * )
 */
void NachOS_Open() { // System call 5
	int resultado;

	int name = machine->ReadRegister(4);

	int car = -1;

	int index = 0;

	OpenFileId id = 0;

	char* filename = new char[128];

	while (0 != car) {

		machine->ReadMem(name, 1, &car);
		filename[index] = (char)car;
		name++;
		index++;
		}

	int unix_handle = open(filename, O_RDWR);
	if (-1 == unix_handle) {
		perror("-1 abriendo la vara");
		}
	else {
		id = currentThread->tabla->Open(unix_handle);
		}
	machine->WriteRegister(2, id);
	returnFromSystemCall();
	}

/*
 *  System call interface: OpenFileId Write( char *, int, OpenFileId )
 */
void NachOS_Write() {// System call 6
	bool seguir = true;
	char* buffer = NULL;
	int memPos = machine->ReadRegister(4);
	int size = machine->ReadRegister(5); // Lee el tamaño

	int* value = new int;

	buffer = new char[size + 1];

	for (int i = 0; i < size && seguir;++i) {
		machine->ReadMem(memPos++, 1, value);
		if((char)*value!='$'){
		buffer[i] = (char)*value;
		}else {
			seguir=false;
			}
		}

	// buffer = Data leida del usuario (nombre del archivo)
	OpenFileId id = machine->ReadRegister(6);

	int uId = 0;
	//uId = currentThread->tabla->get_unix_handle(id);
	// Semaforo para sincronizar el leer
	//Console->P();
	switch (id) {
		case ConsoleInput: // User could not write to standard input
			machine->WriteRegister(2, -1);
			break;
		case ConsoleOutput:

			buffer[size] = 0;
			printf("%s", buffer);
			stats->numConsoleCharsWritten += size;
			machine->WriteRegister(2, size);
			break;

		case ConsoleError: // This trick permits to write integers to console
			printf("%d\n", machine->ReadRegister(4));
			break;
		default: // All other opened files
			// Verify if the file is opened, if not return -1 in r2
			// Get the unix handle from our table for open files
			// Do the write to the already opened Unix file
			// Return the number of chars written to user, via r2
			uId = currentThread->tabla->get_unix_handle(id);
			if (uId != -1) {
				size = write(uId, buffer, size);
				++stats->numDiskWrites;
				machine->WriteRegister(2, size);
				}
			else {
				printf("Something went wrong when writing in the file.\n");
				machine->WriteRegister(2, -1);
				}
			break;
		}
	//Console->V();
	// Update simulation stats, see details in Statistics class in machine/stats.cc

	returnFromSystemCall(); // Update the PC registers
	}

/*
 *  System call interface: OpenFileId Read( char *, int, OpenFileId )
 */
// void NachOS_Read() { // System call 7
// 	char* buffer = NULL;
// 	int size = machine->ReadRegister(5); // Read size to write
// 	buffer = new char[size];

// 	OpenFileId id = machine->ReadRegister(6); // Read file descriptor
// 	int ide = currentThread->tabla->get_unix_handle(id);
// 	size = read(ide, buffer, size); //Ejecuta el read de unix basado el unixHandle que tiene el archivo en la tabla de archivos abiertos y lo guarda en buffer

// 	if (-1 != ide) { //Se ejecuta bien
// 		int memPos = machine->ReadRegister(4);
// 		for (int i = 0; i < size; ++i) { //Escribe en memoria (buffer) lo que leyo del archivo
// 			machine->WriteMem(memPos++, 1, buffer[i]);
// 			}

// 		machine->WriteRegister(2, size); //Se ejecuta bien
// 		}
// 	else {
// 		machine->WriteRegister(2, -1);
// 		}

// 	returnFromSystemCall(); // Update the PC register
// 	}

void NachOS_Read(){
	int address = machine->ReadRegister(4);
	int size = machine->ReadRegister(5);
	OpenFileId id = machine->ReadRegister(6);
	size_t charsRead;
	char *buffer;
	int wordBuffer;
	int unixHandle;
	buffer = new char[size];
	bool readingError = false;
	switch(id){
		case ConsoleInput:
			charsRead = read(id, buffer, size);
			stats->numConsoleCharsRead++;
			machine->WriteRegister(2, charsRead);
			break;
		case ConsoleOutput:
			machine->WriteRegister(2,-1);
			printf("Error: user can not read from output\n");
			readingError = true;
			break;
		case ConsoleError:
			printf("Error: user can not read from console error\n");
			break;
		default:
			if(currentThread->tabla->is_opened(id)){
				unixHandle = currentThread->tabla->get_unix_handle(id);
				charsRead = read(unixHandle,buffer,size);
				buffer[charsRead] = '\0';
				machine->WriteRegister(2,charsRead);
				stats->numDiskReads++;
			} else {
				machine->WriteRegister(2,-1);
			}
		break;
	}
	DEBUG('s', "Buffer: %s\n", buffer);
	if(!readingError){
		for(int i=0;i<charsRead;i++){
			wordBuffer = (int)buffer[i];
			machine->WriteMem(address, 1, wordBuffer);
			address++;
		}
	}

	delete buffer;
	returnFromSystemCall();
}

/*
 *  System call interface: void Close( OpenFileId )
 */
void NachOS_Close() { // System call 8
	//Cierra el archivo basado en su id que esta guardado en la tabla de archivos abiertos TAA
	OpenFileId id = machine->ReadRegister(4);
	long uId = currentThread->tabla->get_unix_handle(id);
	int bandera = close(uId);

	if (bandera == 0) {

		currentThread->tabla->Close(id);
		}
	else {
		printf("No se pudo cerrar el archivo: %d.\n", id);
		}

	machine->WriteRegister(2, bandera);

	returnFromSystemCall();
	}

/* Asistete del SysCall fork */
void forkAux (void* address) {     //Metodo auxiliar que usa Fork
	AddrSpace* space;

	space = currentThread->space;
	space->InitRegisters();
	space->RestoreState();

	// Set the return address for this thread to the same as the main thread
	// This will lead this thread to call the exit system call and finish
	machine->WriteRegister (RetAddrReg, 4);
	machine->WriteRegister (PCReg, (long) address);
	machine->WriteRegister (NextPCReg, (long) address + 4);
	machine->Run();                     // jump to the user progam
	//ASSERT (false);
}

/*
 *  System call interface: void Fork( void (*func)() )
 */
void NachOS_Fork() {    //System call # 9
	
	printf("FORK");
	Thread* newThread = new Thread ("Hilo Hijo");    //nuevo proceso hijo
	printf("FORK");
	newThread->tabla = currentThread->tabla; // Se copian los archivos abiertos
	
	printf("FORK");
	newThread->tablaSC = currentThread->tablaSC; // Se copian los systemcalls abiertos
	printf("FORK");
	currentThread->tabla->add_thread(); // Un hijo está usando la tabla de archivos
	printf("FORK");
	
	newThread->space = new AddrSpace (currentThread->space); // Asigna el espacio a al hijo (DS y CS = father. SS independiente)
	printf("FORK");
/*
	void* tmp = (void*) machine->ReadRegister (4);
	printf("FORK");
	newThread->Fork (forkAux, tmp);
	printf("FORK");
	*/
	returnFromSystemCall();


}//Kernel_Fork


/*
 *  System call interface: void Yield()
 */
void NachOS_Yield() {               //System call # 10
	
	DEBUG ('a', "Yield, returns control to the CPU.\n");
	currentThread->Yield();
	returnFromSystemCall(); // !
	DEBUG ('a', "Exiting Yield.\n");

} //Nachos_Yield

/*
 *  System call interface: Sem_t SemCreate( int )
 */
void NachOS_SemCreate() { // System call 11
	int initVal = machine->ReadRegister(4);
	Semaphore* sem = new Semaphore("semaphore", initVal);
	initVal = currentThread->tablaSC->Open((void*)sem);
	machine->WriteRegister(2, initVal);
	returnFromSystemCall();
	}

/*
 *  System call interface: int SemDestroy( Sem_t )
 */
void NachOS_SemDestroy() { // System call 12
	int pos = machine->ReadRegister(4);
	Semaphore* sem = (Semaphore*)currentThread->tablaSC->get_unix_handle(pos);
	delete sem;
	pos = currentThread->tablaSC->Close(pos);
	machine->WriteRegister(2, pos);
	returnFromSystemCall();
	}

/*
 *  System call interface: int SemSignal( Sem_t )
 */
void NachOS_SemSignal() { // System call 13
	int pos = machine->ReadRegister(4);
	Semaphore* sem = (Semaphore*)currentThread->tablaSC->get_unix_handle(pos);
	sem->V();
	returnFromSystemCall();
	}

/*
 *  System call interface: int SemWait( Sem_t )
 */
void NachOS_SemWait() { // System call 14
	int pos = machine->ReadRegister(4);
	Semaphore* sem = (Semaphore*)currentThread->tablaSC->get_unix_handle(pos);
	sem->P();
	returnFromSystemCall();
	}

/*
 *  System call interface: Lock_t LockCreate( int )
 */
void NachOS_LockCreate() { // System call 15
	int initVal = machine->ReadRegister(4);
	Lock* lock = new Lock("lock");
	currentThread->tablaSC->Open((void*)lock);
	returnFromSystemCall();
	}

/*
 *  System call interface: int LockDestroy( Lock_t )
 */
void NachOS_LockDestroy() { // System call 16
	int pos = machine->ReadRegister(4);
	Lock* lock = (Lock*)currentThread->tablaSC->get_unix_handle(pos);
	delete lock;
	currentThread->tablaSC->Close(pos);
	returnFromSystemCall();
	}

/*
 *  System call interface: int LockAcquire( Lock_t )
 */
void NachOS_LockAcquire() { // System call 17
	int posLock = machine->ReadRegister(4);
	Lock* lock = (Lock*)currentThread->tablaSC->get_unix_handle(posLock);
	lock->Acquire();
	machine->WriteRegister(2, posLock);
	returnFromSystemCall();
	}

/*
 *  System call interface: int LockRelease( Lock_t )
 */
void NachOS_LockRelease() { // System call 18
	int posLock = machine->ReadRegister(4);
	Lock* lock = (Lock*)currentThread->tablaSC->get_unix_handle(posLock);
	lock->Release();
	machine->WriteRegister(2, posLock);
	returnFromSystemCall();
	}

/*
 *  System call interface: Cond_t LockCreate( int )
 */
void NachOS_CondCreate() { // System call 19
	int initVal = machine->ReadRegister(4);
	Condition* cond = new Condition("condition");
	currentThread->tablaSC->Open((void*)cond);
	machine->WriteRegister(2, initVal);
	returnFromSystemCall();
	}

/*
 *  System call interface: int CondDestroy( Cond_t )
 */
void NachOS_CondDestroy() { // System call 20
	int pos = machine->ReadRegister(4);
	Condition* cond = (Condition*)currentThread->tablaSC->get_unix_handle(pos);
	delete cond;
	currentThread->tablaSC->Close(pos);
	returnFromSystemCall();
	}

/*
 *  System call interface: int CondSignal( Cond_t )
 */
void NachOS_CondSignal() { // System call 21
	int posCond = machine->ReadRegister(4);
	Condition* cond = (Condition*)currentThread->tablaSC->get_unix_handle(posCond);
	int posLock = machine->ReadRegister(5);
	Lock* lock = (Lock*)currentThread->tablaSC->get_unix_handle(posLock);
	cond->Signal(lock);
	machine->WriteRegister(2, posCond);
	returnFromSystemCall();
	}

/*
 *  System call interface: int CondWait( Cond_t )
 */
void NachOS_CondWait() { // System call 22
	int posCond = machine->ReadRegister(4);
	Condition* cond = (Condition*)currentThread->tablaSC->get_unix_handle(posCond);
	int posLock = machine->ReadRegister(5);
	Lock* lock = (Lock*)currentThread->tablaSC->get_unix_handle(posLock);
	cond->Wait(lock);
	machine->WriteRegister(2, posCond);
	returnFromSystemCall();
	}

/*
 *  System call interface: int CondBroadcast( Cond_t )
 */
void NachOS_CondBroadcast() { // System call 23
	int posCond = machine->ReadRegister(4);
	Condition* cond = (Condition*)currentThread->tablaSC->get_unix_handle(posCond);
	int posLock = machine->ReadRegister(5);
	Lock* lock = (Lock*)currentThread->tablaSC->get_unix_handle(posLock);
	cond->Broadcast(lock);
	machine->WriteRegister(2, posCond);
	returnFromSystemCall();
	}

/*
 *  System call interface: Socket_t Socket( int, int )
 */
void NachOS_Socket() {	// System call 30

	int domain = machine->ReadRegister(4);
	int type = machine->ReadRegister(5);

	Socket_t idSocket;

	if (domain == 0) {//Version 4
		if (type == 0) {//SOCKSTREAM
			idSocket = socket(AF_INET, SOCK_STREAM, 0);
			}
		else {
			idSocket = socket(AF_INET, SOCK_DGRAM, 0);
			}
		}
	else {// version 6
		if (type == 0) {//SOCKSTREAM
			idSocket = socket(AF_INET6, SOCK_STREAM, 0);
			}
		else {
			idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
			}
		}

	if (idSocket == -1) {
		printf("socket constructor -1\n");
		}

	int valor = currentThread->tabla->Open(idSocket);
	machine->WriteRegister(2, valor);
	returnFromSystemCall();
	}

/*
 *  System call interface: Socket_t Connect( char *, int )
 */
void NachOS_Connect() {	// System call 31

	struct sockaddr_in servidor;

	int port;
	int sockId;
	int ipAddr;
	int result;

	bzero((char*)&servidor, sizeof(servidor));

	sockId = machine->ReadRegister(4);
	ipAddr = machine->ReadRegister(5);
	port = machine->ReadRegister(6);

	int car = -1;
	int index = 0;
	char* addr = new char[128];

	while (0 != car) {
		machine->ReadMem(ipAddr, 1, &car);
		addr[index] = (char)car;  //Int to char

		ipAddr++;
		index++;
		}

	if (currentThread->tabla->is_opened(sockId)) {

		int unixHandle = currentThread->tabla->get_unix_handle(sockId);

		servidor.sin_family = AF_INET;
		inet_pton(AF_INET, addr, &servidor.sin_addr);
		servidor.sin_port = htons(port);

		result = connect(unixHandle, (struct sockaddr*)&servidor, sizeof(servidor));

		}

	if (-1 == result) {

		printf("-1 conect");

		}

	machine->WriteRegister(2, result);
	returnFromSystemCall();
	}

/*
 *  System call interface: int Bind( Socket_t, int )
 */
void NachOS_Bind() {	// System call 3

	int idSocket = machine->ReadRegister(4);
	int port = machine->ReadRegister(5);

	int sockSize = sizeof(struct sockaddr_in);

	struct sockaddr_in  addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	int unix_handle = currentThread->tabla->get_unix_handle(idSocket);
	int result = bind(unix_handle, (struct sockaddr*)&addr, sockSize);

	if (result == -1)
		perror("socket bind");

	machine->WriteRegister(2, result);
	returnFromSystemCall();
	}

/*
 *  System call interface: int Listen( Socket_t, int )
 */
void NachOS_Listen() {	// System call 33
	int idSocket = machine->ReadRegister(4);
	int backlog = machine->ReadRegister(5);
	int unix_handle = currentThread->tabla->get_unix_handle(idSocket);

	int resultado = listen(unix_handle, backlog);

	if (resultado == -1)
		perror("socket listen");

	machine->WriteRegister(2, resultado);
	returnFromSystemCall();
	}

/*
 *  System call interface: int Accept( Socket_t )
 */
void NachOS_Accept() {	// System call 34

	int idSocket = machine->ReadRegister(4);
	sockaddr_in addr;
	socklen_t size = sizeof(addr);
	int unix_handle = currentThread->tabla->get_unix_handle(idSocket);
	int resultado = accept(unix_handle, (struct sockaddr*)&addr, &size);

	if (resultado == -1)
		perror("Connection failed....\n");


	machine->WriteRegister(2, resultado);
	returnFromSystemCall();
	}

/*
 *  System call interface: int Shutdown( Socket_t, int )
 */
void NachOS_Shutdown() {	// System call 25
	int idSocket = machine->ReadRegister(4);
	int mode = machine->ReadRegister(5);

	int unix_handle = currentThread->tabla->get_unix_handle(idSocket);

	int resultado = shutdown(unix_handle, mode);
	machine->WriteRegister(2, resultado);
	returnFromSystemCall();
	}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which) {
	int type = machine->ReadRegister(2);

	switch (which) {

		case SyscallException:
			switch (type) {
				case SC_Halt: // System call # 0
					NachOS_Halt();
					break;
				case SC_Exit: // System call # 1
					NachOS_Exit();
					break;
				case SC_Exec: // System call # 2
					NachOS_Exec();
					break;
				case SC_Join: // System call # 3
					NachOS_Join();
					break;

				case SC_Create: // System call # 4
					NachOS_Create();
					break;
				case SC_Open: // System call # 5
					NachOS_Open();
					break;
				case SC_Read: // System call # 6
					NachOS_Read();
					break;
				case SC_Write: // System call # 7
					NachOS_Write();
					break;
				case SC_Close: // System call # 8
					NachOS_Close();
					break;

				case SC_Fork: // System call # 9
					NachOS_Fork();
					break;
				case SC_Yield: // System call # 10
					NachOS_Yield();
					break;

				case SC_SemCreate: // System call # 11
					NachOS_SemCreate();
					break;
				case SC_SemDestroy: // System call # 12
					NachOS_SemDestroy();
					break;
				case SC_SemSignal: // System call # 13
					NachOS_SemSignal();
					break;
				case SC_SemWait: // System call # 14
					NachOS_SemWait();
					break;

				case SC_LckCreate: // System call # 15
					NachOS_LockCreate();
					break;
				case SC_LckDestroy: // System call # 16
					NachOS_LockDestroy();
					break;
				case SC_LckAcquire: // System call # 17
					NachOS_LockAcquire();
					break;
				case SC_LckRelease: // System call # 18
					NachOS_LockRelease();
					break;

				case SC_CondCreate: // System call # 19
					NachOS_CondCreate();
					break;
				case SC_CondDestroy: // System call # 20
					NachOS_CondDestroy();
					break;
				case SC_CondSignal: // System call # 21
					NachOS_CondSignal();
					break;
				case SC_CondWait: // System call # 22
					NachOS_CondWait();
					break;
				case SC_CondBroadcast: // System call # 23
					NachOS_CondBroadcast();
					break;

				case SC_Socket: // System call # 30
					NachOS_Socket();
					break;
				case SC_Connect: // System call # 31
					NachOS_Connect();
					break;
				case SC_Bind: // System call # 32
					NachOS_Bind();
					break;
				case SC_Listen: // System call # 33
					NachOS_Listen();
					break;
				case SC_Accept: // System call # 32
					NachOS_Accept();
					break;
				case SC_Shutdown: // System call # 33
					NachOS_Shutdown();
					break;

				default:
					printf("Unexpected syscall exception %d\n", type);
					ASSERT(false);
					break;
				}
			break;

		case PageFaultException:
		{
		break;
		}

		case ReadOnlyException:
			printf("Read Only exception (%d)\n", which);
			ASSERT(false);
			break;

		case BusErrorException:
			printf("Bus -1 exception (%d)\n", which);
			ASSERT(false);
			break;

		case AddressErrorException:
			printf("Address -1 exception (%d)\n", which);
			ASSERT(false);
			break;

		case OverflowException:
			printf("Overflow exception (%d)\n", which);
			ASSERT(false);
			break;

		case IllegalInstrException:
			printf("Ilegal instruction exception (%d)\n", which);
			ASSERT(false);
			break;

		default:
			printf("Unexpected exception %d\n", which);
			ASSERT(false);
			break;
		}
	}
