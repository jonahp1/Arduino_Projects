#include <Bounce.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=716.2500114440918,122.75000190734863
AudioEffectBitcrusher    bitcrusher1;    //xy=779.9999313354492,207.77778816223145
AudioFilterStateVariable filter1;        //xy=853.7500133514404,122.50000190734863
AudioEffectDelay         delay1;         //xy=881.7500114440918,367.5000343322754
AudioFilterStateVariable filter3;        //xy=912.2222290039062,214.44445419311523
AudioMixer4              mixer4;         //xy=1006.2500152587891,341.25000381469727
AudioMixer4              mixer1;         //xy=1010.4166793823242,129.3611183166504
AudioAmplifier           amp1;           //xy=1182.222240447998,538.8888702392578
AudioEffectFade          fade1;          //xy=1182.5000190734863,502.50000762939453
AudioMixer4              mixer2;         //xy=1216.2499160766602,359.861141204834
AudioEffectGranular      granular1;      //xy=1218.7500228881836,420.0000057220459
AudioMixer4              mixer3;         //xy=1305.0000228881836,522.5000076293945
AudioFilterStateVariable filter2;        //xy=1370.972152709961,365.9722099304199
AudioAmplifier           amp2;           //xy=1492.2224578857422,354.44445419311523
AudioOutputUSB           usb1;           //xy=1624.7778129577637,394.4722099304199
AudioOutputI2S           i2s1;           //xy=1627.444492340088,353.1111078262329
AudioConnection          patchCord1(i2s2, 0, filter1, 0);
AudioConnection          patchCord2(bitcrusher1, 0, filter3, 0);
AudioConnection          patchCord3(filter1, 0, mixer1, 0);
AudioConnection          patchCord4(delay1, 0, mixer4, 0);
AudioConnection          patchCord5(delay1, 0, bitcrusher1, 0);
AudioConnection          patchCord6(delay1, 1, mixer4, 1);
AudioConnection          patchCord7(delay1, 2, mixer4, 2);
AudioConnection          patchCord8(filter3, 0, mixer1, 3);
AudioConnection          patchCord9(mixer4, fade1);
AudioConnection          patchCord10(mixer4, 0, mixer2, 0);
AudioConnection          patchCord11(mixer4, amp1);
AudioConnection          patchCord12(mixer1, delay1);
AudioConnection          patchCord13(amp1, 0, mixer3, 1);
AudioConnection          patchCord14(fade1, 0, mixer3, 0);
AudioConnection          patchCord15(mixer2, 0, filter2, 0);
AudioConnection          patchCord16(granular1, 0, mixer2, 1);
AudioConnection          patchCord17(mixer3, granular1);
AudioConnection          patchCord18(filter2, 0, amp2, 0);
AudioConnection          patchCord19(amp2, 0, i2s1, 0);
AudioConnection          patchCord20(amp2, 0, i2s1, 1);
AudioConnection          patchCord21(amp2, 0, usb1, 0);
AudioConnection          patchCord22(amp2, 0, usb1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=725.25,87.5
// GUItool: end automatically generated code



#define GRANULAR_MEMORY_SIZE 12800
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

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

// setting up LEDs, 1-4 = button LEDs, 5-8 bottom knob row LEDs
int LED1 = 20;
int LED2 = 21;
int LED3 = 38;
int LED4 = 39;
int LED5 = 28;
int LED6 = 29;
int LED7 = 30;
int LED8 = 8;

// setting up debounced Button Variables
Bounce button1 = Bounce(2, 10);
Bounce button2 = Bounce(3, 10);
Bounce button3 = Bounce(4, 10);
Bounce button4 = Bounce(5, 10);

int inputPinArray[] = {2, 3, 4, 5, 16, 17};
int outputPinArray[] = {8, 20, 21, 28, 29, 30, 38, 39};

void setup() {
  Serial.begin(9600);
  AudioMemory(700);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(20);

// setting up input pins with array help
        for (int i = 0; i < 6; i++){
          pinMode(inputPinArray[i], INPUT);
        }
 // setting up output pins with array help
        for (int j = 0; j < 8; j++){
          pinMode(outputPinArray[j], OUTPUT);
        }
  
  mixer1.gain(0, 0.707);
  mixer1.gain(3, 0.0);
  delay1.delay(0, 300);
  delay1.delay(1, 500);
  delay1.delay(2, 700);

  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);

  filter1.frequency(500.0);
  filter2.frequency(500.0);

  mixer2.gain(0, 1.0);
  mixer2.gain(1, 0.0);


  mixer3.gain(0, 1);
  mixer3.gain(1, 1);
  mixer3.gain(1, 1);

  mixer4.gain(0, 1);
  mixer4.gain(1, 1);
  mixer4.gain(2, 1);
  mixer4.gain(3, 1);

  amp1.gain(0.0);
  
  digitalWrite(LED1, HIGH);
  granular1.beginPitchShift(100.0); // pitch shift set up, 100ms grains. 

}

