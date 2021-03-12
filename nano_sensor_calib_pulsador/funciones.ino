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
