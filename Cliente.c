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

// Código correspondiente al CLIENTE

• Cliente:
Al iniciarse la aplicación debe mostrar por pantalla un mensaje de bienvenida. A continuación debe abrir
el puerto registrado 2.500 (TCP o UDP según su criterio de diseño) y quedar a la espera de la recepción
de un mensaje por parte del servidor. Una vez llegado un mensaje, la aplicación debe mostrar por
pantalla la siguiente información:

o Fecha y hora de la PC cliente.
o IP y puerto desde la que llega el mensaje.
o Mensaje.

En ambos casos debe realizar el control de error de los ingresos de datos, funciones y las llamadas a sistema
utilizadas, mostrando mensajes informativos en caso de falla.*/

/* === Inclusion de cabeceras ================================================================ */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


/* === Declaraciones globales ================================================================ */
#define PORT_CLIENT 2500
#define MAX_MESSAGE_SIZE 200
#define WELCOME "Bienvenido al cliente de mensajería."
#define ERROR_SOCKET_CREATION "ERROR: En la creacion del socket"
#define ERROR_RECEIVE_MESSAGE "ERROR: No se pudo recibir el mensaje"
#define ERROR_BINDING "ERROR: No fue posible asociar el socket con la dirección y puerto del cliente"
#define SUCCESS_CLOSE_RECEIVED "Mensaje de CLOSE recibido correctamente"
#define SUCCESS_BINDING "Asociación exitosa del socket con la dirección y puerto del servidor"
#define WAITING_FOR_MESSAGES "Esperando nuevos mensajes..."
#define NEW_MESSAGE_NOTIFICATION "Nuevo mensaje recibido!"
#define CLOSING_APPLICATION "Cerrando la aplicación"
#define LABEL_ORIGIN_IP "IP Origen:"
#define LABEL_PORT "Puerto:"
#define LABEL_THETIME "Fecha y Hora:"
#define LABEL_MESSAGE "Mensaje:"
#define DATE_FORMAT "%d/%m/%y"
#define TIME_FORMAT "%H:%M:%S"
#define CLOSING_KEYWORD "Close\0"

/* === Funcion Principal ================================================================ */
int main(){
    system("clear"); // Limpia la consola
    
    int socketDescriptor, flag_close=0; // Declaración de la variable 'socketDescriptor' para almacenar el descriptor del socket y 'flag_close' para controlar la salida de la aplicacion
    struct sockaddr_in serverAddress; // Estructura para almacenar la dirección del SERVIDOR en la red 
    struct sockaddr_in clientAddress; // Estructura para almacenar la dirección del CLIENTE en la red 
    int client_struct_length = sizeof(clientAddress);  // Variable para almacenar el tamaño de la estructura de dirección del CLIENTE
    int server_struct_length = sizeof(serverAddress); // Variable para almacenar el tamaño de la estructura de dirección del SERVIDOR

    char message[MAX_MESSAGE_SIZE]; // Arreglo de caracteres para almacenar el mensaje. El tamaño máximo del mensaje es 200.

    printf("\t%s\n\n",WELCOME); // Imprime un mensaje de bienvenida al servidor de mensajería
    
    socketDescriptor = socket(AF_INET,SOCK_DGRAM,0); // Creación de un socket UDP utilizando la familia de direcciones de Internet (AF_INET)
    
    if(socketDescriptor<0){ // Comprueba si la creación del socket fue exitosa 
        printf("%s\n",ERROR_SOCKET_CREATION); // Imprime un mensaje de error si la creación del socket falló
        return -1; // Termina la ejecución del programa con un código de error
    }
    
    clientAddress.sin_family = AF_INET; // Establece la familia de direcciones del CLIENTE como IPv4
    clientAddress.sin_port = htons(PORT_CLIENT); // Asigna el número de puerto del CLIENTE (convertido al formato de red)
    clientAddress.sin_addr.s_addr = htons(INADDR_ANY); // Establecer la dirección IP del cliente a cualquier dirección IP que esté asignada a la máquina local.
    
    if(bind(socketDescriptor,(struct sockaddr*)&clientAddress,sizeof clientAddress)<0){ // Asocia el socket con la dirección y el puerto del CLIENTE
        printf("%s\n",ERROR_BINDING); // Imprime un mensaje en caso que haya error de asociación
        return -1;
    }

    printf("%s\n\n%s\n",SUCCESS_BINDING,WAITING_FOR_MESSAGES); // Mensaje de éxito al realizar la función bind

    memset(message,'\0',sizeof(message)); // Limpia el buffer del mensaje
    
    while(flag_close==0){ // Bucle principal: mientras la bandera de cierre no se haya activado
        if (recvfrom(socketDescriptor,message,sizeof(message),0,(struct sockaddr*)&serverAddress,&server_struct_length)<0){ // Se recibe mensaje del servidor
            printf("%s\n",ERROR_RECEIVE_MESSAGE); // Si la recepción del mensaje falla imprime un mensaje de error
            return -1; // Termina la ejecucion de la aplicacion
        }

        printf("\n\n%s\n",NEW_MESSAGE_NOTIFICATION); // Imprime notificacion de que se recibio un nuevo mensaje
        printf("\n* %s %s",LABEL_ORIGIN_IP, inet_ntoa(serverAddress.sin_addr)); // Imprime IP Origen del mensaje recibido
        printf("\n* %s %i",LABEL_PORT, ntohs(serverAddress.sin_port)); // Imprime PUERTO del mensaje recibido

        time_t tiempo = time(0); // Obtiene el tiempo actual
        struct tm *tlocal = localtime(&tiempo); // Convierte el tiempo a una estructura tm local
        char dateOutput[20];
        char timeOutput[20];
        strftime(dateOutput, sizeof(dateOutput), DATE_FORMAT, tlocal); // Formatea la fecha 
        strftime(timeOutput, sizeof(timeOutput), TIME_FORMAT, tlocal);  // Formatea la hora 
        
        char dateTimeOutput[40];
        snprintf(dateTimeOutput, sizeof(dateTimeOutput), "%s %s", dateOutput, timeOutput); // Concatena la fecha y la hora en una única cadena
        
        printf("\n* %s %s\n",LABEL_THETIME,dateTimeOutput); // Imprime la fecha y la hora

        printf("* %s %s\n",LABEL_MESSAGE,message);  // Imprime el mensaje recibido

        if (strcasecmp(message,CLOSING_KEYWORD) == 0){ // Si el mensaje es "Close"
            printf("\n%s\n\n",SUCCESS_CLOSE_RECEIVED); // Imprime un mensaje indicando que se recibió "Close"
            flag_close=1; // Activar la bandera de salida   
        }

        memset(message,'\0',sizeof(message)); // Limpia el buffer del mensaje
    }
    
    printf("%s\n\n\n",CLOSING_APPLICATION); // Imprime un mensaje indicando que la aplicación se está cerrando
    close(socketDescriptor);    // Cerramos el socket
    
    return 0;
}