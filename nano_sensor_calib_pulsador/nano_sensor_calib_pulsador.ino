/*----------------------------------------------------------
    Medidor de CO2 con display I2C y pulsador
  ----------------------------------------------------------*/
#include <MHZ19_uart.h>     
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//-----------------Pines----------------------------
const int rx_pin = 3;     // Serial rx
const int tx_pin = 4;     // Serial tx
const int pinBuzzer = 12; // Buzzer
const int pinLed = 13;    // Led
const int pinCalib = 2;  // Pulsador calibracion
//--------------------------------------------------
long loops = 0;                         // Contamos las veces que se ejecutó el loop
MHZ19_uart sensor;                      // creo el objeto del sensor
LiquidCrystal_I2C display(0x27,16,2);   // Creo el objeto display  dirección  0x27 y 16 columnas x 2 filas
//-----------------Setup----------------------------
void setup() {
  pinMode(pinCalib, INPUT_PULLUP); // entrada pulsado para calibrar, seteada como pulluppara poder conectar pulsador sin poenr resistencia adicional
  pinMode(pinBuzzer, OUTPUT);            // Setteamos los pines de buzzer y led
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);                    // Iniciamos el serial
  display.begin();                       // Inicio el display            
  display.clear();                       // Limpio la pantalla 
  display.backlight();                   // Prendo la backlight
  alarma(1, 250);
  // Print por serial
  Serial.print("INICIANDO \n");
  // Print por display
  displayPrint(0, 1, "INICIANDO");       // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  delay(5000);                           // Esperamos 5 segundos
  display.clear();                       // Limpio la pantalla
  // Calentamos 
  sensor.begin(rx_pin, tx_pin);          // Setteamos los pines y iniciamos el sensor
  sensor.setAutoCalibration(false);      //Se setea la autocalibracion en falso para poder calibrar cuando se desee
  // Presentamos el medidor
  // Print por serial
  Serial.print("AireNuevo UNAHUR \n"); 
  Serial.print("MEDIDOR de CO2 \n");  
  // Print por display
  display.clear();                            
  displayPrint(0, 0, "AireNuevo UNAHUR");     // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  displayPrint(0, 1, "MEDIDOR de CO2");       // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  delay(5000);                                
  display.clear();                            // Limpio la pantalla
  // Print por serial
  Serial.print("Calentando, espere 1 minuto");
  // Print por display
  displayPrint(0, 0, "Calentando");       // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "Espere 1 minuto"); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  delay(60000);                          // Esperamos 2 minutos
  display.clear();                        // Limpio la pantalla
  alarma(3, 250);                         // Alarma indicando que terminó el calentamiento
}

void loop() {
  // Si detecta que se presionó el pulsador, se ejecuta la calibración
  if (digitalRead(pinCalib) == LOW) { 
    calibrar();
  }
  // Se presenta el nombre del proyecto cada 30 loops
  if(++loops % 30 == 0) { // Si loops es múltiplo de 30
    alarma(1, 250); 
    // Print por serial
    Serial.print("AireNuevo UNAHUR \n"); 
    Serial.print("MEDIDOR de CO2 \n");  
    // Print por display
    display.clear();                            // Limpio la pantalla
    displayPrint(0, 0, "AireNuevo UNAHUR");     // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    displayPrint(0, 1, "MEDIDOR de CO2");       // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    delay(5000);
    loops = 0;                                  // Reinicio la cuenta
  } 
  int co2ppm = sensor.getPPM(); // mide CO2
  int temp = sensor.getTemperature(); // mide temperatura

  // Print por serial  
  Serial.print("CO2: " + String(co2ppm) + "ppm \n");  // Escribe CO2
  Serial.print("Temp: " + String(temp) + "*C \n");    // Escribe temperatura
  // Print por display
  // Muestra medición de CO2   
  display.clear();                                    // Borra pantalla
  displayPrint(0, 0, "CO2: ");                        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(8, 0, String(co2ppm));                 // Ubicamos el cursor en la novena posición(columna:8) de la primera línea(fila:0)
  displayPrint(12, 0, "ppm");                         // Ubicamos el cursor en la treceava posición(columna:12) de la primera línea(fila:0)
  // Muestra medición de temperatura
  displayPrint(0, 1, "Temp: ");                       // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  displayPrint(8, 1, String(temp));                   // Ubicamos el cursor en la novena posición(columna:8) de la segunda línea(fila:1)
  displayPrint(12, 1, "*C");                          // Ubicamos el cursor en la treceava posición(columna:12) de la segunda línea(fila:1)
  
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
