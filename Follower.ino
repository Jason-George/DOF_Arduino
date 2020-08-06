unsigned long tme; //kepp track of time every 700 milliseconds
//volatile keyword is used for variables accessed or changed during an interrupts
volatile boolean left_int, front_int,right_int; 
volatile uint8_t buf[10];
volatile uint8_t counter;  

ISR(INT0_vect) //Interrupts code for LEFT
{

  cli(); //disables global interrupts
  left_int = true;
  buf[counter]=1; //1 = left (1 is arbitatry)
  counter++;
  delayMicroseconds(450); //This delay so that the entire 8 or so pulses passes, without triggering an interrupt
 }



ISR(INT1_vect)  //Interrupts code for FRONT
{

  cli(); //disables global interrupts
 front_int = true;
 buf[counter]=2; //2 = front ( 2 is arbitatry)
 counter++;
 delayMicroseconds(450); //This delay so that the entire 8 or so pulses passes, without triggering an interrupt
 
}

ISR(INT2_vect)  //Interrupts code for RIGHT
{

  cli(); //disables global interrupts
  right_int = true;
  buf[counter]=3; //1 = left (3 is arbitatry)
  counter++;
  delayMicroseconds(450); //This delay so that the entire 8 or so pulses passes, without triggering an interrupt
  
}

void setup() {

 cli(); //disables Global interrupts
 Serial.begin(115200);
 tme=0;
 counter=0; //for buffer
 
 //Initalises direction boolean
 left_int=false;
 right_int=false;
 front_int = false;

//set Interrupts pin to receive interrupts
 pinMode(21,INPUT);
 pinMode(20,INPUT);
 pinMode(19,INPUT);

 //Motor Control Logic
 pinMode(32,OUTPUT);
 pinMode(33,OUTPUT);
 pinMode(34,OUTPUT);
 pinMode(35,OUTPUT);

//Set all pins low, so the motors aren't triggered 
 digitalWrite(32,LOW);
 digitalWrite(33,LOW);
 digitalWrite(34,LOW);
 digitalWrite(35,LOW);
  
//Set up Interrupts
EICRA =   0b00111111; //INT0,1,2 - rising edge; type of interrupt enables= rising edge 
EIMSK |= (1<<INT0) | (1<<INT1)|(1<<INT2); //INT0,1,2 is enabled on pins 19,20 & 21
sei(); //Enable Global Interrupts 
 
}

void loop() {

if ((buf[0] == 1 || buf[1]==1) && left_int) { //if buffer fills up with ones after the left interrupt has occured, then it is left

       
      digitalWrite(32,LOW);
      digitalWrite(33,LOW);
      digitalWrite(34,LOW);
      digitalWrite(35,HIGH); //LEFT forward
      tme=millis(); //Take reading so that after 700 ms passes, all motors stop
      
       //Serial.println("L"); //for debugging
      left_int=false;
      counter=0; //set counter to 0 so next interrupts fills buffer from 0
      
      sei(); //Enable Global interrupts again so the next interrupts takes place
      
    }

    else if  ((buf[0]==2 || buf[1]==2) && front_int) {  //if buffer fills up with twos after the front interrupt has occured, then it is left
      
      digitalWrite(32,LOW);
      digitalWrite(33,HIGH);
      digitalWrite(34,LOW);
      digitalWrite(35,HIGH);
      tme=millis(); //Take reading so that after 700 ms passes, all motors stop
     
      front_int=false;
      
      //Serial.println("F"); //for debugging
      
      counter=0; //set counter to 0 so next interrupts fills buffer from 0
      sei(); //Enable Global interrupts again so the next interrupts takes place

   }

    else  if ((buf[0]==3 || buf[1]==3 || buf[2]==3) && right_int) {  //if buffer fills up with three after the right interrupts has occured, then it is left
   
      digitalWrite(32,LOW);
      digitalWrite(33,HIGH);
      digitalWrite(34,LOW);
      digitalWrite(35,LOW);
      tme=millis(); //Take reading so that after 700 ms passes, all motors stop
      
      //Serial.println("R"); //for debugging
     
      right_int = false;
      counter=0; //set counter to 0 so next interrupts fills buffer from 0
      sei(); //Enable Global interrupts again so the next interrupts takes place
    }

    if (millis()-tme >=700) { //every 700 milliseconds stop all the motors, so that the follower doesn't go haywire in all directions

      digitalWrite(32,LOW);
      digitalWrite(33,LOW);
      digitalWrite(34,LOW);
      digitalWrite(35,LOW);
      tme=millis();  //Take present milliseconds reading for the next loop
    }

  
    
}
