#include <SoftwareSerial.h> //library used to simulate UART in pin 8 & 9 as the usual serial pins 0 & 1 is used to upload the code

const char trig_pin = 10; //Pin to trigger Ulthrasound Sensor
unsigned long tme;

SoftwareSerial mySerial(8,9);  //rx=8,tx=9
bool stop_pulse; //to switch off/on ultrasound pulse 

void process_Data(byte val)
{
  switch (val) {
  case 50: //Front
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    break;
  case 52: //Left
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    break;
  case 51: //Right
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    break;
  case 53: //Back
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    break;
  case 54: //Stop
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    break;
  case 55: //Stop/Start Pulse
     if(stop_pulse == false) 
     {
      stop_pulse=true;
     }
     else {
       
      stop_pulse=false;
     }
    break;

}
}

void setup() {
  // initialize serial port:
  Serial.begin(115200); //to display on the Computer; 115200 = BAUD rate
   // initialize software serial port for Bluwtooth receiver
  mySerial.begin(9600); //9600=BAUD rate
     
  tme=0; //tme is used to check elasped time
 
 //boolean to switch off & on Pulses
  stop_pulse=true;

  //Set 13 Pin Inbuilt LED as Output
  pinMode(13,OUTPUT); 
  
  pinMode(trig_pin,OUTPUT); //Set the Trigger pin as output
  
  //Set 4 pins as Output for the Motor Drive
 pinMode(2,OUTPUT); //left forward
 pinMode(3,OUTPUT); //left backward
 pinMode(4,OUTPUT); //right forward
 pinMode(5,OUTPUT);//right backward
 
  
}

void loop() {


if (!stop_pulse && (millis()-tme)>=1000) { //once every 400 milliseconds this loop is executed
digitalWrite(trig_pin,HIGH);  //trigger pin is set for 10 microseconds to send the Ultrasound pulse
delayMicroseconds(10);
digitalWrite(trig_pin,LOW); 
tme= millis(); //records amount of milliseconds that have passed since the start of the program
delay(5);//5 milliseconds delay
}



if (mySerial.available()>0) {  //Check if data is available in the Buffer
   
   byte in_char = mySerial.read();  //If yes, read it
   if (in_char < 60)process_Data(in_char); //Send it to control the motor

   //Otherwise control the inBuilt LED
   if (in_char == 69) digitalWrite(13,HIGH);
   if (in_char == 70) digitalWrite(13,LOW);
 }

}

