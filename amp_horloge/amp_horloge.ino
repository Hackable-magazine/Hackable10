#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#define HEURES  6
#define MINUTES 5

void setup() {
  // les deux broches en sortie
  pinMode(HEURES, OUTPUT);
  pinMode(MINUTES, OUTPUT);
}

void loop() {
  // pour le stockage de l'heure
  tmElements_t tm;

  // puis-je lire l'heure ?
  if(RTC.read(tm)) {
    // oui
    // Si l'heure est entre 0 et 11
    if(tm.Hour < 12)
      // on utilise directement la valeur
      analogWrite(HEURES, 255/12*tm.Hour);
    else
      // sinon on est l'après midi et on retranche 12
      analogWrite(HEURES, 255/12*(tm.Hour-12));
    // On utilise la valeur pour les minutes directement
    analogWrite(MINUTES, 255/60*(tm.Minute));
  } else {
    // On en peut pas lire l'heure
    if (RTC.chipPresent()) {
      // heure n'est pas valide
      // il faut d'abord régler l'horloge avec le croquis
      // d'exemple TimeRTCSet
    } else {
      // problème avec DS1307
      // il faut vérifier les connexion
    }
  }
  // pause de 5s
  // il n'est pas utilise de rafraîchir l'heure plus souvent
  delay(5000);
}
