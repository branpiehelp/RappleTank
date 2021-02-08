#include<SoftwareSerial.h>
//Bluetooth
int Rx=6;
int Tx=5;
SoftwareSerial btSerial(Tx,Rx);
//Command
String my="";
int val;
//Ultrasonic
int distance;
int d;
//DC Motor
int L_SPEED, R_SPEED;
int cnt=0;
//Servo Motor
#define servoPin 9
float servo_angle=1450;
float old_servo_angle=1450;
float d_angle=0;
bool servo_angle_change=false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(13,OUTPUT); //Left Motor Enable HIGH
  pinMode(12,OUTPUT); //Right Motor Enable LOW
  pinMode(11,OUTPUT); //Left Motor PWM Minimum 30
  pinMode(10,OUTPUT); //Right Motor PWM Minimum 30
  pinMode(7, OUTPUT); //Trig
  pinMode(8, INPUT);  //Echo
}

void loop() {
  // put your main code here, to run repeatedly:
  servo_handler();
  distance=ultraSonic();
  if(distance<10){
    stop();
  }
  my="";
  
  while(btSerial.available())
  {
    char myChar = (char)btSerial.read();
    my+=myChar;
    delay(5);
  }

    if(my.length() > 0){
      my.replace("\n","");
      my.replace("\r",""); 
      int num; 
      Serial.println(my);
      if(my == "f"){
        Serial.println("forward");
        forward();
        }
      else if(my == "s"){
        Serial.println("stop");
        L_SPEED = 0;
        R_SPEED = 0;
        cnt=0;
        }
      else if(my == "r"){
        Serial.println("right");
        right();
        }
      else if(my == "l"){
        Serial.println("left");
        left();
        }
      else if(my == "b"){
        Serial.println("backward");
        backward();
        }
      else if(my == "cannon"){
        sscanf(my.c_str(),"cannon %d",&num);
        cannon_angle(num);
        } 
      Serial.println(cnt);
      }
    analogWrite(11, L_SPEED);
    analogWrite(10, R_SPEED);
}
void forward(){
  if(cnt==0){     // move slow
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);
    L_SPEED = 70;
    R_SPEED = 70;
    cnt=1;
  }
  else{         // move fast
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);
    L_SPEED = 255;
    R_SPEED = 255;
    cnt=2;
  }
  
}

void backward(){
  if(cnt==0){     // move slow
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    L_SPEED = 70;
    R_SPEED = 70;
    cnt=1;
  }
  else{         // move fast
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    L_SPEED = 255;
    R_SPEED = 255;
    cnt=2;
  }
}

void left(){
  if(cnt==0 or cnt==1){ // rotate in place
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    L_SPEED = 70;
    R_SPEED = 70;
    cnt=0;
  }
  else if(cnt==2){    // turn left
    L_SPEED = 70;
    cnt=1;
  }
}

void right(){
  if(cnt==0 or cnt==1){ // rotate in place
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);
    L_SPEED = 70;
    R_SPEED = 70;
    cnt=0;
  }
  else if(cnt==2){    // turn right
    R_SPEED = 70;
    cnt=1;
  }
}
void stop(void)
{
    analogWrite(11, 0);
    analogWrite(10, 0);
}
int ultraSonic(void){
  digitalWrite(7, HIGH);
  delayMicroseconds(10);
  digitalWrite(7, LOW);
  val=pulseIn(8, HIGH);
  d=val*340/2/10000;
  return d;
}
void cannon_angle(int a)
{
  servo_angle=map(a,0,180,600,2300);
  Serial.println(old_servo_angle);
  Serial.println(servo_angle);
  servo_angle_change=true;  
}
void servo_handler()
{
    // A pulse each 20ms
  if(servo_angle_change==true){
    d_angle=old_servo_angle-servo_angle;
    if(abs(d_angle)<=200){
      d_angle*=0.01;
      if(abs(d_angle)<=50){
        servo_angle_change=false;
        old_servo_angle=servo_angle;
        Serial.println(old_servo_angle);
        
      }
    }
    else{
      d_angle*=0.1;
    }
    old_servo_angle=old_servo_angle-d_angle;
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(old_servo_angle); // Duration of the pusle in microseconds
    digitalWrite(servoPin, LOW);
    delayMicroseconds(18550); // 20ms - duration of the pusle
    // Pulses duration: 600 - 0deg; 1450 - 90deg; 2300 - 180deg
  }
}
