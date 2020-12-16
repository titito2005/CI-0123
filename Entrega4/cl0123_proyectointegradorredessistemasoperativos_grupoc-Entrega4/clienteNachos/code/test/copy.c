#include "syscall.h"



// void metodito();

// int main()
// {
//  //    OpenFileId input;
//  //    OpenFileId output;
//  //    char buffer[1024];
//  //    int n = 0;


//  //    Create( "nachos.2" );
//  //    input  = Open( "nachos.1" );
//  //    output = Open( "nachos.2" );
//  //    while( (n = Read( buffer, 1024, input ) ) > 0 ) {
// 	// Write( buffer, n, output);
//  //    }
//  //    Close( input );
//  //    Close( output );
//     Fork( metodito );
//     Exit( 0 );
// }

// void metodito(){
//     printf("%s\n", "Hola");
//     Exit(0);
// }
void Verifica();

int main() {

    Fork( Verifica );

}

void Verifica() {
    //char buffer [5] = "Hola";
    //Write(buffer,5,1);
   //Exit(0);
}
