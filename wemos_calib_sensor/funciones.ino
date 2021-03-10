//-----------Led-RGB----------------------
void rgb(char color) {
  switch (color) {
    case 'r':
      digitalWrite(pinLedR, 0);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 255);
      break;
    case 'g':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 0);
      digitalWrite(pinLedB, 255);
      break;
    case 'y':
      digitalWrite(pinLedR, 0);
      digitalWrite(pinLedG, 0);
      digitalWrite(pinLedB, 255);
      break;
    case 'b':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 0);
      break;
  }
}
//-----------------Alarma----------------------------
/*
void alarma(int veces, int duracionNota) {
  for(int i=0; i<veces; i++)
  {
    tone(pinBuzzer, 523, duracionNota);   // Hago sonar el buzzer, 523 corresponde a la nota C5
    delay(duracionNota);                  // Espero lo que dura la nota
    noTone(pinBuzzer);                    // Silencio el buzzer
    delay(duracionNota);                  // Delay entre alarmas
  }
}*/
//-----------Print-por-display----------------------
void displayPrint(int posicion, int linea, String texto) {
  display.setCursor(posicion, linea);       // Ubicamos el cursor en la posicion y linea deseada
  display.print(texto);                     // Escribe primera linea del cartel
}
void imprimirCO2(int co2ppm, int temp) {
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
}
//-----------Calibración----------------------
/*
void calibrar()
{
  const long segundosEspera = 1800;      // Cantidad de segundos a esperar
  long segundosPasados = 0;              // Cuenta segundos
  // Print por serial
  Serial.print("COMIENZA CALIBRACION \n");
  // Print por display
  display.clear();
  displayPrint(0, 0, "COMIENZA");
  displayPrint(0, 1, "CALIBRACION");
  delay(10000); // Espera 10 segundos
 
  while(segundosPasados <= segundosEspera) { // espera media hora
    if (++segundosPasados % 60 == 0) {                           // Si cnt es múltiplo de 60
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
      displayPrint(0, 0, "CALIBRANDO");
      displayPrint(0, 1, String(segundosPasados / 60));           // Ubicamos el cursor en la primera posición(columna:0) de la primer línea(fila:0) y escribimos los minutos pasados
      displayPrint(7, 1, "minutos");
    }
    delay(1000); // Espera 1 segundo
  }
  sensor.calibrateZero();               // Calibra
  // Print por serial
  Serial.print("PRIMERA CALIBRACION \n");
  // Print por display
  display.clear();                      // Limpio pantalla
  displayPrint(0, 0, "PRIMERA");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "CALIBRACION");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) 
  alarma(1, 250);
  delay(60000);                         // Espera 1 minuto
  sensor.calibrateZero();               // Calibra por segunda vez por las dudas
  // Print por serial
  Serial.print("SEGUNDA CALIBRACION \n");
  // Print por display
  display.clear();                      // Limpio pantalla     
  displayPrint(0, 0, "SEGUNDA");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "CALIBRACION");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) 
  alarma(1, 250);
  delay(10000); // Espera 10 segundos
  // Print por serial
  Serial.print("CALIBRACION TERMINADA \n");
  // Print por display
  display.clear(); // borra pantalla  
  displayPrint(0, 0, "CALIBRACION");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "TERMINADA");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  alarma(5, 250);
  delay(10000); // Espera 10 segundos 
}*/
