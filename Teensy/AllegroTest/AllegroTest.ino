/* Pushbutton with Pullup, Teensyduino Tutorial #3
   http://www.pjrc.com/teensy/tutorial3.html

   This example code is in the public domain.
*/

elapsedMillis timer;
long nbtours = 0;
int nbtours_prev;
int interval = 100;
const int moyenne_fenetre = 10;
int moyenne[moyenne_fenetre];
int moyenne_compteur = 0;
int moyenne_somme;
float vitesse_inst;
int vitesse_moyenne;


void setup() {                
  Serial.begin(38400);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(2, compteur, FALLING);
  pinMode(13, OUTPUT);
}

void loop()                     
{
  if (digitalRead(2) == HIGH) 
  {
    digitalWrite(13, LOW);
  } 
  else 
  {
    digitalWrite(13, HIGH);
  }
  if (timer > interval)
  {
    if (moyenne_compteur == moyenne_fenetre - 1)
      moyenne_compteur = 0;
    Serial.println(nbtours - nbtours_prev);
    moyenne[moyenne_compteur] = nbtours - nbtours_prev;
    moyenne_compteur++;
    timer = 0;
    vitesse_inst = (nbtours - nbtours_prev) * 60000 / interval;
    Serial.print("Vitesse instantanee: ");
    Serial.println(vitesse_inst);
    
    moyenne_somme = 0;
    for (int i = 0; i < moyenne_fenetre ; i++){
      moyenne_somme+= moyenne[i];
    }
    vitesse_moyenne = moyenne_somme * 60000 / interval / moyenne_fenetre;
    usbMIDI.sendControlChange(1,vitesse_moyenne, 1);
    Serial.print("Vitesse moyenne: ");
    Serial.println(vitesse_moyenne);
    nbtours_prev = nbtours;

  }
}
void compteur()
{ 
  nbtours++;
}

