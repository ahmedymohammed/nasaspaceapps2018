//arduino code
/*the IDE of the arduino will not compile this code until you download the libraries mintioned in the code ,, the first library is "dht" library and the second is 
"Servo" library .. if you use the code without downloading libraries , the IDE will show compiling errors*/
#include <Servo.h>      //the Servo library
#include "dht.h"        //the humidty sensor library
const int trigPin = 8;  // trig pin of ultrasonic  
const int echoPin = 13; // echo pin of ultrasonic
long duration;          // the time taken by ultrawaves from the emmiting side to the receving side
int distance;           // the distance travelled by the ultrawaves
#define dht_apin A2     // the analog pin of the humidty sensor (because it depends on resistance)
#define gasanalog A3    // the analog pin of the gas sensor (because it depends on resistance)
#define capPin A5       // the first analog receving pin of the metal detector
#define pulsePin A4     // the second analog receving pin of the metal detecor 
#define led 10          // the OUTPUT of the Led of the metal detector 
#define in1  2          // the first pin controlling with the direction of the right motor 
#define in2  3          // the second pin controling with the direction of the right motor
#define in3  4          // the first pin controlling with the direction of the left motor
#define in4  5          // the second pin controling with the direction of the left motor
#define buzzer 9        // the pin controling the buzzer of metal detector
int buz = 12;           // the pin controling the buzzer of the (gas sensor, humidty sensor and moisture sensor)
#define moist A0        // the pin receving the analog read of the moisture sensor because it woks as a variable resistance 
int moistureread;       // the variable taking it's value form the pin "A0" especially the pin called "moist"
int deg;                // the degree of rotation of moisture servo

long sumExpect=0;       // the first variable responsibel for metal detector calculation
long ignor=0;           // the second variable responsibel for metal detector calculation
long diff=0;            // the third variable responsibel for metal detector calculation
long pTime=0;           // the fourth variable responsibel for metal detector calculation
long buzPeriod=0; 




dht DHT;                   //function from the library "dht.h" that actives the humidty/tempreature sensor
Servo servo;               //function from the library "Servo.h" to define the first servo 
Servo moisture;            //function from the library "Servo.h" to define the second servo 
void setup () {
  
  pinMode (trigPin, OUTPUT);  //set the pin "trigpin" to be an output for the data
  pinMode (echoPin, INPUT);   //set the pin "echopin" to be an input for the data
  pinMode (in1, OUTPUT);      //set the pin "in1" to be an output to give HIGH or LOW signals from the arduino to the H-Bridge control the direction of rotation for the motor 
  pinMode (in2, OUTPUT);      //set the pin "in2" to be an output to give HIGH or LOW signals from the arduino to the H-Bridge control the direction of rotation for the motor 
  pinMode (in3, OUTPUT);      //set the pin "in3" to be an output to give HIGH or LOW signals from the arduino to the H-Bridge control the direction of rotation for the motor 
  pinMode (in4, OUTPUT);      //set the pin "in4" to be an output to give HIGH or LOW signals from the arduino to the H-Bridge control the direction of rotation for the motor 
  pinMode(pulsePin,OUTPUT);   //set the pin of the pulse of the metal detector to give pulses to the metal detector by the arduino
  pinMode(capPin, INPUT);     //set the capacitor pin of the metal detector to be an input to read the capacitor by the arduino analog pins
  pinMode(led, OUTPUT);       //set the led pin resposible for the metal detector to be an output
  
  digitalWrite(pulsePin, LOW); //set the pulse pin of the metal detector to LOW in a kind of saftey step to get the accurate reading
  digitalWrite(buzzer, LOW);   //set the buzzer pin of the metal detector to LOW to stop it from buzzing if it was buzzing befor restarting the arduino
  
  Serial.begin(9600);          //start the serial number (9600) so the rate of transferring data to the arduino is 9600 bits/second 
  servo.attach(11);            //attach the servo of the ultrasonic in the pin 11
  moisture.attach(7);          //attach the servo of the moisture sensor in the pin 7
  pinMode(buzzer, OUTPUT);     //set the mode of the buzzer of the metal detector to be an output 
  pinMode(buz, OUTPUT);        //set the mode of the other sensors (gas sensor, humidty sensor adn smoiture sensor) to be an output 
}

