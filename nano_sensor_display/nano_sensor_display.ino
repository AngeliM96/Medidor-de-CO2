/*----------------------------------------------------------
    Sensor de co2 usando display I2C
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
long loops = 0;                         // Contamos las veces que se ejecutó el loop
MHZ19_uart sensor;                      // creo el objeto del sensor
LiquidCrystal_I2C display(0x27,16,2);   // Creo el objeto display  dirección  0x27 y 16 columnas x 2 filas
//-----------------Alarma----------------------------
void alarma(int veces, int duracionNota) {
  for(int i=0; i<veces; i++)
  {
    digitalWrite(pinLed, HIGH);           // Prendo led 
    tone(pinBuzzer, 523, duracionNota);   // Hago sonar el buzzer, 523 corresponde a la nota C5
    delay(duracionNota);                  // Espero lo que dura la nota
    noTone(pinBuzzer);                    // Silencio el buzzer
    digitalWrite(pinLed , LOW);           // Apago led
    delay(duracionNota);                  // Delay entre alarmas
  }
}
//-----------------Setup----------------------------
void setup() {
  pinMode(pinBuzzer, OUTPUT);       // Setteamos los pines de buzzer y led
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);               // Iniciamos el serial
  display.begin();                  // Inicio el display            
  display.clear();                  // Limpio la pantalla 
  display.backlight();              // Prendo la backlight
  alarma(1, 250);
  // Print por serial
  Serial.print("INICIANDO \n");
  display.setCursor(0, 1);          // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  // Print por display
  display.print("INICIANDO");
  display.clear()                   // Limpio la pantalla
  delay(5000);                      // Esperamos 5 segundos
  // Calentamos 
  sensor.begin(rx_pin, tx_pin);     // Setteamos los pines y iniciamos el sensor
  sensor.setAutoCalibration(false); //Se setea la autocalibracion en falso para poder calibrar cuando se desee
  // Print por serial
  Serial.print("Calentando, espere 2 minutos");
  // Print por display
  display.setCursor(0, 0);          // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("Calentando");      // Escribe primera linea del cartel
  display.setCursor(0, 1);          // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("Espere 2 minutos");// Escribe segunda linea del cartel
  delay(120000);                    // Esperamos 2 minutos
  display.clear()                   // Limpio la pantalla
  alarma(3, 250);                   // Alarma indicando que terminó el calentamiento
}

//-----------------Loop----------------------------
void loop() {
  if(++loops % 30 == 0) { // Si loops es múltiplo de 30
    alarma(1, 250); 
    // Print por serial
    Serial.print("AireNuevo UNAHUR \n"); 
    Serial.print("MEDIDOR de CO2 \n");  
    // Print por display
    display.clear();                            // Limpio la pantalla
    display.setCursor(0,0);                     // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    display.print("AireNuevo UNAHUR"); 
    display.setCursor(0, 1);                    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    display.print("MEDIDOR de CO2");
    delay(5000);
    loops = 0;                                  // Reinicio la cuenta
  } 
  int co2ppm = sensor.getPPM(); // mide CO2
  int temp = sensor.getTemperature(); // mide temperatura

  // Print por serial  
  Serial.print("CO2: " + String(co2ppm) + "ppm \n"); // Escribe CO2
  Serial.print("Temp: " + String(temp) + "*C \n"); // Escribe temperatura
  // Print por display
  // Muestra medición de CO2   
  display.clear();            // borra pantalla
  display.setCursor(0, 0);    // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("CO2: ");     // Escribe texto
  display.setCursor(8, 0);    // Ubicamos el cursor en la novena posición(columna:8) de la primera línea(fila:0)
  display.print(co2ppm);      // Escribe CO2
  display.setCursor(12, 0);   // Ubicamos el cursor en la treceava posición(columna:12) de la primera línea(fila:0)
  display.print("ppm");       // Escribe texto
  // Muestra medición de temperatura
  display.setCursor(0, 1);    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  display.print("Temp: ");    // Escribe texto
  display.setCursor(8, 1);    // Ubicamos el cursor en la novena posición(columna:8) de la segunda línea(fila:1)
  display.print(temp);        // Escribe temperatura
  display.setCursor(12, 1);   // Ubicamos el cursor en la treceava posición(columna:12) de la segunda línea(fila:1)
  display.print("*C");        // Escribe texto
  //  Emite una alarma en función del resultado
    if(co2ppm >= 1200){
      alarma(8, 250);
    } 
    else if(co2ppm >= 1000){
      alarma(4, 500);
    } 
    else if(co2ppm >= 800){
      alarma(2, 1000);
    } 
  delay(10000); //demora 10 seg entre mediciones
}
