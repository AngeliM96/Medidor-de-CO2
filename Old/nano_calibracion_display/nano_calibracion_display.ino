/*----------------------------------------------------------
    Calibracion del sensor usando display I2C
  ----------------------------------------------------------*/
#include <MHZ19_uart.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//-----------------Pines----------------------------
const int rx_pin = 3;     // Serial rx
const int tx_pin = 4;     // Serial tx
const int pinBuzzer = 12; // Buzzer
const int pinLed = 13;    // Led

//--------------------------------------------------
const long segundosEspera = 1800;      // Cantidad de segundos a esperar
long segundosPasados = 0;              // Cuenta segundos
MHZ19_uart sensor;                     // Creo el objeto del sensor
LiquidCrystal_I2C display(0x27,16,2);  // Creo el objeto display  dirección  0x27 y 16 columnas x 2 filas
//-----------------Alarma----------------------------
void alarma(int veces) {
  for(int i=0; i<veces; i++)
  {
    digitalWrite(pinLed, HIGH);       // Prendo led 
    tone(pinBuzzer, 523, 250);        // Hago sonar el buzzer, 523 corresponde a la nota C5, 250 es la duración de la nota
    delay(250);                       // Espero lo que dura la nota
    noTone(pinBuzzer);                // Silencio el buzzer
    digitalWrite(pinLed , LOW);       // Apago led
    delay(250);                       // Delay entre alarmas
  }
}
//-----------Print-por-display----------------------
void displayPrint(int posicion, int linea, String texto) {
  display.setCursor(posicion, linea);       // Ubicamos el cursor en la posicion y linea deseada
  display.print(texto);                     // Escribe primera linea del cartel
}
//-----------------Setup----------------------------
void setup() {
  display.begin();                      // Inicio
  display.clear();                      // Limpio
  display.backlight();                  // Prendo la backlight
  // Setteamos los pines de buzzer y led
  pinMode(pinBuzzer, OUTPUT); 
  pinMode(pinLed, OUTPUT);
  sensor.begin(rx_pin, tx_pin);         // Setteamos los pines y iniciamos el sensor
  sensor.setAutoCalibration(false);     // Se setea la autocalibracion en falso para poder calibrar cuando se desee
  Serial.begin(9600);                   // Iniciamos el serial
  // Print por serial
  Serial.print("Calentando \n");        // Escribe primera linea del cartel
  Serial.print("Espere 1 minuto \n");   // Escribe segunda linea del cartel
  // Print por display
  display.clear();                      // Borra pantalla  
  displayPrint(0, 0, "Calentando");     // Escribe primera linea del cartel
  displayPrint(0, 1, "Espere 1 minuto");// Escribe segunda linea del cartel
  delay(60000);                         // Espera 60 segundos
  display.clear();                      // Borra pantalla  
  alarma(2);
}

//-----------------Loop-----------------------------
void loop() {
  if ( ++segundosPasados % 60 == 0) {                           // Si cnt es múltiplo de 60
    // Print por serial
    Serial.print(String(segundosPasados / 60) + " minutos \n"); // Cada minuto muestra el valor 
    Serial.print("CO2: " + String(sensor.getPPM()) + "ppm \n"); // Escribe CO2
    // Print por display
    display.clear();                                            // Borra pantalla  
    displayPrint(0, 0, String(segundosPasados / 60));           // Ubicamos el cursor en la primera posición(columna:0) de la primer línea(fila:0) y escribimos los minutos pasados
    displayPrint(7, 0, "minutos");
    displayPrint(0, 1, "CO2: ");                                // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) y escribimos el CO2
    displayPrint(8, 1, String(sensor.getPPM()));
    displayPrint(12, 1, "ppm");
  }
  else {
    // Print por display
    display.clear();                                            // Borra pantalla  
    displayPrint(0, 0, String(segundosPasados / 60));           // Ubicamos el cursor en la primera posición(columna:0) de la primer línea(fila:0) y escribimos los minutos pasados
    displayPrint(7, 0, "minutos");
  }
  
  delay(1000); // Espera 1 segundo

  if (segundosPasados > segundosEspera) { // Se pasaron 30 minutos 
    sensor.calibrateZero();               // Calibra
    // Print por serial
    Serial.print("PRIMERA CALIBRACION \n");
    // Print por display
    display.clear();                      // Limpio pantalla
    displayPrint(0, 0, "PRIMERA");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
    displayPrint(0, 1, "CALIBRACION");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) 
    alarma(1);
    delay(60000);                         // Espera 1 minuto
    sensor.calibrateZero();               // Calibra por segunda vez por las dudas
    // Print por serial
    Serial.print("SEGUNDA CALIBRACION \n");
    // Print por display
    display.clear();                      // Limpio pantalla     
    displayPrint(0, 0, "SEGUNDA");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
    displayPrint(0, 1, "CALIBRACION");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) 
    alarma(1);
    delay(60000);                         // Espera 1 minuto
    
    for(int i=0; i<10; i++) {             // Muestra mediciones
      // Print por serial
      Serial.print("CO2: " + String(sensor.getPPM()) + "ppm \n");
      // Print por display
      display.clear();                    // borra pantalla
      displayPrint(0, 0, "CO2: ");        // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) y escribimos el CO2
      displayPrint(8, 0, String(sensor.getPPM()));
      displayPrint(12, 0, "ppm");
      delay(10000);                       // Espera 10 segundos
    }
    segundosPasados = 0;                  // Vuelve a empezar la calibración
    alarma(5);
  }
}
