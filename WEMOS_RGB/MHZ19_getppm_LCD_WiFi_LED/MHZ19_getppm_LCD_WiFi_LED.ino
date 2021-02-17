/*----------------------------------------------------------
    MH-Z19 CO2 sensor  SAMPLE - VERSIÓN PARA WEMOS D1 MINI
  ----------------------------------------------------------*/
#include <MHZ19_uart.h> // incluye librería para manejo del sensor de CO2
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <ESP8266WiFi.h>  // manejo de wifi de wemos

// Network SSID - nombre de la red wifi y clave
// const char* ssid = "nombre_red";  // nombre de la red
// const char* password = "xxxxxxxxx";  // clave
const char* ssid = "Fibertel WiFi707 2.4GHz";  // nombre de la red
const char* password = "14142222";  // clave

WiFiServer server(80);  // crea un servicio que escucha conecciones entrantes de un puerto específico

//Crear el objeto display  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C display(0x27,16,2);  //


const int rx_pin = 14;  //Serial rx pin no
const int tx_pin = 16; //Serial tx pin no
const int buzzer = 0 ; // control del buzzer

const int led_R = 12 ; // control LED ROJO
const int led_V = 13 ; // control LED VERDE
const int led_A = 15 ; // control LED AZUL
boolean anodoComun0 = false;   // Led RGB (ánodo común)

int cnt = 0; // cuenta LOOPS

MHZ19_uart mhz19; // asigna medidor al sensor
int co2ppm;

 
void setup() {

  // empieza programa de medición

  pinMode(led_R, OUTPUT);  //Inicia LED ROJO
  pinMode(led_V, OUTPUT);  //Inicia LED VERDE
  pinMode(led_A, OUTPUT);  //Inicia LED AZUL
  definirColor(0, 255, 0,anodoComun0);  // empieza con LED en verde
   
  pinMode(buzzer, OUTPUT); //Inicia buzzer
  tone(buzzer, 55,20); // pita para avisar que arrancó

  // Inicializar el display
  display.begin();
  display.clear(); // borra pantalla
  //Encender la luz de fondo.
  display.backlight();
  display.setCursor(0,0);
  display.print("AireNuevo UNAHUR"); // Cada minuto muestra el valor
  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  display.print("MEDIDOR de CO2");    // Escribe texto
  delay(10000); //muestra el cartel de inicio por 10 segundos
  display.clear(); // borra pantalla  
  
  Serial.begin(115200); // abre puerto serie para comunicarse
  delay(10);
 
  // Connecta con WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectándose a ");
  Serial.println(ssid); // escribe en el puerto serie a la red que intenta conectarse
  WiFi.hostname("Medidor_CO2");  // nombre con el que aparece el medidor en el router de wifi
  WiFi.begin(ssid, password);  // abre la conexión wifi

  while ((WiFi.status() != WL_CONNECTED) && (cnt < 3) ) {
    display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
    display.print("BUSCANDO WIFI "); // AVISA QUE BUSCA CONECTARSE A WIFI
    display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    display.print(++cnt); // AVISA QUE BUSCA CONECTARSE A WIFI

    Serial.print("."); // mientras que no se conecta escribe "." en el puerto serie 
    delay(1000); // espera 1 segundo
    display.clear(); // borra pantalla

    if ( cnt == 300) { // Si cnt es 300
      display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
      display.print("ERROR WIFI "); // AVISA QUE NO PUDO CONECTARSE A  WIFI
      display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
      display.print("NO CONECTADO "); // AVISA QUE NO PUDO CONECTARSE A  WIFI
      delay(5000); // espera 5 segundos
      display.clear(); // borra pantalla
    }    
  }
  cnt = 0; // pone contador de loops en cero
  
  if (WiFi.status() == WL_CONNECTED) { // está conectado a wifi
    display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
    display.print("WIFI conectado"); // AVISA QUE SE CONECTÓ A WIFI
    display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
    display.print( WiFi.localIP().toString().c_str()); // MUESTRA IP de WIFI
    delay(5000); // espera 5 segundos
    display.clear(); // borra pantalla
    
    Serial.println("");
    Serial.println("WiFi conectado"); // avisa en el puerto serie que está conectado
    // Print the IP address
    Serial.printf("Servidor Web Iniciado, abra %s en un mavegador web\n", WiFi.localIP().toString().c_str()); // avisa que se abrió la el puerto y da el número
    server.begin();
  } else {

  }


  // Escribimos Mensaje de inicio en el display.
  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  display.print("INICIANDO");
  delay(5000); // espera 5 segundos
  display.clear(); // borra pantalla
  
/*----------------------------------------------------------
    calentando MH-Z19 CO2 sensor
  ----------------------------------------------------------*/

  mhz19.begin(rx_pin, tx_pin); // inicializa el sensor
  mhz19.setAutoCalibration(false); // deshabilita autocalibrado automático
  
// muestra indicación de que está calentando 
  display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("Calentando");    // Escribe primera linea del cartel
  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:0)
  display.print("Espere 1 minuto");  // Escribe segunda linea del cartel
  
  delay(60000); // espera 1 minuto
  display.clear(); // borra pantalla  

  tone(buzzer, 55,100); // pitido de 55Hz indicando que funciona normal, calentado

}


