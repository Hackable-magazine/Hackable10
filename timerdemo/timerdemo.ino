#include <TimerOne.h>
// ajouts des fonction utilitaire avr-libc
#include <util/atomic.h>

int etatLED = LOW;
volatile unsigned int compteur = 42000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
  Timer1.initialize(100000);
  Timer1.attachInterrupt(cliLED);
}

void cliLED() {
  etatLED = !etatLED;
  digitalWrite(LED_BUILTIN, etatLED);
  compteur--;
}

void loop() {
  // un nouvelle variable pour la copie
  // du compteur
  unsigned int compteurCopie;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    // on copie la valeur de compteur
    // ceci ne peut être interrompu
    compteurCopie = compteur;
  }

  // on test la copie
  if(compteurCopie == 0)
    digitalWrite(4, HIGH);
}
