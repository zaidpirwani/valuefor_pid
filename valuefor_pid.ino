#include <IRLib.h>
const int ir = 8;
IRrecv My_Receiver(ir);
IRdecode My_Decoder;

int sensors[] = {0,0,0};
int sensPins[]= {A6,A2,A1};
int bgnd[] = {0,0,0};
int line[]  = {1023,1023,1023};
int diff[]  = {0,0,0};
float value;
int follow=0;

unsigned long previousMillis = 0;
const long interval = 50;

const int motor1a = 9;  //PWM
const int motor2a = 12; //PWM
const int motor1b = 10;
const int motor2b = 11;

const int buzzer = 3;
const int lights = A3;

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("STARTING");
  My_Receiver.enableIRIn();
  delay(1000);
  pinMode(motor1a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor2b, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(lights, OUTPUT);
  
  digitalWrite(lights, LOW);
  delay(100);
  digitalWrite(lights, HIGH);
  delay(100);
  digitalWrite(lights, LOW);
  delay(100);
  digitalWrite(lights, HIGH);
  delay(100);
}

void loop(){
  if (My_Receiver.GetResults(&My_Decoder)) {
      My_Decoder.decode();      //Decode the data
      switch(My_Decoder.value){
        case 0x843501FE: // PAUSE
          calibrate();
          break;  
        case 0x843502FD: // FORWARD
          if(follow==0)
            follow=1;
          else
            follow=0;
          break;
        case 0x843504FB: // MENU
          horn();
          break;  
        case 0x843508F7: // MOUSE
          headLights();
          break;
        case 0x843503FC:  // STOP
          halt();
          break;
      }
      //My_Decoder.DumpResults(); //Show the results on serial monitor
      My_Receiver.resume();     //Restart the receiver
  }
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    if(follow==1){
      getSensor();
      setMotors(value);
    }
  }
}

void horn(){
  digitalWrite(buzzer, HIGH);
}

void headLights(){
  digitalWrite(lights, !digitalRead(lights));  
}

void getSensor() {
  digitalWrite(lights, LOW);
  int i, s[3], a;
  for(i=0;i<3;i++)
    sensors[i] = analogRead(sensPins[i]);

  for(a=0; a<3; a++){
    if(sensors[a]<line[a])
      s[a]=100;
    else if(sensors[a]>bgnd[a])
      s[a]=0;
    else
      s[a]=((bgnd[a]-sensors[a])/(float)(bgnd[a]-line[a]))*100;
  }

  if(s[0]==100&&s[1]==100&&s[2]==100)
    value=0;
  else if(s[0]==0&&s[1]==0&&s[2]==0)
    value=0;
  else
    value= s[1]-100-(2*s[0])+(2*s[2]);

  if(value>100)
    value=100;
  else if(value<-100)
    value=-100;

  Serial.print(value);
  Serial.print(" \n");
}

void calibrate(){
  int a, b;
  for(a=0; a<3; a++){
      bgnd[a] = 0;
      line[a] = 1023;
    }
  digitalWrite(lights, LOW);
  delay(1000);
  forward();
  unsigned long prevTime = millis();
  do{
    int s[3] = {0,0,0};
    for(a=0; a<3; a++){
      s[a] += analogRead(sensors[a]);
    }
    int tmp[3];
    for(a=0; a<3; a++){
        tmp[a] = s[a];
        if(tmp[a]<line[a])  line[a] = tmp[a];
        if(tmp[a]>bgnd[a])  bgnd[a] = tmp[a];
        Serial.print(String(tmp[a]) + " , ");
      }
    Serial.println("");
  }while(millis()<(prevTime+700));
  halt();
  Serial.print("LINE: ");
  for(a=0; a<3; a++){
    Serial.print(String(line[a]) + " , ");
  }
  Serial.println("");
  Serial.print("FLOOR: ");
  for(a=0; a<3; a++){
    Serial.print(String(bgnd[a]) + " , ");
  }
  Serial.println("");
  Serial.print("DIFF: ");
  for(a=0; a<3; a++){
    diff[a] = bgnd[a] - line[a];
    Serial.print(String(diff[a]) + " , ");
  }
  Serial.println("");  
}

void setMotors(int value){
  int spdP, spdL;
  if(value>0)
    spdP = map(value, 0, 100, 50, 255);
  if(value<0)
    spdL = map(value*-1, 0, 100, 50, 255);
  if(value>-10 && value<10)
    forward();
  else{
      if(value>10){
        analogWrite(motor1a, spdP);
        digitalWrite(motor2a, LOW);
        
        digitalWrite(motor1b, LOW);
        analogWrite(motor2b, spdP);        
      }else if(value<-10){
        digitalWrite(motor1a, LOW);
        analogWrite(motor2a, spdL);
        
        analogWrite(motor1b, spdL);
        digitalWrite(motor2b, LOW);        
      }
  }
}

void forward(){
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor2a, HIGH);
  
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2b, LOW);
}

void reverse(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor2a, LOW);
  
  digitalWrite(motor1b, HIGH);
  digitalWrite(motor2b, HIGH);
}

void right(){
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor2a, LOW);
  
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2b, HIGH);
}

void left(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor2a, HIGH);
  
  digitalWrite(motor1b, HIGH);
  digitalWrite(motor2b, LOW);
}

void halt(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor2a, LOW);
  
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2b, LOW);

  digitalWrite(buzzer, LOW);
}