// prepara página web para mandar a un cliente (servidor web)
String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 5\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html>"
               "co2:  ");
  htmlPage += co2ppm; // imprime el valor de CO2
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;
}

// rutina para manejar colores de leds
void definirColor(int rojo, int verde, int azul, boolean anodoComun)
{
  if(anodoComun == true)
  {
    rojo = 255 - rojo;
    verde = 255 - verde;
    azul = 255 - azul;
  }

  analogWrite(led_R, rojo); // Prende LED ROJO
  analogWrite(led_V, verde); // Prende LED VERDE
  analogWrite(led_A, azul); // Prende LED AZUL
}



/*----------------------------------------------------------
    MH-Z19 CO2 sensor  loop
  ----------------------------------------------------------*/
  
void loop() {
  
  if ( ++cnt % 30 == 0) { // Si cnt es múltiplo de 30
    display.clear(); // borra pantalla  
    display.setCursor(0,0);
    display.print("AireNuevo UNAHUR"); // Cada minuto muestra leyenda institucional
    display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    display.print("MEDIDOR de CO2");    // Escribe texto
    delay(5000); //demora 5 seg entre 30 mediciones
    cnt = 0; // vuelve a empezar LOS LOOPS

    if(WiFi.status() != WL_CONNECTED){  // si se desconectó, intenta conectarse de nuevo.
      Serial.println("Se perdió la conexion WiFi! Reiniciando..."); // avisa que se perdió la conexión de wifi
      WiFi.begin(ssid, password);  // abre la conexión wifi
      delay(30000); // espera 30 segundos para darle tiempo a conectarse

      if (WiFi.status() == WL_CONNECTED) { // está conectado a wifi
        display.clear(); // borra pantalla
        display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
        display.print("WIFI conectado"); // AVISA QUE SE CONECTÓ A WIFI
        display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
        display.print( WiFi.localIP().toString().c_str()); // MUESTRA IP de WIFI
        delay(5000); // espera 5 segundos
        display.clear(); // borra pantalla        
        server.begin();
      } else { // sigue desconectado
        display.clear(); // borra pantalla
        display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
        display.print("WIFI"); // AVISA QUE NO SE CONECTÓ A WIFI
        display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:1)
        display.print("Desconectado"); // AVISA QUE NO SE CONECTÓ A WIFI
        delay(5000); // espera 5 segundos
        display.clear(); // borra pantalla
     }
    } else { // sigue conectado a wifi
    }
  }  
//  int co2ppm = mhz19.getPPM(); // mide CO2
  co2ppm = mhz19.getPPM(); // mide CO2
  int temp = mhz19.getTemperature(); // mide temperatura

