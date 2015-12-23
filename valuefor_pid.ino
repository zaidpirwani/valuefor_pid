  /*ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
int light = A3;
int sensors[] = {0,0,0};
int sensPins[]= {A6,A2,A1};
float s1,s2,s3,value;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //set pin as output
  pinMode(light, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  //set light on
  digitalWrite(light, LOW);
  int i;
  for(i=0;i<3;i++)
  {
    sensors[i] = analogRead(sensPins[i]);
  }
//for sensor 1
  if(sensors[0]<220)
  {
    s1=100;
  }
  else if(sensors[0]>320)
  {
    s1=0;
  }
  else
  {
    s1=(320-sensors[0]);
  }
//for sensor 2
  if(sensors[1]<220)
  {
    s2=100;
  }
  else if(sensors[1]>310)
  {
    s2=0;
  }
  else
  {
    s2=((310-sensors[1])/90.0)*100;
  }
//for sensor 3
  if(sensors[2]<110)
  {
    s3=100;
  }
  else if(sensors[2]>180)
  {
    s3=0;
  }
  else
  {
    s3=((180-sensors[2])/70.0)*100;
  }
//conversion into 1 value
  if(s1==100&&s2==100&&s3==100)
  {
    value=0;
  }
  else if(s1==0&&s2==0&&s3==0)
  {
    value=0;
  }
  else
  {
    value= s2-100-(2*s1)+(2*s3);
  }
//value in range
  if(value>200)
  {
    value=200;
  }
  else if(value<-200)
  {
    value=-200;
  }
  else
  {
    value=value;
  
  }

   Serial.print(value);
   Serial.print(" \n");
    
  }

