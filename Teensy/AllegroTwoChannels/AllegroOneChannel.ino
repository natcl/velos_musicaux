const byte ledPin = 13; 
const byte sensorPin1 = 2;
const byte sensorPin2 = 3;

byte min_rpm = 0;
byte max_rpm = 150;

// Timers
elapsedMillis rpm_cycle_timer1;
elapsedMillis interpol_timer1;

elapsedMillis rpm_cycle_timer2;
elapsedMillis interpol_timer2;


// Rpm
int nbtours1 = 0;
int nbtours2 = 0;
int interval1 = 500; //default 500
int interval2 = 500; //default 500
const int moyenne_fenetre1 = 10; //default 10
const int moyenne_fenetre2 = 10; //default 10
int moyenne1[moyenne_fenetre1];
int moyenne_compteur1 = 0;
int moyenne_somme1;
int moyenne2[moyenne_fenetre1];
int moyenne_compteur2 = 0;
int moyenne_somme2;
float vitesse_inst1;
float vitesse_moyenne1;
float delta1 = 0;
int fast_interval1 = 10;
float continuous_val1 = 0;
float vitesse_inst2;
float vitesse_moyenne2;
float delta2 = 0;
int fast_interval2 = 10;
float continuous_val2 = 0;

void setup() {                
  Serial.begin(115200);
  
  // Hall sensor
  pinMode(sensorPin1, INPUT_PULLUP);
  pinMode(sensorPin2, INPUT_PULLUP);
  attachInterrupt(sensorPin1, compteur1, FALLING);
  attachInterrupt(sensorPin2, compteur2, FALLING);

  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop()                     
{
  if (digitalRead(sensorPin1) == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
  } 
  else 
  {
    digitalWrite(ledPin, LOW);
  }
  
  
  //Sensor 1
  if (rpm_cycle_timer1 > interval1)
  {
    rpm_cycle_timer1 = 0;
    moyenne_somme1 = 0;

    if (moyenne_compteur1 == moyenne_fenetre1 - 1)
      moyenne_compteur1 = 0;
    moyenne1[moyenne_compteur1] = nbtours1;
    moyenne_compteur1++;
    
    vitesse_inst1 = nbtours1  * 60000 / interval1;
    //Serial.print("Vitesse instantanee: ");
    //Serial.println(vitesse_inst);
    
    
    for (int i = 0; i < moyenne_fenetre1 ; i++)
    {
      moyenne_somme1+= moyenne1[i];
    }
    vitesse_moyenne1 = moyenne_somme1 * 60000 / interval1 / moyenne_fenetre1;
    //Serial.print("Vitesse moyenne: ");
    //Serial.println(vitesse_moyenne1);

    delta1 = vitesse_moyenne1 - continuous_val1;

    nbtours1 = 0;
  }

  if (interpol_timer1 > fast_interval1)
  {
    interpol_timer1 = 0;
    continuous_val1 = continuous_val1 + delta1 / (interval1 / fast_interval1);
    if (continuous_val1 < 0)
      continuous_val1 = 0;
    Serial.print("Capteur1: ");
    Serial.println(continuous_val1);
  }


  //Sensor 2
  if (rpm_cycle_timer2 > interval2)
  {
    rpm_cycle_timer2 = 0;
    moyenne_somme2 = 0;

    if (moyenne_compteur2 == moyenne_fenetre2 - 1)
      moyenne_compteur2 = 0;
    moyenne2[moyenne_compteur2] = nbtours2;
    moyenne_compteur2++;
    
    vitesse_inst2 = nbtours2  * 60000 / interval2;
    //Serial.print("Vitesse instantanee: ");
    //Serial.println(vitesse_inst);
    
    
    for (int i = 0; i < moyenne_fenetre2 ; i++)
    {
      moyenne_somme2+= moyenne2[i];
    }
    vitesse_moyenne2 = moyenne_somme2 * 60000 / interval2 / moyenne_fenetre2;
    //Serial.print("Vitesse moyenne: ");
    //Serial.println(vitesse_moyenne2);

    delta2 = vitesse_moyenne2 - continuous_val2;

    nbtours2 = 0;
  }

  if (interpol_timer2 > fast_interval2)
  {
    interpol_timer2 = 0;
    continuous_val2 = continuous_val2 + delta2 / (interval2 / fast_interval2);
    if (continuous_val2 < 0)
      continuous_val2 = 0;
    //Serial.print("Capteur2: ");
    //Serial.println(continuous_val2);
  }
}

void compteur1()
{ 
  nbtours1++;
}

void compteur2()
{ 
  nbtours2++;
}

