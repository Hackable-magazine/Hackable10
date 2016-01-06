#include <TimerOne.h>

// variable pour l'état de la led
int etatLED = LOW;

void setup() {
  // Led sur la carte en sortie
  pinMode(LED_BUILTIN, OUTPUT);
  // Configuration du timer
  // un déclenchement toutes les 100000 microseconde
  //Timer1.initialize(100000);
  Timer1.initialize();
  // Lors d'un déclenchement une interruption est 
  // générée et cette fonction est appelée
  //Timer1.attachInterrupt(cliLED);
  Timer1.attachInterrupt(cliLED,100000);
}

// fonction appelée lors de l'interruptio
void cliLED() {
  // On inverse l'état
  // Si LOW alors HIGH, si HIGH alors LOW
  etatLED = !etatLED;
  // changement d'état de la led
  digitalWrite(LED_BUILTIN, etatLED);
}

void loop() {
  // rien ici
}
