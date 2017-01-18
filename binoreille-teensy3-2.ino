#include <Audio.h> 
#include <Wire.h> 
#include <SPI.h> 
#include <SD.h> 
#include <SerialFlash.h> 

// GUItool: begin automatically generated code 
AudioFilterBiquad biquad2; //xy=375.3333435058594,250 
AudioInputI2S i2s2; //xy=578.3333435058594,157.00000762939453 
AudioFilterBiquad biquad1; //xy=730.0833129882812,169.08334350585938 
AudioOutputI2S i2s1; //xy=891.3333740234375,134 
AudioConnection patchCord1(i2s2, 0, biquad1, 0); 
AudioConnection patchCord2(biquad1, 0, i2s1, 0); 
AudioConnection patchCord3(biquad1, 0, i2s1, 1); 
AudioControlSGTL5000 sgtl5000_1; //xy=361.3333740234375,203 
// GUItool: end automatically generated code 

const int myInput = AUDIO_INPUT_MIC; 

void setup() { 
 Serial.begin(115200); 
 // Audio connections require memory to work. For more 
 // detailed information, see the MemoryAndCpuUsage example 
 AudioMemory(12); 

 // Enable the audio shield, select input, and enable output 
 sgtl5000_1.enable(); 
 sgtl5000_1.volume(0.5); 
 sgtl5000_1.inputSelect(myInput); 
 sgtl5000_1.micGain(36); 

 // paramétrage des filtres passe bas et passe haut : fixes, pour un confort permanent 
 biquad1.setHighpass(0,300,0.3); 
 biquad1.setLowpass(1,12000,0.1); 

} 

// mesure du temps écoulé, cf. https://www.pjrc.com/teensy/td_timing_elaspedMillis.html 
elapsedMillis volmsec; 

void loop() { 


 // every 50 ms, adjust the volume, frequency and filter with 
 float vol = analogRead(0); 
 float frq = analogRead(1); 
 float with = analogRead(2); 

 if (volmsec > 500) { 
 Serial.print("raw volume : "); 
 Serial.println(vol); 
 Serial.print("raw freq : "); 
 Serial.println(frq); 
 Serial.print("raw width : ");
 Serial.println(with); 
 }

 vol = vol / 1023.0; 
 frq = frq * 20.0; 
 with = (with / 1023.0) / 1.3; 

 if (volmsec > 500) { 
 Serial.print("Vol : "); 
 Serial.println(vol); 
 Serial.print("Frq : "); 
 Serial.println(frq); 
 Serial.print("with : "); 
 Serial.println(with); volmsec = 0; 
 } 

 // réglage du volume de sortie 
 sgtl5000_1.volume(vol,vol); 

 // réglage du filtre passe-bande selon les positions des potentiomètres 
 biquad1.setBandpass(2,frq,with); 
}
