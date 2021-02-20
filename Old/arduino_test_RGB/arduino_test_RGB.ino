int pinRojo = 7;
int pinVerde = 6;
int pinAzul = 5;
int retardo = 2000;

void setup() {
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);  
}

void definirColor(int rojo, int verde, int azul)
{
  rojo = 255 - rojo;
  verde = 255 - verde;
  azul = 255 - azul;
  analogWrite(pinRojo, rojo);
  analogWrite(pinVerde, verde);
  analogWrite(pinAzul, azul);  
  delay(retardo); 
}

void loop()
{
  definirColor(255, 0, 0);  // rojo
  definirColor(0, 255, 0);  // verde
  definirColor(0, 0, 255);  // azul
  definirColor(255, 255, 0);  // amarillo 
  definirColor(255, 0, 255);  // violeta
  definirColor(0, 255, 255);  // cyan
}