void loop () {

    measure();                 //A defined function below that measures the distance using ultrasonic and put the value in the variable called "distance" declared above...
                               // first robot turning condition
                               
    if(distance<25){                           //if the distance in front of the robot was less than 25 cms, the ultrasonic will turn right and see if the robot can turn right or no    
        for(deg=90 ; deg<=180 ; deg+=1){       //a for function responsible for the movment of the servo holding the ultrasonic to right
          servo.write(deg);                    //giving the servo the values of the counting for function to turn right
          delay(15); }                         //delay for the servo to prevent it from turning very fast 
    measure();                                 //A defined function below that measures the distance using ultrasonic and put the value in the variable called "distance" declared above...
        if(distance>25){               //if the distance in the right dirction was more than 25 cms .. then the robot will turn right
          digitalWrite(in1, HIGH);     //giving the "in" pins of a motor HIGH and LOW values makes the motor rotating in specific direction
          digitalWrite(in2, LOW);      //for exmaple if a motor was given a HIGH, LOW values respectively according to my connections, the motor will move forward
          digitalWrite(in3, LOW);      //here the first motor will move forward becuase it's given HIGH, LOW values respectively ...
          digitalWrite(in4, HIGH);}    //and the second motor will move backwards because it's given LOW, HIGH values respectivley.. and the net movment of the robot will be turning right

        if(distance<25){                  //if the distance in the right direction was less than 25 cms, the ultrasonic will turn left and see if the robot can turn right or no
        for(deg=180 ; deg>=0 ; deg-=1){   //a for function responsible for the movment of the servo holding the ultrasonic to left
          servo.write(deg);               //giving the servo the values of the counting for function to turn left
          delay(15);}                     //delay for the servo to prevent it from turning very fast 
    measure();                            //A defined function below that measures the distance using ultrasonic and put the value in the variable called "distance" declared above...
    
        for(deg=0 ; deg<=90 ; deg+=1){    //a for function responsible for the movment of the servo holding the ultrasonic to the standard position
          servo.write(deg);               //giving the servo the values of the counting for function to go to the standard position
          delay(15);}                     //delay for the servo to prevent it from turning very fast 
        if(distance>25){
          digitalWrite(in1, LOW);    //giving the "in" pins of a motor HIGH and LOW values makes the motor rotating in specific direction
          digitalWrite(in2, HIGH);   //for exmaple if a motor was given a HIGH, LOW values respectively according to my connections, the motor will move forward
          digitalWrite(in3, HIGH);   //here the first motor will move backwards becuase it's given LOW, HIGH values respectively ...
          digitalWrite(in4, LOW);}   //and the second motor will move forward because it's given HIGH, LOW values respectivley.. and the net movment of the robot will be turning left
        if(distance<25){
          digitalWrite(in1, LOW);    //giving the "in" pins of a motor HIGH and LOW values makes the motor rotating in specific direction
          digitalWrite(in2, HIGH);   //for exmaple if a motor was given a HIGH, LOW values respectively according to my connections, the motor will move forward
          digitalWrite(in3, LOW);    //here the first motor will move backwards becuase it's given LOW, HIGH values respectively ...
          digitalWrite(in4, HIGH);}  //and the second motor will move backwards because it's given LOW, HIGH values respectivley.. and the net movment of the robot will be moving back..
                    }} 
       else {
        digitalWrite(in1, HIGH);     //giving the "in" pins of a motor HIGH and LOW values makes the motor rotating in specific direction
        digitalWrite(in2, LOW);      //for exmaple if a motor was given a HIGH, LOW values respectively according to my connections, the motor will move forward
        digitalWrite(in3, HIGH);     //here the first motor will move forward becuase it's given HIGH, LOW values respectively ...
        digitalWrite(in4, LOW);}     //and the second motor will move forward because it's given HIGH, LOW values respectivley.. and the net movment of the robot will be moving forward
       
    
    DHT.read11(dht_apin);                     //function in the library of the humity/tempreature sensor that reads the humidty and temp.
    Serial.print("Current humidity = ");      //function called "print" is used to display the wards "Current humidity = " in the serial monitor
    Serial.print(DHT.humidity);               //function called "print" is used to display the determined humidty in the serial monitor
    Serial.println("%  ");                    //this function "println" is a function used also to display something in the serial monitor and taking new line after pinting..
    Serial.print("temperature = ");           //the same as the functions explained above..
    Serial.print(DHT.temperature);            //the same as the functions explained above..
    Serial.println("C  ");                    //the same as the functions explained above..
    
      if (DHT.humidity>=10){             //if the relative humidty was more than 10% ... 
      digitalWrite(buz, HIGH);}          //the buzzer will make a sound
      delay(1000);                       //this delay is made to determin the humidty, if the humidty was high, the buzzer will make sound, but because 3 sensors work on the same 
                                         //-same buuzer, if the humidty was high but the gas read wasn't high, the buuzer will not make sound for more than some microseconds...
      if(DHT.humidity<10){               //if the relative humidty was less than 10% ... 
      digitalWrite(buz, LOW);}           //the buzzer will stop making sound

      int gasread = analogRead(gasanalog);   //declaring the variable called gas read that takes it's value form the analog read in the pin A3
      Serial.print("gas read = ");           //printing the words "gas read =" on the serial monitor
      Serial.println(gasread);               //printing the value of the gas read on the serial monitor
     
      if(gasread >=50) {                //if the gas read was above 50 and hydrogen or ch4 or propane or carbon monoxide were detected.. 
      digitalWrite(buz, HIGH);}         //the buzzer will make sound
      delay(1000);                      //this delay is made to determin the gasread, if the gasread was high, the buzzer will make sound, but because 3 sensors work on the same
                                        //-same buuzer, if the gasread was high but the moisture read wasn't high, the buuzer will not make sound for more than some microseconds...
      if(gasread< 50){                   //if the gasread was less than 50 ... 
      digitalWrite(buz, LOW);}           //the buzzer will stop making sound

      for (deg=0 ; deg<= 180 ; deg = deg+1){    //for function responsible for the moisture sensor servo movment
      moisture.write(deg);                      //giving the moisture servo the value of the variable "deg" to let it turn
      delay(15);}                               //delay for the servo to prevent it from turning very fast 

      moistureread=analogRead(moist);           //giving the variable called moistureread the analog read value of the pin A0
      Serial.println(moistureread);             //a "println" function used to display the read of the moisture sensor and taking new line after printing
      
      if(moistureread>900){                     //if the moisture read was more than 900 *this value may seem huge compared to the normal moist of mars but the moisture sensor was used
      digitalWrite(buz, HIGH);}                 //-used without the lm393 compiling chip... so it's very sensitive for humidty, of the humidty was just a little higher than normal  
                                                //-normal .. the buzzer will make sound
      if(moistureread<900){                     //if the moisture read was less than 900, the buzzer will stop making sound
      digitalWrite(buz, LOW);}                  //here in the moisture part no delay was used because the buzzer will have long time to make sound before the code is repeated

      for (deg=180; deg>=0; deg=deg+1){         //this for function is responsible for making the servo turn back to it's standard position 
      moisture.write(deg);}                     // the end of the code ^-^
      
 /* ***************************************************************************** the metal detector part ***************************************************************************** */
  int minval=1023;
  int maxval=0;
  long unsigned int sum=0;
  
    for (int i=0; i<256; i++){        //this is roughly long "For Function" .... nessecary for the metal detector ...
    //reset the capacitor            
    pinMode(capPin,OUTPUT);          
    digitalWrite(capPin,LOW);        
    delayMicroseconds(20);          
    pinMode(capPin,INPUT);           
    applyPulses();                   
                                     //read the charge of capacitor
    int val = analogRead(capPin);    //takes 13x8=104 microseconds
    minval = min(val,minval);
    maxval = max(val,maxval);
    sum+=val;
    
    long unsigned int cTime=millis();
    char buzState=0;
    
     if (cTime<pTime+10){
     if (diff>0)
     buzState=1;
     else if(diff<0)
     buzState=2;}
   
     if (cTime>pTime+buzPeriod){
     if (diff>0)
     buzState=1;
     
     else if (diff<0)
     buzState=2;
     pTime=cTime; }  
   
     if (buzPeriod>300)
     buzState=0;

      if (buzState==0){
      digitalWrite(led, LOW);
      noTone(buzzer);}
      
      else if (buzState==1){
      tone(buzzer,2000);
      digitalWrite(led, HIGH);}
    
      else if (buzState==2){
      tone(buzzer,500);
      digitalWrite(led, HIGH);}
    }                                 //the end of the long for function ...
  
  sum-=minval;                       //subtract minimum and maximum value to remove spikes
  sum-=maxval;
  
  if (sumExpect==0) 
  sumExpect=sum<<6; //set sumExpect to expected value
  long int avgsum=(sumExpect+32)>>6; 
  diff=sum-avgsum;
  
    if (abs(diff)<avgsum>>10){
    sumExpect=sumExpect+sum-avgsum;
    ignor=0;}
   
    else 
    ignor++;
    if (ignor>64){
    sumExpect=sum<<6;
    ignor=0;}
  
    if (diff==0) 
    buzPeriod=1000000;
    else 
    buzPeriod=avgsum/(2*abs(diff));    
}

 /* ***************************************************************************** the metal detector part ***************************************************************************** */

