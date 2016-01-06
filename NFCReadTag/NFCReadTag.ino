#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

// lecteur en SPI avec /CS sur 10
PN532_SPI pn532spi(SPI, 10);
// Le lecteur est ce nouvel objet
NfcAdapter nfc = NfcAdapter(pn532spi);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Lecteur NDEF");
  // Initialisation et affichage du lecteur détecté
  nfc.begin();
}

void loop(void) {  
  Serial.println("\nApprochez un tag NFC");

  // Si ne détecte aucun tag on s'arrête là
  while(!nfc.tagPresent()) {
    delay(5);
    return;
  }

  // Un tag est trouvé
  Serial.println("Je vois un tag ");
  // Lecture
  NfcTag tag = nfc.read();
  // test si enregistrement NDEF présent
  if (tag.hasNdefMessage()) {
    Serial.println("contenant du NDEF");
    // récupération du contenu NDEF
    NdefMessage message = tag.getNdefMessage();
    // S'il y a au moins un message
    if (message.getRecordCount() > 0) {
      Serial.println("avec au moins un message NDEF");
      // On récupère le premier enregistrement
      NdefRecord record = message.getRecord(0);
      // Nous ne voulons que des types bien-connus
      if(record.getTnf()==TNF_WELL_KNOWN) {
        Serial.println("ayant le TNF \"bien-connu\"");
        // et que des URIs
        if(record.getType()=="U") { // URL
          Serial.println("type URI");
          // récupération de la taille du contenu 
          int payloadLength = record.getPayloadLength();
          // variable pour le contenu
          byte payload[payloadLength];
          // récupération du contenu
          record.getPayload(payload);
          // Fabrication d'un objet String de caractères
          String payloadAsString = "";
          // Le vrai contenu commence à la position 1, non 0
          // A 0, il y a le code identifiant de l'URI
          for (int i = 1; i < payloadLength; i++) {
            payloadAsString += (char)payload[i];
          }
          // Le premier correspond à une URI file:// ?
          if(payload[0]==0x1D ) {
            Serial.print("file://");
            Serial.println(payloadAsString);
            int port = payloadAsString.substring(0,payloadAsString.indexOf('/')).toInt();
            int val = payloadAsString.substring(payloadAsString.indexOf('/')+1).toInt();
            pinMode(port, OUTPUT);
            digitalWrite(port, val);
            
          // si non, on s'intéresseur aussi aux URI personnalisés
          } else if(payload[0]==0x00) {
            Serial.println(payloadAsString);
          }
        }
      }
    }
  }
  delay(1000);
}
