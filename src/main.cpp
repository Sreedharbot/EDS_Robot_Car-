#include <Arduino.h>
#include <Servo.h>

//global Variable
uint8_t old_state = 1;
uint8_t new_state = 0;
uint8_t but_value;
uint8_t activate;

uint8_t MAX_SPEED = 50;
float SOUND_SPEED = 0.034;       //speed of sound 343 m/s


//optical distance sensor pinout "OD"
uint8_t OD_1 = A0;
uint8_t OD_2 = A1;
uint8_t OD_3 = A2;

struct ODsensor{
  float OD1_data;
  float OD2_data;
  float OD3_data;
};


//ultra-sonic sensor pinout - "US"
uint8_t US_1_Trig = 12;
uint8_t US_1_Echo = A3;

uint8_t US_2_Trig = 11;
uint8_t US_2_Echo = A4;

uint8_t US_3_Trig = 10;
uint8_t US_3_Echo = A5;

struct USsensor{
  float US_1_data;
  float US_2_data;
  float US_3_data;
};

// Motor Control Pinouts - M1 , M2
uint8_t M1_P1 = 6;
uint8_t M1_P2 = 7;
uint8_t M2_P1 = 8;
uint8_t M2_P2 = 9;

// Servo Pinouts - S
uint8_t S1 = 14;
uint8_t S2 = 15;
uint8_t S3 = 5;

Servo servo_1;
Servo servo_2;
Servo servo_3;


// User Button pinout - B
uint8_t B = USER_BTN;

//Status LED 
uint8_t L = LED_BUILTIN;




