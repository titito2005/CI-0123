# Cl0123_ProyectoIntegradorRedesSistemasOperativos_GrupoC

# Instrucciones para ejecutar el programa

    1. Ejecutar en la raíz del proyecto el comando "make servidor" para crear el servidor

    2. Antes de ejecutar el servidor, primero debe ingresar el comando "cd src" en la terminal
    
    3. Ejecutar primero ./servidor antes de ejecutar cualquier cliente

    4. Para compilar el cliente de nachOS se deben seguir los siguientes pasos: 
        -En la raiz del proyecto, ejecutar el comando cd clienteNachos.
       
        -Una vez en la carpeta clienteNachos, ejecutar cd code -> cd userprog -> make depend -> make
       
        -Despues ejecutar cd ../test -> make cliente -> cd ../userprog 
       
        -Ahora solo corresponde ejecutar el cliente una vez que ya se encuentra en userprog con el comando 
        /nachos -x ../test/cliente.
        
        -Aquí se desplegará un menú interactivo al usuario en donde hay 3 opciones: la primera es buscar información de un país, la segunda es buscar información de un cantón y la tercera es finalizar la ejecución del programa. Para escoger alguna de las opciones se debe ingresar el número 1,2 ó 3 según corresponda. 

        -Si se escoge la opción 1, entonces se debe ingresar el nombre del país que se desee ver. De igual manera deben ser nombres en inglés, si hay espacios se debe ingresar un guión, y deben ir mayúsculas según corresponda en las iniciales de los nombres. 

        -Si se escoge la opción 2, entonces se debe ingresar el nombre del cantón que se desee ver. Deben ser nombres sin tildes, si hay espacios se debe ingresar un guión, y deben ir mayúsculas según corresponda en las iniciales de los nombres.

        -Si se escoge la opción 3, entonces se finalizará la ejecución del cliente en consola. 
        NOTA IMPORTANTE: antes de ejecutar este cliente, primero debe ejecutar el servidor 


    5. Si desea ejecutar el cliente desde un browser web, entonces proceder al navegador e ingresar en la barra de búsquedas lo siguiente:
        -En el caso de querer ver la información de un país: localhost:8080/País 
            *Casos especiales a considerar: Los nombres de los países deben ser en inglés, si hay nombres de países con espacios debe ingresar un guión(-) en lugar de un espacio, los nombres de los países deben llevar mayúsculas según corresponda en sus iniciales.*

        -En el caso de querer ver la información de un cantón: localhost:8080/Costa-Rica/Cantón
            *Casos especiales a considerar: aplica lo mismo de arriba para los nombres de cantones y además los nombres deben ir sin tildes en la solicitud. En este caso si en la posición de Costa-Rica se ingresa otro país, se recibirá un código 400.
    
    7. Una vez que ya no se vayan a hacer más solicitudes, se debe proceder a cerrar la terminal que está corriendo el servidor.


# Fin de las instrucciones