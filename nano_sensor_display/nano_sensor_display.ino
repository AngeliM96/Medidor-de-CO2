/*----------------------------------------------------------
    Sensor de co2 usando serial
  ----------------------------------------------------------*/
#include <MHZ19_uart.h> // incluye librería para manejo del sensor de CO2

//-----------------Pines----------------------------
const int rx_pin = 3;     // Serial rx
const int tx_pin = 4;     // Serial tx
const int pinBuzzer = 12; // Buzzer
const int pinLed = 13;    // Led
//--------------------------------------------------
long loops = 0; // Contamos las veces que se ejecutó el loop
MHZ19_uart sensor; // creo el objeto del sensor

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
  pinMode(pinBuzzer, OUTPUT); // Setteamos los pines de buzzer y led
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);         // Iniciamos el serial
  alarma(1, 250);
  Serial.print("INICIANDO \n");
  delay(5000);                // Esperamos 5 segundos
  // Calentamos 
  sensor.begin(rx_pin, tx_pin);     // Setteamos los pines y iniciamos el sensor
  sensor.setAutoCalibration(false); //Se setea la autocalibracion en falso para poder calibrar cuando se desee
  Serial.print("Calentando, espere 2 minutos");
  delay(120000);                    // Esperamos 2 minutos
  alarma(3, 250);                    // Alarma indicando que terminó el calentamiento
}

//-----------------Loop----------------------------
void loop() {
  if(++loops % 30 == 0) { // Si loops es múltiplo de 30
    alarma(1, 250); 
    Serial.print("AireNuevo UNAHUR \n"); 
    Serial.print("MEDIDOR de CO2 \n");  
    delay(5000); //demora 5 seg entre mediciones
    loops = 0; // vuelve a empezar LOS LOOPS
  } 
  int co2ppm = sensor.getPPM(); // mide CO2
  int temp = sensor.getTemperature(); // mide temperatura

//  Muestra medición de CO2    
  Serial.print("CO2: " + String(co2ppm) + "ppm \n"); // Escribe CO2
//  Muestra medición de temperatura
  Serial.print("Temp: " + String(temp) + "*C \n"); // Escribe temperatura
  
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