//  Muestra medición de CO2    
  display.clear(); // borra pantalla
  display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("CO2: ");    // Escribe texto
  display.setCursor(8, 0); // Ubicamos el cursor en la novena posición(columna:8) de la primera línea(fila:0)
  display.print(co2ppm); // Escribe CO2
  display.setCursor(12, 0); // Ubicamos el cursor en la treceava posición(columna:12) de la primera línea(fila:0)
  display.print("ppm"); // Escribe texto
  
//  Muestra medición de temperatura
/*  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  display.print("temp: ");    // Escribe texto
  display.setCursor(8, 1); // Ubicamos el cursor en la novena posición(columna:8) de la segunda línea(fila:1)
  display.print(temp); // Escribe temperatura
  display.setCursor(12, 1); // Ubicamos el cursor en la treceava posición(columna:12) de la segunda línea(fila:1)
  display.print("*C"); // Escribe texto
*/

//  Emite un sonido en función del resultado
    int tono = co2ppm / 100;
    switch (tono){
    case 3: // menor a 600ppm, 55Hz, 0.1 seg
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO BAJO");    // Escribe texto
       definirColor(0, 255, 0,anodoComun0); // PRENDE LED VERDE
       break;
    case 4: // menor a 600ppm, 55Hz, 0.1 seg
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO BAJO");    // Escribe texto
       definirColor(0, 255, 0,anodoComun0); // PRENDE LED VERDE
       break;
    case 5: // menor a 600ppm, 55Hz, 0.1 seg
//       tone(buzzer, 55,100);
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO BAJO");    // Escribe texto
       definirColor(0, 255, 0,anodoComun0); // PRENDE LED VERDE
       break;
    case 6: // menor a 700ppm, 55Hz, 0.1 seg// menor a 600ppm
//       tone(buzzer, 55,100);    
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO BAJO");    // Escribe texto
       definirColor(0, 255, 0,anodoComun0); // PRENDE LED VERDE
       break;
    case 7: // menor a 800ppm, 55Hz, 0.1 seg
       tone(buzzer, 55,100);
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO MEDIO");    // Escribe texto
       definirColor(255, 255, 0,anodoComun0); // PRENDE LED AMARILLO
       break;
    case 8: // menor a 900ppm, 110Hz, 1 seg
       tone(buzzer, 110,1000);
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO ALTO");    // Escribe texto       
       definirColor(255, 0, 0,anodoComun0); // PRENDE LED ROJO
       break;
    case 9: // menor a 1000ppm, 220Hz, 2 seg
       tone(buzzer, 220,2000);
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO ALTO");    // Escribe texto
       definirColor(255, 0, 0,anodoComun0); // PRENDE LED ROJO
       break;
    case 10: // menor a 1100ppm, 440Hz, 2 seg
       tone(buzzer, 440,2000); 
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO ALTO!!");    // Escribe texto
       definirColor(255, 0, 0,anodoComun0); // PRENDE LED ROJO
       break;
    case 11: // menor a 120ppm, 880Hz, 2 seg
       tone(buzzer, 880,2000); 
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO ALTO!!");    // Escribe texto
       definirColor(255, 0, 0,anodoComun0); // PRENDE LED ROJO
       break;
    default: // menor a 1300ppm, 880Hz, 4 seg
       tone(buzzer, 880,4000);
       display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
       display.print("RIESGO ALTO!!!!");    // Escribe texto
       definirColor(255, 0, 0,anodoComun0); // PRENDE LED ROJO
       break;
    }
  Serial.println(co2ppm); // escribe en el puerto serie el valor de CO2

  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
//*    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
//*        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }

    while (client.available()) {
      // but first, let client finish its request
      // that's diplomatic compliance to protocols
      // (and otherwise some clients may complain, like curl)
      // (that is an example, prefer using a proper webserver library)
      client.read();
    }

    // close the connection:
    client.stop();
//*    Serial.println("[Client disconnected]");
  }
  
    
  delay(10000); //demora 10 seg entre mediciones

}
