/*----------------------------------------------------------
    Medidor de CO2 con display I2C y pulsador WEMOS
  ----------------------------------------------------------*/
#include <MHZ19_uart.h>     
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//-----------------Pines----------------------------
const int rx_pin = 14;     // Serial rx
const int tx_pin = 16;     // Serial tx
const int pinLedR = 12;    // Led Rojo
const int pinLedG = 13;    // Led Verde
const int pinLedB = 15;    // Led Azul
//--------------------------------------------------
long loops = 0;                         // Contamos las veces que se ejecutó el loop
MHZ19_uart sensor;                      // creo el objeto del sensor
LiquidCrystal_I2C display(0x27,16,2);   // Creo el objeto display  dirección  0x27 y 16 columnas x 2 filas
//-----------------Setup----------------------------
void setup() {
  pinMode(pinLedR, OUTPUT);                 //Inicia LED ROJO
  pinMode(pinLedG, OUTPUT);                 //Inicia LED VERDE
  pinMode(pinLedB, OUTPUT);                 //Inicia LED AZUL
  digitalWrite(pinLedR, 255);
  digitalWrite(pinLedG, 255);
  digitalWrite(pinLedB, 255);
  rgb('b');
  Serial.begin(9600);                    // Iniciamos el serial
  display.begin();                       // Inicio el display            
  display.clear();                       // Limpio la pantalla 
  display.backlight();                   // Prendo la backlight
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
  Serial.print("Calentando, espere 1 minuto \n");
  // Print por display
  displayPrint(0, 0, "Calentando");       // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "Espere 1 minuto"); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  delay(60000);                          // Esperamos 2 minutos
  display.clear();                        // Limpio la pantalla
  rgb('g');
}

void loop() {
  // Se presenta el nombre del proyecto cada 30 loops
  if(++loops % 30 == 0) { // Si loops es múltiplo de 30 
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
  imprimirCO2(co2ppm, temp);
  //  Emite una alarma en función del resultado
  if(co2ppm >= 1000 and co2ppm < 1200){
    rgb('r');                         
  } 
  else if(co2ppm >= 800 and co2ppm < 1200){
    rgb('y');
  }
  else if(co2ppm < 800){
    rgb('g');
  }
  while(sensor.getPPM() >= 1200) {
    rgb('r'); 
  }
  imprimirCO2(sensor.getPPM(), sensor.getTemperature());
  delay(10000); //demora 10 seg entre mediciones
}