void loop() { 
      button1.update();
      button2.update();
      button3.update();
      button4.update();

   if (button1.risingEdge()){
      delay1.delay(0, 100);
      delay1.delay(1, 250);
      delay1.delay(2, 500);
      
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
    }
    if (button2.risingEdge()){
      delay1.delay(0, 500);
      delay1.delay(1, 750);
      delay1.delay(2, 1000);
      
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
    }
     if (button3.risingEdge()){
      delay1.delay(0, 1000);
      delay1.delay(1, 1150);
      delay1.delay(2, 2000);
      
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
    }
    if (button4.risingEdge()){
      delay1.disable(0);
      delay1.disable(1);
      delay1.disable(2);
      
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
    }

    // if/else pair uses switch 1 to  allow input from mic to pass into effects... 1 =  audbile, 0 = no input from mic. Mixer input vol being used to manage this
    if (digitalRead(switch1) == 1){
      mixer1.gain(0, 1);
    }
    else{
      mixer1.gain(0, 0);
    }

    // scaledReading = ((rawReading / 1023.0) * (scaleTop - scaleBottom)) + scaleBottom;
    //pow((rawReading / 1023.0), 2.0) * (scaleTop - scaleBottom)) + scaleBottom;
    // just setting up some variables to be used later in code... scaling them as well either to 0.0-1.0 or using the formulas above
    float dryVol = (((float)analogRead(knob5)) / 1023.0);
    float inputCutoffLog = pow((float)analogRead(slider1) / 1023.0, 2) * (5000.0 - 50.0) + 50.0;
    float outputCutoffLog = pow((float)analogRead(slider2) / 1023.0, 2) * (5000.0 - 50.0) + 50.0;
    float LPFInFdbck = pow((float)analogRead(knob4) / 1023.0, 2) * (5000.0 - 50.0) + 50.0;


    float fdbck = ((float)analogRead(knob9)) / 1023.0;

    float granVol = ((float)analogRead(knob3)) / 1023.0;
    float ratio = (((float)analogRead(knob2)) / 1023.0 * (2.0 - 0.125)) + 0.125;
    float tremSpeed = (((float)analogRead(knob6)) / 1023.0 * (200.0 - 10.0)) + 10.0;
    float masterVol = ((float)analogRead(knob1)) / 1023.0;
    
    int bitDegrade = map(analogRead(knob7), 0, 1023, 1, 16);
    int sampDegrade = map(analogRead(knob8), 0, 1023, 1, 44100);


    if (digitalRead(switch2) == 1){
    // kinda crude (but functional!) tremolo effect... switch is being used as an effect on/off. amp1 manages dry sig, fade1 used to provide trem effect
    amp1.gain(0.0);
    digitalWrite(LED5, HIGH);
    fade1.fadeIn(tremSpeed);
    delay(tremSpeed);
    fade1.fadeOut(tremSpeed);
    delay(tremSpeed);
    }
    else{
    amp1.gain(1.0);
    digitalWrite(LED5, LOW);

    }
    
    granular1.setSpeed(ratio);
    mixer2.gain(0, dryVol); // "dry" volume control from delay (i.e. NO GRANULAR!)
    mixer2.gain(1, granVol); //granular volume control
    
    mixer1.gain(3, fdbck); //delay feedback control (looped back around through mixer 1, channel 3)
    filter3.frequency(LPFInFdbck);
    bitcrusher1.bits(bitDegrade);
    if (bitDegrade == 16){
      digitalWrite(LED6, HIGH);
    }
    else{
      digitalWrite(LED6, LOW);
    }
    bitcrusher1.sampleRate(sampDegrade);
    if (sampDegrade == 44100){
      digitalWrite(LED7, HIGH);
    }
    else{
      digitalWrite(LED7, LOW);
    }

    // next if/else if/else are just a little user interface piece... when the delay feedback is above 0.8 the LED under knob 9 will light up, otherwise it'll be LOW
    if (fdbck >= 0.8){
      digitalWrite(LED8, HIGH);
    }
    else if (fdbck < 0.8){
      digitalWrite(LED8, LOW);
    }
    else{}

    filter1.frequency(inputCutoffLog); //filter cutoff control for input signal
    filter2.frequency(outputCutoffLog); //filter cutoff control for output signal 

    amp2.gain(masterVol);
}
