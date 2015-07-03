const byte ledPin = 13; 
const byte sensorPin = 2;

byte min_rpm = 0;
byte max_rpm = 150;

// Timers
elapsedMillis rpm_cycle_timer;
elapsedMillis interpol_timer;

// Rpm
const int nbMagnets = 1 ; //default 1
const int interval = 400; //default 500 400
const int mean_window = 4; //default 10 4
int nbRotations = 0;
int mean[mean_window];
int mean_counter = 0;
int mean_sum;
float instant_speed;
float mean_speed;
float delta = 0;
int interpol_interval = 10; //default 10
float continuous_val = 0;

void setup() {                
  Serial.begin(115200);
  
  // Hall sensor
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(sensorPin, counter, FALLING);

  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop()                     
{
  if (digitalRead(sensorPin) == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
  } 
  else 
  {
    digitalWrite(ledPin, LOW);
  }
  
  
  //Sensor 1
  if (rpm_cycle_timer > interval)
  {
    rpm_cycle_timer = 0;
    mean_sum = 0;

    if (mean_counter == mean_window - 1)
      mean_counter = 0;
    mean[mean_counter] = nbRotations;
    mean_counter++;
    
    // Nombe de tours par minute (instantané)
    instant_speed = nbRotations  * 60000 / interval / nbMagnets;
    //Serial.print("Vitesse instantanee: ");
    //Serial.println(instant_speed);
    
    // Calcul de la vitesse mean
    for (int i = 0; i < mean_window ; i++)
    {
      mean_sum+= mean[i];
    }
    mean_speed = mean_sum * 60000 / interval / mean_window / nbMagnets;
    //Serial.print("Vitesse mean: ");
    //Serial.println(mean_speed);

    delta = mean_speed - continuous_val;

    nbRotations = 0;
  }

  if (interpol_timer > interpol_interval)
  {
    interpol_timer = 0;
    continuous_val = continuous_val + delta / (interval / interpol_interval);
    if (continuous_val < 0)
      continuous_val = 0;
    Serial.print("Capteur1: ");
    Serial.println(continuous_val);
  }
}

void counter()
{ 
  nbRotations++;
}


