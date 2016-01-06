#include <TimerOne.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// RX sur 3, TX sur 2
SoftwareSerial softSerial(3, 2);
Adafruit_GPS GPS(&softSerial);

unsigned long previousMillis = 0;
#define PAUSE 2000

// fonction appelé lors de l'interruption
void lectureGPS() {
  char c = GPS.read();
}

// Affichage d'une valeur sur deux chiffres
void print2digits(int val, Stream &console) {
  // si <10 alors on préfixe d'un 0
  console.print(val < 10 ? "0" : "");
  console.print(val, DEC);
}

void setup(){    
  Serial.begin(115200);
  GPS.begin(4800);

  // une interruption à chaque milliseconde
  Timer1.initialize(1000);
  Timer1.attachInterrupt(lectureGPS);
  
  delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();

  // On a un message NMEA
  if (GPS.newNMEAreceived()) {
    // S'il n'est pas lisible
    if (!GPS.parse(GPS.lastNMEA()))
      // on abandonne
      return;
  }

  // toutes les PAUSE millisecondes
  if(currentMillis - previousMillis >= PAUSE) {
    previousMillis = currentMillis;

    // on affiche les données de base
    Serial.print("\nHeure GMT: ");
    print2digits(GPS.hour,Serial); Serial.print(':');
    print2digits(GPS.minute,Serial); Serial.print(':');
    print2digits(GPS.seconds,Serial);
    Serial.println("");
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.println((int)GPS.fix >0 ? "OUI" : "NON");
    // Si le récepteur GPS a pu calculer une position on
    // affiche plus d'informations
    if (GPS.fix) {
      Serial.print("Qualite: "); Serial.println(GPS.fixquality);
      Serial.print("HDOP: "); Serial.println(GPS.HDOP,2);
      Serial.print("Precision: "); Serial.print(GPS.HDOP*4,2); Serial.println("m");
      Serial.print("Position: ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
      
      Serial.print("Vitesse: "); 
      Serial.print(GPS.speed,2); Serial.print(" noeud(s)     ");
      Serial.print((GPS.speed*1.852),2); Serial.println(" km/h");
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Nbr Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
}
