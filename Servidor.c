/* PROTOCOLOS DE COMUNICACION TCP/IP
Año: 2023
Tema: Laboratorio N7 - Interfaz de Socket

GRUPO 3

Integrantes:
- Bustos Roldan, Mauro
- Fernandez Siles, Joaquin
- Medina Raed, Luis Eugenio

Se desea crear una aplicación distribuida de mensajería la cual consta de un servidor el cual envía en
simultáneo mensajes a todas las aplicaciones cliente instaladas en una determinada subred. 

// Código correspondiente al SERVIDOR

• Servidor:
Al iniciarse la aplicación debe mostrar por pantalla un mensaje de bienvenida. A continuación debe abrir
un puerto efímero aleatorio entre el 50.000 y el 65.000 (TCP o UDP según su criterio de diseño). Luego, la
aplicación debe solicitar el ingreso de un mensaje vía teclado (de hasta 200 caracteres), enviar el mismo a
todos los hosts de la subred y quedar a la espera de la escritura de un nuevo mensaje. */

/* === Inclusion de cabeceras ================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

/* === Declaraciones globales ================================================================ */
#define PORT_MIN 50000
#define PORT_RANGE 15000
#define PORT_CLIENT 2500
#define MAX_MESSAGE_SIZE 200
#define WELCOME "Bienvenido al servidor de mensajería."
#define ERROR_SOCKET_CREATION "ERROR: En la creacion del socket"
#define ERROR_BROADCAST_SOCKET "ERROR: Al configurar socket en modo broadcast"
#define ERROR_LENGTH_EXCEEDED "ERROR: El mensaje excede los 200 caracteres"
#define ERROR_SEND_MESSAGE "ERROR: No se pudo enviar el mensaje"
#define ERROR_BINDING "ERROR: No fue posible asociar el socket con la dirección y puerto del servidor"
#define WAIT_USER_INPUT "Ingrese un mensaje: "
#define SUCCESS_MESSAGE_SENT "Mensaje enviado correctamente"
#define SUCCESS_CLOSE_SENT "Mensaje de CLOSE enviado correctamente"
#define CLOSING_APPLICATION "Cerrando la aplicación"
#define SERVER_IP "192.168.1.1"
#define BROADCAST_IP "255.255.255.255"
#define CLOSING_KEYWORD "Close\0"