void measure(){
digitalWrite(trigPin, LOW);           //set the trigger pin of the ultrasonic to LOW in a kind of satey step to ensure that the reading of the echo will be right 
delayMicroseconds(2);                 //micro-seconed delay fot the ultrasonic
digitalWrite(trigPin, HIGH);          //set the trigger pin to HIGH to send the ultrasignals 
delayMicroseconds(10);                //micro-seconed delay to give the ultrasonic the suitable time to send the ultrawaves properly
digitalWrite(trigPin, LOW);           //set the trigger pin to LOW after the ultrawaves are sent
duration = pulseIn(echoPin, HIGH);    //use the pulseIn function to determine the time took by the ultrawaves to reflect on an obsacle and go back to the echo side  
distance= duration*0.034/2;           //calculating the distance taken by the ultrawaves using the law "distance=time*speed" in the terms of the speed of ultrawaves (340 meters/second)
Serial.print("Distance: ");           //printing in the serial monitor the word "distance" to print the distance determined by the ultrasonic after it
Serial.println(distance);             //printing the distance and taking new line after the printing the value to avoid the interferance of the multiple data
delay(100);                           //giving the arduino small rest after measuring the distance
}
void applyPulses(){

 for (int i=0;i<3;i++) {
  digitalWrite(pulsePin,HIGH); //take 3.5 uS
  delayMicroseconds(3);
  digitalWrite(pulsePin,LOW);  //take 3.5 uS
  delayMicroseconds(3);}}
