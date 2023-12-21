# Laboratorio Nº7 - Interfaz de Socket

## 📚 Enunciado

Se desea crear una aplicación distribuida de mensajería la cual consta de un servidor el cual envía en simultáneo mensajes a todas las aplicaciones cliente instaladas en una determinada subred.  

Las aplicaciones tienen que hacer lo siguiente:  

● Servidor:  

Al iniciarse la aplicación debe mostrar por pantalla un mensaje de bienvenida. A continuación debe abrir un puerto efímero aleatorio entre el 50.000 y el 65.000 (TCP o UDP según su criterio de diseño). Luego, la aplicación debe solicitar el ingreso de un mensaje vía teclado (de hasta 200 caracteres), enviar el mismo a todos los hosts de la subred y quedar a la espera de la escritura de un nuevo mensaje.  

● Cliente:  

Al iniciarse la aplicación debe mostrar por pantalla un mensaje de bienvenida. A continuación debe abrir el puerto registrado 2.500 (TCP o UDP según su criterio de diseño) y quedar a la espera de la recepción de un mensaje por parte del servidor. Una vez llegado un mensaje, la aplicación debe mostrar por pantalla la siguiente información:  

• Fecha y hora de la PC cliente.  
• IP y puerto desde el que llega el mensaje.  
• Mensaje.  

En ambos casos debe realizar el control de error de los ingresos de datos, funciones y las llamadas al sistema utilizadas, mostrando mensajes informativos en caso de falla.  

### Consideraciones:

• Debe usar IPv4 como protocolo de Internet.  
• Para evitar problemas de comunicación entre las aplicaciones, puede clonar la máquina virtual Ubuntu (disponible en Classroom) y hacer correr una aplicación en cada una para probar el laboratorio. En este caso, la configuración de red de cada máquina virtual debe ser “Red interna” (Nombre de la red “LAN”) y se le debe asignar una IP estática a cada una dentro de una misma red (si quiere, puede usar para su comodidad la red 192.168.1.0/24).  

## Construidos con 🛠️

* [VisualStudioCode](https://code.visualstudio.com/) - Editor de código fuente
* [GNU Make](https://www.gnu.org/software/make/) - Herramienta de gestión de dependencias
* [GCC](https://gcc.gnu.org/) - Compilador de C para Linux (Ubuntu)

## 👨‍💻 Autor

- Bustos Roldan, Mauro
- Fernandez Siles, Joaquin
- Medina Raed, Luis Eugenio