/* === Funcion Principal ================================================================ */
int main(void){
    system("clear"); // Limpia la consola  

    int socketDescriptor, flag_close=0; // Declaración de la variable 'socketDescriptor' para almacenar el descriptor del socket y 'flag_close' para controlar la salida de la aplicacion
    int port; // Declaración de una variable entera 'port' para almacenar el número de puerto a utilizar
    int flag_length_exceeded=1; // Declaración de una variable entera llamada 'flag_length_exceeded' para controlar si el mensaje enviado es demasiado largo
    int broadcast=1; // Declaracion de variable usada en setsockopt para activar transmisiones.
    
    struct sockaddr_in serverAddress; // Estructura para almacenar la dirección del SERVIDOR en la red 
    struct sockaddr_in clientAddress; // Estructura para almacenar la dirección del CLIENTE en la red 
        
    char message[MAX_MESSAGE_SIZE]; // Arreglo de caracteres para almacenar el mensaje. El tamaño máximo del mensaje es 200.
    
    int client_struct_length = sizeof(clientAddress); // Variable para almacenar el tamaño de la estructura de dirección del CLIENTE

    printf("\t%s\n\n", WELCOME); // Imprime un mensaje de bienvenida al servidor de mensajería

    // GENERACION DE PUERTO
    srand(time(NULL)); // Iniciar semilla con tiempo actual
    port = rand(); // Genera un número aleatorio en el rango de 0 a RAND_MAX
    port = port % PORT_RANGE; // Reduce el rango del número aleatorio al rango definido por PORT_RANGE (15000), ahora en el rango [0, 14999]
    port = port + PORT_MIN; // Se ajusta el número aleatorio al rango deseado agregando PORT_MIN (50000), ahora en el rango [50000, 64999]

    // CREACION Y CONFIGURACION SOCKET
    socketDescriptor = socket(AF_INET,SOCK_DGRAM,0); // Creación de un socket UDP utilizando la familia de direcciones de Internet (AF_INET)
    
    if(socketDescriptor<0){  // Comprueba si la creación del socket fue exitosa
        printf("%s\n",ERROR_SOCKET_CREATION); // Imprime un mensaje de error si la creación del socket falló
        return -1; // Termina la ejecución de la aplicacion con un código de error
    }

    if (setsockopt(socketDescriptor,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof broadcast)!=0) { // Configura el socket para permitir la transmisión de broadcast
        printf("%s\n",ERROR_BROADCAST_SOCKET);  // Imprime un mensaje de error si no se pudo configurar el socket en modo broadcast
        close(socketDescriptor); // Cierra el socket
        return -1; // Termina la ejecución de la aplicacion con un código de error
    }

    // CONFIGURACION SERVIDOR
    memset(&serverAddress,0,sizeof serverAddress); // Limpia la estructura de dirección del servidor para evitar información no deseada
    serverAddress.sin_family = AF_INET; // Establece la familia de direcciones del servidor como IPv4
    serverAddress.sin_port = htons(port); // Asigna el número de puerto del servidor (convertido al formato de red)
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP); // Asigna la dirección IP del servidor (en este caso, 192.168.1.1)
    if(bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof serverAddress) < 0){ // Asocia el socket con la dirección y el puerto del SERVIDOR
        printf("%s\n",ERROR_BINDING); // Imprime un mensaje en caso que haya error de asociación
        return -1;
    }

    // CONFIGURACION CLIENTES
    memset(&clientAddress,0,sizeof clientAddress); // Limpia la estructura de dirección del cliente para evitar información no deseada
    clientAddress.sin_family = AF_INET; // Establece la familia de direcciones del cliente como IPv4
    clientAddress.sin_port=htons(PORT_CLIENT); // Asigna el número de puerto del cliente (convertido al formato de red)
    clientAddress.sin_addr.s_addr = inet_addr(BROADCAST_IP); // Asigna la dirección IP del cliente (en este caso, 255.255.255.255, que es una dirección de broadcast)

    while(flag_close==0){ // Bucle principal: mientras la bandera de salida no se haya activado     
        do{ 
            memset(message,'\0',sizeof(message)); // Limpia el buffer del mensaje
    
            if(flag_length_exceeded==0){ // Control de que el mensaje sea menor a 200 caracteres
                printf("\n%s\n\n",ERROR_LENGTH_EXCEEDED);
                memset(message,'\0',sizeof(message)); // Limpia el buffer del mensaje si excede los 200 caracteres             
            }

            printf("%s",WAIT_USER_INPUT); // Solicita al usuario que ingrese el mensaje
            gets(message);  // Lee el mensaje ingresado por el usuario
            if(strlen(message)>200){
                flag_length_exceeded=0; // Activa la bandera de longitud excedida
            }
            else{
                flag_length_exceeded=1; // Desactiva la bandera de longitud excedida
            }

        }while(flag_length_exceeded==0);

        
        if(sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr*)&clientAddress, client_struct_length) < 0){ // Envio de mensajes a los clientes
            printf("\n%s\n",ERROR_SEND_MESSAGE); // Si el envío del mensaje falla imprime un mensaje de error
            return -1; // Termina la ejecución de la aplicacion
        }

        if (strcasecmp(message,CLOSING_KEYWORD) == 0){ // Si el mensaje es "Close" 
            printf("\n%s\n\n",SUCCESS_CLOSE_SENT);  // Imprime un mensaje indicando que se envió "Close"            
            flag_close=1;  // Activa la bandera de salida
        }else{
            printf("\n%s\n\n",SUCCESS_MESSAGE_SENT); // Si el mensaje no es "Close", imprime un mensaje indicando que el mensaje fue enviado
        }
    }

    printf("\n%s\n\n\n",CLOSING_APPLICATION); // Imprime un mensaje indicando que la aplicación se está cerrando
    
    close(socketDescriptor); // Cierre del socket
    
    return 0;
}