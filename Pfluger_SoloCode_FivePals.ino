#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=60,55
AudioSynthWaveformModulated waveformMod1;   //xy=165,141
AudioSynthWaveformModulated waveformMod5;   //xy=165,303
AudioSynthWaveformModulated waveformMod2;   //xy=166,189
AudioSynthWaveformModulated waveformMod4;   //xy=166,263
AudioSynthWaveformModulated waveformMod3;   //xy=167,225
AudioAmplifier           amp5;           //xy=168,507
AudioAmplifier           amp6;           //xy=168,552
AudioAmplifier           amp4;           //xy=169,459
AudioAmplifier           amp7;           //xy=169,602
AudioAmplifier           amp3;           //xy=170,413
AudioMixer4              mixer3;         //xy=359,239
AudioMixer4              mixer2;         //xy=499,247
AudioMixer4              mixer4;         //xy=609,365
AudioFilterStateVariable filter1;        //xy=623,251
AudioAmplifier           amp1;           //xy=744,347
AudioAmplifier           amp2;           //xy=745,384
AudioEffectDelay         delay1;         //xy=759,116
AudioMixer4              mixer1;         //xy=762,246
AudioOutputUSB           usb1;           //xy=888,405
AudioOutputI2S           i2s1;           //xy=890,354
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(waveformMod1, 0, mixer2, 0);
AudioConnection          patchCord3(waveformMod1, amp7);
AudioConnection          patchCord4(waveformMod5, 0, mixer3, 3);
AudioConnection          patchCord5(waveformMod5, amp5);
AudioConnection          patchCord6(waveformMod2, 0, mixer3, 0);
AudioConnection          patchCord7(waveformMod2, amp6);
AudioConnection          patchCord8(waveformMod4, 0, mixer3, 2);
AudioConnection          patchCord9(waveformMod4, amp4);
AudioConnection          patchCord10(waveformMod3, 0, mixer3, 1);
AudioConnection          patchCord11(waveformMod3, amp3);
AudioConnection          patchCord12(amp5, 0, waveformMod4, 0);
AudioConnection          patchCord13(amp6, 0, waveformMod1, 0);
AudioConnection          patchCord14(amp4, 0, waveformMod3, 0);
AudioConnection          patchCord15(amp7, 0, waveformMod5, 0);
AudioConnection          patchCord16(amp3, 0, waveformMod2, 0);
AudioConnection          patchCord17(mixer3, 0, mixer2, 1);
AudioConnection          patchCord18(mixer2, 0, filter1, 0);
AudioConnection          patchCord19(mixer4, amp1);
AudioConnection          patchCord20(mixer4, amp2);
AudioConnection          patchCord21(filter1, 0, mixer1, 1);
AudioConnection          patchCord22(amp1, 0, i2s1, 0);
AudioConnection          patchCord23(amp1, 0, usb1, 0);
AudioConnection          patchCord24(amp2, 0, i2s1, 1);
AudioConnection          patchCord25(amp2, 0, usb1, 1);
AudioConnection          patchCord26(delay1, 0, mixer1, 3);
AudioConnection          patchCord27(mixer1, delay1);
AudioConnection          patchCord28(mixer1, 0, mixer4, 0);
AudioConnection          patchCord29(mixer1, 0, mixer4, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=70,20
// GUItool: end automatically generated code


//declare all sensor variables:
int knob1 = A10;
int knob2 = A11;
int knob3 = A12;
int knob4 = A13;
int knob5 = A14;
int knob6 = A15;
int knob7 = A16;
int knob8 = A17;
int knob9 = A18;
int slider1 = A21;
int slider2 = A22;
int switch1 = 16;
int switch2 = 17;

// setting up debounced Button Variables
Bounce button1 = Bounce(2, 10);
Bounce button2 = Bounce(3, 10);
Bounce button3 = Bounce(4, 10);
Bounce button4 = Bounce(5, 10);

// setting up LEDs
int LED1 = 20;
int LED2 = 21;
int LED3 = 38;
int LED4 = 39;
int LED = 25;

//setting up 2D array for "frequency quantization" to all major modes using ratios... Works by indexing (1) mode type AND (2) interval ratio from that mode. Later *'d by a "fundamental" called oscBaseFreq
                       // major                                          // dorian                                     //phrygian                                    
float quantArray[7][8]= {{1.0, 1.125, 1.25, 1.333, 1.5, 1.667, 1.875, 2}, {1.0, 1.125, 1.2, 1.333, 1.5, 1.6, 1.778, 2}, {1.0, 1.067, 1.2, 1.333, 1.5, 1.6, 1.778, 2}, 
//lydian                                        //mixo                                            //minor                                       //locrian
{1.0, 1.125, 1.25, 1.406, 1.5, 1.667, 1.875, 2},{1.0, 1.125, 1.25, 1.333, 1.5, 1.667, 1.778, 2}, {1.0, 1.125, 1.2, 1.333, 1.5, 1.6, 1.778, 2}, {1.0, 1.067, 1.2, 1.333, 1.406, 1.6, 1.778, 2}};

int scaleTracker = 0;
int inputPinArray[] = {2, 3, 4, 5, 16, 17};
int outputPinArray[] = {20, 21, 25, 38, 39};

// used to scale harmonic osc chords by certain ratios, starts as 1 BUT they will be later manipulated w/ the buttons...
float harmMult2 = 1.0;
float harmMult3 = 1.0;
float harmMult4 = 1.0;
float harmMult5 = 1.0;

void setup() {
  Serial.begin(9600);
  AudioMemory(600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(20);

  // setting up input pins with array help
    for (int i = 0; i < 6; i++){
      pinMode(inputPinArray[i], INPUT);
    }
  // setting up output pins with array help
  for (int j = 0; j < 5; j++){
      pinMode(outputPinArray[j], OUTPUT);
    }

  //initializing waveforms:
  waveformMod1.begin(1, 100, WAVEFORM_SQUARE);
  waveformMod2.begin(1, 200, WAVEFORM_SAWTOOTH);
  waveformMod3.begin(1, 300, WAVEFORM_SAWTOOTH);
  waveformMod4.begin(1, 400, WAVEFORM_SAWTOOTH);
  waveformMod5.begin(1, 500, WAVEFORM_SAWTOOTH);
  
  //inializing filter:
  filter1.frequency(440);
  filter1.resonance(1.0);

  // setting amps for modulation knot to 0.0 to begin (only will be heard based on switch position (see code)...
  amp3.gain(0.0);
  amp4.gain(0.0);
  amp5.gain(0.0);
  amp6.gain(0.0);
  amp7.gain(0.0);

  //next is initializing volume controls to be active on power on:
  amp1.gain(1);
  amp2.gain(1);
  mixer1.gain(0, 1); //mic (i2c) inlet
  mixer1.gain(1, 1); //master oscillator inlet
  mixer2.gain(0, 1); // osc 1 into osc mixer (mixer2)... bc of the # of oscillators, I had to break them up like this
  mixer2.gain(1, 1); // 4 oscs in from mixer3
  mixer3.gain(0, 1); // 1 of 4 harmony oscillators...
  mixer3.gain(1, 1); // 2 of 4 harmony oscillators...
  mixer3.gain(2, 1); // 3 of 4 harmony oscillators...
  mixer3.gain(3, 1); // 4 of 4 harmony oscillators...
  mixer4.gain(0, 1); // dry sig from all 5 oscs
  mixer4.gain(1, 1); // oscs through delay

  //initializing delay :)
 delay1.delay(0, 1000);
}

void loop() { // scaledReading = ((rawReading / 1023.0) * (scaleTop - scaleBottom)) + scaleBottom;
//loose variable declarations that will be used later:
float oscBaseFreq = pow((float)analogRead(knob2) / 1023.0, 2) * (500.0 - 50.0) + 50.0; // MACRO 1... being used to control fundamental oscialltor and base freq of 4 other (before they are mult'd by harmMult variables (see button logic for elaboration..)
float harmonicOscLevs = pow((float)analogRead(slider1) / 1023.0, 2); // harmonicOscLevs (MACRO 2) used for all 4 "harmonic oscillator" volumes
float cutoff = pow((float)analogRead(slider2) / 1023.0, 2) * (5000.0 - 50.0) + 50.0;
float modDepth = ((float)analogRead(slider2) / 1023.0) * (0.707 - 0.0) + 0.0; // MACRO 3 note that slider 2 is being used to control the filter cutoff AND the moddepth (only when switch 1 is flipped ip) see code below...
float delFdbck = ((float)analogRead(knob7) / 1023.0);
float oscFundVol = pow((float)analogRead(knob4) / 1023.0, 2);
int tonalityFlip = digitalRead(switch2);

// scales array work...
int quantIndex1 = map(analogRead(knob3), 0, 1023, 0, 6); // index 1 used to select mode. Being mapped in order to appropriately meet array size requirement
// the following if/else is being used to give quick visual feedback on the LED under "knob3" when the active mode changes
if (scaleTracker != quantIndex1){ 
  digitalWrite(LED, HIGH);
  delay(100); // quick delay so you can see the LED flash, but not too long so that it doesn't meaningfully interupt any other processes
  digitalWrite(LED, LOW);
}
else{} // useless else just for safety, one time I heard someone say "always add an else" in a lecture, so now I'm scared not to

scaleTracker = quantIndex1;
int quantIndex2 = map(analogRead(knob6), 0, 1023, 0, 7);
float scaleScrub = quantArray[quantIndex1][quantIndex2];

// filter for oscillators
filter1.frequency(cutoff);

//osc 1 control:
waveformMod1.frequency(oscBaseFreq * scaleScrub);

//manging harmonic oscillator manipulations: buttons control ratio changes (used to mult by fun base freq), switch 2 used as tonality flip (for relevant intervals) (UP = MAJOR, DOWN = MINOR)
// updates the status of all 4 buttons to see if there's been a change
button1.update();
button2.update();
button3.update();
button4.update();

// button 1 + switch up = Maj Triad
// button 1 + switch down = Min Triad
// button 2 + switch up = Maj 7
// button 2 + switch down = Min 7
// button 3 + switch up = Maj Triad + Maj6
// button 3 + switch down = Min Traid + Maj 6
// button 4 + switch up = Maj Triad + 6 + 9
// button 4 + switch down = Min Triad + Maj6 + 9


            if (button1.risingEdge() && tonalityFlip == 1){
            harmMult2 = 1.5; // fifth
            harmMult3 = 1.25; //major third
            harmMult4 = 0.0; // nothin, being used to silence osc
            harmMult5 = 2.0; //octave
            
            digitalWrite(LED1, HIGH); 
            digitalWrite(LED2, LOW); 
            digitalWrite(LED3, LOW);  
            digitalWrite(LED4, LOW);
          }
           
           if (button1.risingEdge() && tonalityFlip == 0){
            harmMult2 = 1.5; // fifth
            harmMult3 = 1.2; //minor third
            harmMult4 = 0.0; // nothin, being used to silence osc
            harmMult5 = 2.0; //octave
            
            digitalWrite(LED1, HIGH); 
            digitalWrite(LED2, LOW); 
            digitalWrite(LED3, LOW);  
            digitalWrite(LED4, LOW);
          }
          
          
            if (button2.risingEdge() && tonalityFlip == 1){
            harmMult2 = 1.5; // fifth              
            harmMult3 = 1.25; //major third
            harmMult4 = 1.875; // maj 7(leading tone)          
            harmMult5 = 2.0; //octave
            
            digitalWrite(LED1, LOW); 
            digitalWrite(LED2, HIGH); 
            digitalWrite(LED3, LOW);  
            digitalWrite(LED4, LOW);
          }

           if (button2.risingEdge() && tonalityFlip == 0){
            harmMult2 = 1.5; // fifth              
            harmMult3 = 1.2; //minor third
            harmMult4 = 1.778; // b7     
            harmMult5 = 2.0; //octave
            
            digitalWrite(LED1, LOW); 
            digitalWrite(LED2, HIGH); 
            digitalWrite(LED3, LOW);  
            digitalWrite(LED4, LOW);
          }
          
            if (button3.risingEdge() && tonalityFlip == 1){
            harmMult2 = 1.5; //fifth
            harmMult3 = 1.25; //major third
            harmMult4 = 1.66666666667; // maj sixth
            harmMult5 = 2.0; // octave
            
            digitalWrite(LED1, LOW); 
            digitalWrite(LED2, LOW); 
            digitalWrite(LED3, HIGH);  
            digitalWrite(LED4, LOW);
           }

           if (button3.risingEdge() && tonalityFlip == 0){
            harmMult2 = 1.5; //fifth
            harmMult3 = 1.2; //minor third
            harmMult4 = 1.66666666667; // maj sixth 
            harmMult5 = 2.0; // octave
            
            digitalWrite(LED1, LOW); 
            digitalWrite(LED2, LOW); 
            digitalWrite(LED3, HIGH);  
            digitalWrite(LED4, LOW);
           }
         
           if (button4.risingEdge() && tonalityFlip == 1){ 
            harmMult2 = 1.5; // P5
            harmMult3 = 1.25; //major third
            harmMult4 = 1.66666666667; // maj sixth 
            harmMult5 = 2.25; // 9th
            
            digitalWrite(LED1, LOW); 
            digitalWrite(LED2, LOW); 
            digitalWrite(LED3, LOW);  
            digitalWrite(LED4, HIGH);
          }
          
           if (button4.risingEdge() && tonalityFlip == 0){ 
            harmMult2 = 1.5; // P5
            harmMult3 = 1.2; //minor third
            harmMult4 = 1.66666666667; // maj sixth 
            harmMult5 = 2.25; // 9th
            
            digitalWrite(LED1, LOW); 
            digitalWrite(LED2, LOW); 
            digitalWrite(LED3, LOW);  
            digitalWrite(LED4, HIGH);
          }

waveformMod2.frequency(oscBaseFreq * harmMult2);
waveformMod3.frequency(oscBaseFreq * harmMult3);
waveformMod4.frequency(oscBaseFreq * harmMult4);
waveformMod5.frequency(oscBaseFreq * harmMult5);

//feedback modulation "knot"
if(digitalRead(switch1) == 1){
  amp3.gain(modDepth);
  amp4.gain(modDepth);
  amp5.gain(modDepth);
  amp6.gain(modDepth);
  amp7.gain(modDepth);
}
else{
  amp3.gain(0.0);
  amp4.gain(0.0);
  amp5.gain(0.0);
  amp6.gain(0.0);
  amp7.gain(0.0);
}
          
//microphone mixer inlet volume:
mixer1.gain(0, ((float)analogRead(knob5)/1023.0)); // data scaling in function call for ease. Didn't feel need to declare a seperate variable

mixer1.gain(3, delFdbck); //controlling delay Feedback amount using an inlet to mixer 1, looped back into delay

//oscillator mixer vol:
mixer2.gain(0, oscFundVol * 0.25); 

//4 stacked oscillator levels (from harmonicOscLevs (MACRO 2))...
mixer3.gain(0, harmonicOscLevs * 0.1);
mixer3.gain(1, harmonicOscLevs * 0.1);
mixer3.gain(2, harmonicOscLevs * 0.1);
mixer3.gain(3, harmonicOscLevs * 0.1);

// last is MAIN volume controls. A master and also an individual L and R control...
float masterVolScaler = ((float)analogRead(knob1) / 1023.0);
float vol_l = ((float)analogRead(knob8) / 1023.0);
float vol_r = ((float)analogRead(knob9) / 1023.0);
amp1.gain(vol_l * masterVolScaler);
amp2.gain(vol_r * masterVolScaler);



}
