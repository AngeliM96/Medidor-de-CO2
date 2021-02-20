/*----------------------------------------------------------
    Calibracion del sensor usando serial
  ----------------------------------------------------------*/
#include <MHZ19_uart.h>

//-----------------Pines----------------------------
const int rx_pin = 0;     // Serial rx
const int tx_pin = 1;     // Serial tx
const int pinBuzzer = 12; // Buzzer
const int pinLed = 13;    // Led

//--------------------------------------------------
const long segundosEspera = 1800; // cantidad de segundos a esperar
long segundosPasados = 0; // cuenta segundos
MHZ19_uart sensor; // creo el objeto del sensor

//-----------------Alarma----------------------------
void alarma(int veces) {
  for(int i=0; i<veces; i++)
  {
    digitalWrite(pinLed, HIGH);  // Prendo led 
    tone(pinBuzzer, 523, 250);   // Hago sonar el buzzer, 523 corresponde a la nota C5, 250 es la duración de la nota
    delay(250);                  // Espero lo que dura la nota
    noTone(pinBuzzer);           // Silencio el buzzer
    digitalWrite(pinLed , LOW);  // Apago led
    delay(250);                  // Delay entre alarmas
  }
}

//-----------------Setup----------------------------
void setup() {
  // Setteamos los pines de buzzer y led
  pinMode(pinBuzzer, OUTPUT); 
  pinMode(pinLed, OUTPUT);
  sensor.begin(rx_pin, tx_pin); // Setteamos los pines y iniciamos el sensor
  sensor.setAutoCalibration(false); //Se setea la autocalibracion en falso para poder calibrar cuando se desee
  Serial.begin(9600); // Iniciamos el serial
  Serial.print("Calentando \n");    // Escribe primera linea del cartel
  Serial.print("Espere 1 minuto \n");  // Escribe segunda linea del cartel
  delay(60000); // Espera 1 minuto
  alarma(2);
}

//-----------------Loop-----------------------------
void loop() {
  if ( ++segundosPasados % 60 == 0) { // Si cnt es múltiplo de 60
    Serial.print(String(segundosPasados / 60) + "minutos \n"); // Cada minuto muestra el valor 
    Serial.print("co2: " + String(sensor.getPPM()) + "ppm \n"); // Escribe CO2
    alarma(1);
  } else {
    Serial.print(String(segundosPasados)+ "segundos \n");    
  }
  
  delay(1000); // Espera 1 segundo

  if (segundosPasados > segundosEspera) { // Se pasaron 30 minutos 
    sensor.calibrateZero(); // Calibra
    Serial.print("PRIMERA CALIBRACION \n");    // Escribe texto
    alarma(1);
    delay(60000); // Espera 1 minuto
    sensor.calibrateZero();  // Calibra por segunda vez por las dudas
    Serial.print("SEGUNDA CALIBRACION \n");    // Escribe texto
    alarma(2);
    
    for(int i=0; i<10; i++) { // muestra mediciones
      Serial.print("co2: " + String(sensor.getPPM()) + "ppm \n");    // Escribe texto
      delay(10000); // Espera 10 segundos
    }
    segundosPasados = 0; // vuelve a empezar la calibración
    alarma(5);
  }
}
