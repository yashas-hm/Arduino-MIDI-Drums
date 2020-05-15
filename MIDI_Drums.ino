#include<MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

//Arduino's analog input
const int pad[7] = {A0, A1, A2, A3, A4, A5, A6};

//Volume threshold variable, bieng used to determine
//if user strikes the drum pad
const int volumeThreshold = 500;

//Variable for volume magnitude of each drum pad
int DrumVal [7]= {0, 0, 0, 0, 0, 0, 0};

//variables used in program logic
//variables used in proces of separation of vibrtion from
//exact force impact
int signalTest[7]={0, 0, 0, 0, 0, 0, 0};


//midi mapping - addictive drums mapping
/*byte CrashCymbal = 49;  
byte RideCymbal = 51;
byte SnareDrum = 38;
byte HighTom = 50;
byte MidTom = 47;
byte LowTom = 45;
byte kick = 36; 
*/
byte Drum[7] ={ 49, //CrashCymbal
                51, //RideCymbal
                38, //SnareDrum
                50, //HighTom
                47, //MidTom
                45, //LowTom
                36  //Kick
              };
              
void setup(){
  MIDI.begin(4);
  //starts serial communication between 
  //Arduino and serial USB port
  Serial.begin(115200);
}

void loop(){
  for(int i=0; i<7; i++)
  { //if magnitude of analoginput was higher than volumeThreshold
    //program checks again if the signal is decreasing
    //if so, it means that drum pad was hit
    if(analogRead(pad[i]) >= volumeThreshold) 
      signalTest[i] = 1;
    if(signalTest[i] == 1 && analogRead(pad[i]) <= (volumeThreshold-100))
    {
      //sets the velocity
      DrumVal[i]= analogRead(pad[i]);
      int velocity = DrumVal[i]/3 + 78;
      if(velocity >= 120)
        velocity = 125;
      else if(velocity < 0)
        velocity = 0;
  
      //sends midi signal over USB serial port
      MIDI.sendNoteOn(Drum[i], velocity, 1);
      MIDI.sendNoteOff(Drum[i], 0, 1);
      //sets signalTest again to 0
      signalTest[i] = 0;
    }
  } 
}
