
/*----------------------------------------------------------
    MH-Z19 CO2 sensor  SAMPLE
  ----------------------------------------------------------*/
#include <MHZ19_uart.h> // incluye librería para manejo del sensor de CO2
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto display  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C display(0x27,16,2);  //


const int rx_pin = 14;	//Serial rx pin no
const int tx_pin = 16;	//Serial tx pin no
const int buzzer = 0 ;
int cnt = 0; // cuenta LOOPS

const int led_R = 12 ; // control LED ROJO
const int led_V = 13 ; // control LED VERDE
const int led_A = 15 ; // control LED AZUL
boolean anodoComun0 = false;   // Led RGB (ánodo común)

MHZ19_uart mhz19; // asigna medidor al sensor

void setup() {
  // empieza programa de medición

  pinMode(led_R, OUTPUT);  //Inicia LED ROJO
  pinMode(led_V, OUTPUT);  //Inicia LED VERDE
  pinMode(led_A, OUTPUT);  //Inicia LED AZUL
  definirColor(0, 255, 0,anodoComun0);  // empieza con LED en verde
   
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); //Inicia buzzer
  tone(buzzer, 55,20);
  // Inicializar el display
  display.begin();
  display.clear(); // borra pantalla
  
  //Encender la luz de fondo.
  display.backlight();

    // Escribimos el Mensaje en el display.
  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  display.print("INICIANDO");
  delay(5000);
  display.clear(); // borra pantalla
/*----------------------------------------------------------
    calentando MH-Z19 CO2 sensor
  ----------------------------------------------------------*/

  mhz19.begin(rx_pin, tx_pin); // inicializa el sensor
  mhz19.setAutoCalibration(false);

// se presenta 
    tone(buzzer, 55,20); // pita para mostrar cartel
    display.setCursor(0,0);
    display.print("AireNuevo UNAHUR"); // Cada minuto muestra el valor
    display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    display.print("MEDIDOR de CO2");    // Escribe texto
    delay(10000); //demora 10 seg entre mediciones
    display.clear(); // borra pantalla  
  
// muestra indicación de que está calentando 
  display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("Calentando");    // Escribe primera linea del cartel
  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("Espere 1 minuto");  // Escribe segunda linea del cartel  
  delay(60000); // espera 1 minuto            
  display.clear(); // borra pantalla  

  tone(buzzer, 55,100); // pitido de 55Hz indicando que funciona normal, calentado
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
    tone(buzzer, 55,20); // pita para mostrar cartel
    display.clear(); // borra pantalla  
    display.setCursor(0,0);
    display.print("AireNuevo UNAHUR"); // Cada minuto muestra el valor
    display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    display.print("MEDIDOR de CO2");    // Escribe texto
    delay(5000); //demora 5 seg entre mediciones
    cnt = 0; // vuelve a empezar LOS LOOPS
  } else {

  }
  
  int co2ppm = mhz19.getPPM(); // mide CO2
  int temp = mhz19.getTemperature(); // mide temperatura

//  Muestra medición de CO2    
  display.clear(); // borra pantalla
  display.setCursor(0, 0); // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  display.print("CO2: ");    // Escribe texto
  display.setCursor(8, 0); // Ubicamos el cursor en la novena posición(columna:8) de la primera línea(fila:0)
  display.print(co2ppm); // Escribe CO2
  display.setCursor(12, 0); // Ubicamos el cursor en la treceava posición(columna:12) de la primera línea(fila:0)
  display.print("ppm"); // Escribe texto
  Serial.print (co2ppm);
  Serial.print("\n");
//  Muestra medición de temperatura
/*  display.setCursor(0, 1); // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  display.print("temp: ");    // Escribe texto
  display.setCursor(8, 1); // Ubicamos el cursor en la novena posición(columna:8) de la segunda línea(fila:1)
  display.print(temp); // Escribe temperatura
  display.setCursor(12, 1); // Ubicamos el cursor en la treceava posición(columna:12) de la segunda línea(fila:1)
  display.print("*C"); // Escribe texto
*/

//  Emite un sonido y escribe en función del resultado
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
       definirColor(255, 50, 0,anodoComun0); // PRENDE LED AMARILLO (pongo 50 en lugar de 255 para compensar ls diferencia de tensión en directa del diodo rojo)
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
   
  delay(10000); //demora 10 seg entre mediciones
}
