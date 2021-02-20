#include "notas.h"

const int pinLed = 13;
const int pinBuzzer = 12;
const int notaAlarma = NOTE_C5;

void setup()
{
  Serial.begin(9600);
  pinMode(pinBuzzer, OUTPUT); 
  pinMode(pinLed, OUTPUT);

  Serial.print("Alarma 1 \n");
  parpadearYSonar(5, 1000);
  delay(1000);

  Serial.print("Alarma 2 \n");
  parpadearYSonar(5, 500);
  delay(1000);

  Serial.print("Alarma 3 \n");
  parpadearYSonar(5, 250);
}

void loop()
{
}

void parpadearYSonar(int veces, int duracionNota)
{
  for(int i=0; i<veces; i++)
  {
    digitalWrite(pinLed, HIGH);   
    tone(pinBuzzer, notaAlarma, duracionNota);   
    delay(duracionNota);
    noTone(pinBuzzer);   
    digitalWrite(pinLed , LOW);   
    delay(duracionNota);
  }
}