//Optical Distance Sensor reading;
void ODS_Data(ODsensor *ods){
    uint16_t a = analogRead(OD_1);
    uint16_t b = analogRead(OD_2);
    uint16_t c = analogRead(OD_3);
    ods->OD1_data=a;
    ods->OD2_data=b;
    ods->OD3_data=c;

    // ods->OD1_data =map(a, 0,1023,0,80);
    // ods->OD2_data =map(b, 0,1023,0,80);
    // ods->OD3_data =map(c, 0,1023,0,80);

 }

 void US_Data(USsensor *us){
  
  //Sensor 1 Ultrasonic Sensor activation 
  digitalWrite(US_1_Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(US_1_Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(US_1_Trig,LOW);

  float time_1 = pulseIn(US_1_Echo,HIGH);
  float distance_1 = SOUND_SPEED*time_1;
  distance_1 = (time_1*SOUND_SPEED)/2;

  //Sensor 2 Ultrasonic Sensor activation 
  digitalWrite(US_2_Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(US_2_Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(US_2_Trig,LOW);

  float time_2 = pulseIn(US_2_Echo,HIGH);
  float distance_2 = SOUND_SPEED*time_1;
  distance_2 = (time_2*SOUND_SPEED)/2;

  //Sensor 3 Ultrasonic Sensor activation 
  digitalWrite(US_3_Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(US_3_Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(US_3_Trig,LOW);

  float time_3 = pulseIn(US_3_Echo,HIGH);
  float distance_3 = SOUND_SPEED*time_1;
  distance_3 = (time_3*SOUND_SPEED)/2;

  us->US_1_data = distance_1;
  us->US_2_data = distance_2;
  us->US_3_data = distance_3;


 }

 void Shoulder_down(){
  // servo_1.write(130);
  // delay(200);
  // servo_1.write(90);
  for(uint8_t a =0;a<=180;a++){
    servo_1.write(a);
    delay(100);
  }
 }

 void Shoulder_up(){
  // servo_1.write(50);
  // delay(200);
  // servo_1.write(90);
 }

 void elbow_down(){
  servo_2.write(130);
  delay(200);
  servo_2.write(90);
 }

 void elbow_up(){
  servo_2.write(50);
  delay(200);
  servo_2.write(90);
 }

  void shoulder_right(){
  servo_3.write(130);
  delay(200);
  servo_3.write(90);
 }

 void shoulder_left(){
  servo_3.write(50);
  delay(200);
  servo_3.write(90);
 }




 void Mot_forward(){
  analogWrite(M1_P1,0);
  analogWrite(M1_P2,MAX_SPEED);
  analogWrite(M2_P1,0);
  analogWrite(M2_P2,MAX_SPEED);

 }

 void Mot_left(){
  analogWrite(M1_P1,0);
  analogWrite(M1_P2,MAX_SPEED);
  analogWrite(M2_P1,0);
  analogWrite(M2_P2,0);

 }

void Mot_right(){
  analogWrite(M1_P1,0);
  analogWrite(M1_P2,0);
  analogWrite(M2_P1,0);
  analogWrite(M2_P2,MAX_SPEED);

 }


 void Mot_halt(){
  analogWrite(M1_P1,0);
  analogWrite(M1_P2,0);
  analogWrite(M2_P1,0);
  analogWrite(M2_P2,0);

 }

 
 void Mot_Revers(){
  analogWrite(M1_P1,MAX_SPEED);
  analogWrite(M1_P2,0);
  analogWrite(M2_P1,MAX_SPEED);
  analogWrite(M2_P2,0);

 }




 void autonomous(uint8_t act){
   struct ODsensor ODSA;
   struct USsensor USSA;

   while(act == 1){
      ODS_Data(&ODSA);
      US_Data(&USSA);
      Serial2.print(ODSA.OD1_data);
      Serial2.print("|");
      Serial2.print(ODSA.OD2_data);
      Serial2.print("|");
      Serial2.print(ODSA.OD3_data);
      Serial2.print("|");
      Serial2.print(USSA.US_1_data);
      Serial2.print("|");
      Serial2.print(USSA.US_2_data);
      Serial2.print("|");
      Serial2.println(USSA.US_3_data);


    if((USSA.US_1_data >= 10) || (USSA.US_2_data >= 10) || (USSA.US_3_data >= 10)){
        Mot_forward();
    }

    if((USSA.US_1_data <= 2) || (USSA.US_2_data <= 2) || (USSA.US_3_data <= 2)){
        Mot_halt();
    }

    if(USSA.US_3_data<5){
        Mot_left();
      }

    if(USSA.US_1_data<5){
        Mot_right();
      }
    
    if(USSA.US_2_data<5){
        Mot_halt();
    }

    if((ODSA.OD1_data>=900) && (ODSA.OD1_data<=1023)){
        Mot_halt();
        Mot_Revers();
    }

    if((ODSA.OD2_data>=900) && (ODSA.OD2_data<=1023)){
        Mot_halt();
        Mot_Revers();

    }

    if((ODSA.OD3_data>=900) && (ODSA.OD3_data<=1023)){
        Mot_halt();
        Mot_Revers();
    }

   }

 }


void setup() {

  Serial2.begin(9600);

  pinMode(B,INPUT_PULLUP);

  pinMode(OD_1,INPUT);
  pinMode(OD_2,INPUT);
  pinMode(OD_3,INPUT);

  pinMode(US_1_Echo,INPUT);
  pinMode(US_2_Echo,INPUT);
  pinMode(US_3_Echo,INPUT);

  pinMode(US_1_Trig,OUTPUT);
  pinMode(US_2_Trig,OUTPUT);
  pinMode(US_3_Trig,OUTPUT);

  pinMode(M1_P1,OUTPUT);
  pinMode(M1_P2,OUTPUT);
  pinMode(M2_P1,OUTPUT);
  pinMode(M2_P2,OUTPUT);

  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);

  pinMode(L,OUTPUT);

  servo_1.attach(S1);
  servo_2.attach(S2);
  servo_3.attach(S3);

 
//   Shoulder_down();
//   Shoulder_up();
//   elbow_down();
//   elbow_up();
//   shoulder_left();
//   shoulder_right();

  
}

void loop() {

  but_value = digitalRead(B);
  if (old_state==1 && but_value == 0){
     if(new_state==0){
        new_state = 1;
        activate = 1;
        digitalWrite(L,HIGH);
        autonomous(activate);
        
        }
        else{
        new_state = 0;
        digitalWrite(L,LOW);
      }
  
     
  }
   
old_state = but_value;



}

