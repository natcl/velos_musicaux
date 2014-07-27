/* Pushbutton with Pullup, Teensyduino Tutorial #3
   http://www.pjrc.com/teensy/tutorial3.html

   This example code is in the public domain.
*/
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Bounce.h>

AudioPlaySdWav     wav;
AudioOutputI2S     dac;

AudioConnection c1(wav, 0, dac, 0);
AudioConnection c2(wav, 1, dac, 1);

AudioControlSGTL5000 audioShield;

// Pins
const byte syncPin = 3; 
const byte playPin =  4;

const byte nextButtonPin = 6;
const byte ledPin = 13; 
const byte sensorPin = 5;

Bounce nextButton = Bounce(nextButtonPin, 100);

// Audio control
byte max_volume = 70;
byte min_volume = 30;
float final_volume = 0.;
byte min_rpm = 0;
byte max_rpm = 150;

const char *tounes[] = {"toune1.wav", "toune2.wav", "toune3.wav"};
byte currentSong = 0;

// Timers
elapsedMillis playback_timer;
elapsedMillis rpm_cycle_timer;
elapsedMillis interpol_timer;


// Rpm
int nbtours = 0;
int interval = 500;
const int moyenne_fenetre = 10;
int moyenne[moyenne_fenetre];
int moyenne_compteur = 0;
int moyenne_somme;
float vitesse_inst;
float vitesse_moyenne;
float delta = 0;
int fast_interval = 10;
float continuous_val = 0;

void setup() {                
  Serial.begin(115200);
  
  // Hall sensor
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(sensorPin, compteur, FALLING);

  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  // Buttons
  pinMode(nextButtonPin, INPUT_PULLUP);
  // Sync and play
  pinMode(syncPin, OUTPUT);
  pinMode(playPin, INPUT);
  attachInterrupt(playPin, playWavSync, RISING);

  AudioMemory(5);

  audioShield.enable();
  audioShield.volume(min_volume);

  SPI.setMOSI(7);
  SPI.setSCK(14);
  SD.begin(10);
}

void loop()                     
{
  nextButton.update();
  if (nextButton.fallingEdge())
  {
    playNextSong();
  }

  if (digitalRead(sensorPin) == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
  } 
  else 
  {
    digitalWrite(ledPin, LOW);
  }
  
  if (rpm_cycle_timer > interval)
  {
    rpm_cycle_timer = 0;
    moyenne_somme = 0;

    if (moyenne_compteur == moyenne_fenetre - 1)
      moyenne_compteur = 0;
    moyenne[moyenne_compteur] = nbtours;
    moyenne_compteur++;
    
    vitesse_inst = nbtours  * 60000 / interval;
    //Serial.print("Vitesse instantanee: ");
    //Serial.println(vitesse_inst);
    
    
    for (int i = 0; i < moyenne_fenetre ; i++)
    {
      moyenne_somme+= moyenne[i];
    }
    vitesse_moyenne = moyenne_somme * 60000 / interval / moyenne_fenetre;
    //Serial.print("Vitesse moyenne: ");
    Serial.println(vitesse_moyenne);

    delta = vitesse_moyenne - continuous_val;

    nbtours = 0;
  }

  if (playback_timer > 1000)
  {
    playback_timer = 0;
    if (!wav.isPlaying())
    {
      Serial.println("SyncPinHigh");
      digitalWrite(syncPin, HIGH);
    }
    
  }

  if (interpol_timer > fast_interval)
  {
    interpol_timer = 0;
    continuous_val = continuous_val + delta / (interval / fast_interval);
    if (continuous_val < 0)
      continuous_val = 0;
    final_volume = constrain(map(continuous_val, min_rpm, max_rpm, min_volume, max_volume), min_volume, max_volume) / 100.;
    //Serial.println(final_volume);
    audioShield.volume(final_volume);

    float vol = analogRead(15);
    vol = vol / 10.24;
    min_volume = vol;

  }
}

void compteur()
{ 
  nbtours++;
}

void playNextSong()
{
  Serial.println("PlayNextSong");
  currentSong++;
  if (currentSong == 3)
    currentSong = 0;
  Serial.println(currentSong);
  //wav.stop();
  wav.play(tounes[currentSong]);
}

void playWavSync()
{
  Serial.println("PLayWavSync");
  wav.play(tounes[currentSong]);
  digitalWrite(syncPin, LOW);
}
