int pinRojo = 7;
int pinVerde = 6;
int pinAzul = 5;
int retardo = 2000;
boolean anodoComun0 = true;

void setup() {
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);  

}

void definirColor(int rojo, int verde, int azul, boolean anodoComun)
{
  if(anodoComun == true)
  {
    rojo = 255 - rojo;
    verde = 255 - verde;
    azul = 255 - azul;
  }
  analogWrite(pinRojo, rojo);
  analogWrite(pinVerde, verde);
  analogWrite(pinAzul, azul);  
}


void loop()
{
  definirColor(255, 0, 0,anodoComun0);  // rojo
  delay(retardo);
  definirColor(0, 255, 0,anodoComun0);  // verde
  delay(retardo);
  definirColor(0, 0, 255,anodoComun0);  // azul
  delay(retardo);
  definirColor(255, 255, 0,anodoComun0);  // amarillo
  delay(retardo);  
  definirColor(255, 0, 255,anodoComun0);  // violeta
  delay(retardo);
  definirColor(0, 255, 255,anodoComun0);  // cyan
  delay(retardo);
}
