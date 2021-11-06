#include <SoftwareSerial.h>
SoftwareSerial serial_connection(12, 13);//Create a serial connection with TX and RX on these pins
#define BUFFER_SIZE 64//This will prevent buffer overruns.
#define ledPin 6 // Pin 7 for the LED

char inData[BUFFER_SIZE];//This is a character buffer where the data sent by the python script will go.
char inChar=-1;//Initialie the first character as nothing
int count=0;//This is the number of lines sent in from the python script
int i=0;
int RMotor_1 = 2;
int RMotor_2 = 3;
int LMotor_1 = 4;
int LMotor_2 = 5;
int REnable = 10;
int LEnable = 11;
int motor_speed;
uint32_t period = 11000L;
int loopcount;
void setup() {
  Serial.begin(9600);
  Serial.println("GPIO test!");
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  serial_connection.begin(9600);
  serial_connection.println("Reasdy");
  Serial.println("Started");
  pinMode(RMotor_1, OUTPUT);
  pinMode(RMotor_2, OUTPUT);
  pinMode(LMotor_1, OUTPUT);
  pinMode(LMotor_2, OUTPUT);
  pinMode(REnable, OUTPUT);
  pinMode(LEnable, OUTPUT);

  analogWrite(10, 210);
  analogWrite(11, 210);
  delay(1000);



}

void loop() {

  byte byte_count=serial_connection.available();//This gets the number of bytes that were sent by the python script
  if(byte_count)//If there are any bytes then deal with them
  {
    Serial.println("Incoming Data");//Signal to the monitor that something is happening
    int first_bytes=byte_count;//initialize the number of bytes that we might handle.
    int remaining_bytes=0;//Initialize the bytes that we may have to burn off to prevent a buffer overrun
    if(first_bytes>=BUFFER_SIZE-1)//If the incoming byte count is more than our buffer...
    {
      remaining_bytes=byte_count-(BUFFER_SIZE-1);//Reduce the bytes that we plan on handleing to below the buffer size
    }
    for(i=0;i<first_bytes;i++)//Handle the number of incoming bytes
    {
      inChar=serial_connection.read();//Read one byte
      inData[i]=inChar;//Put it into a character string(array)
    }
    inData[i]='\0';//This ends the character array with a null character. This signals the end of a string
    if(String(inData)=="2")//From Python script Turn LED ON when transmission starts
    {
      Serial.println("********* Control Handed Over to [Light Sensors] *********");
      digitalWrite(ledPin, LOW); // Turn LED ON
      uint32_t end_time=millis();
      for( uint32_t tStart = millis();  (end_time-tStart) <= period+period+period;  ){
      trigger_light_sensors();
      delay(100);
      end_time=millis();
      }
            move_stop();


    }
    else if(String(inData)=="h")//Turn OFF LED when transmission Stops
    {
      Serial.println("********* Control [CV] has detected Happy *********");
      digitalWrite(ledPin, HIGH); // Turn LED OFF
      emotion_happy();
      move_stop();
    }
    else if(String(inData)=="a"){
        Serial.println("********* Control [CV] has detected Anger *********");
        digitalWrite(ledPin,HIGH);
        emotion_anger();
              move_stop();

      }
     else if(String(inData)=="s"){
      Serial.println("********* Control [CV] has detected SAD *********");
      digitalWrite(ledPin,HIGH);
      emotion_sad();
            move_stop();

      
      
      }
    for(i=0;i<remaining_bytes;i++)//This burns off any remaining bytes that the buffer can't handle.
    {
      inChar=serial_connection.read();
    }
    Serial.println(inData);//Print to the monitor what was detected
    serial_connection.println("Bluetooth answers "+String(count)+": "+inData+" received");//Then send an incrmented string back to the python script
    count++;//Increment the line counter
  }
  delay(100);

}
//Start CV Code Block
void emotion_happy(){
  
  uint32_t end_time=millis();
for( uint32_t tStart = millis();  (end_time-tStart) <= period+period;  ){
   if (end_time-tStart<=period/2-2400){
    turn_left();
    }else if (end_time-tStart<=9500-3400){
      
      move_forward();
      }else if(end_time-tStart<=7650){
        turn_right();
        }else if(end_time-tStart<=11650){
          move_forward();
          }else{
            move_stop();
            }
      end_time=millis();
}
digitalWrite(ledPin, LOW);
move_stop();
  }

void emotion_sad(){
       // 5 minutes
uint32_t end_time=millis();
for( uint32_t tStart = millis();  (end_time-tStart) <= period+800;  ){
   if ((end_time-tStart)<=period/2){
    turn_left();
    }else{
      
      turn_right();
      }
      end_time=millis();
}
digitalWrite(ledPin, LOW);
move_stop();
  
  }

void emotion_anger(){

  uint32_t end_time=millis();
for( uint32_t tStart = millis();  (end_time-tStart) < period;  ){
   if ((end_time-tStart)<=4000){
    move_backward();
    //Serial.println("Back");
    }else if(end_time-tStart<=7000){
      
      move_stop();
      }else{
        move_forward();
            //Serial.println("forward");

        }
      end_time=millis();
}
digitalWrite(ledPin, LOW);
move_stop();
  
}
//End CV Code Block
// Start Light Module Code Block
void trigger_light_sensors(){
    int ldrright = digitalRead(7);
  int ldrleft = digitalRead(8);
    if (ldrright == 0 && ldrleft == 0) {
    Serial.println("Forward");
    move_forward();
  }
  if (ldrright == 0 && ldrleft == 1) {
    Serial.println("Right");
    turn_right();
  }
  if (ldrright == 1 && ldrleft == 0) {
    Serial.println("Left");
    turn_left();
  }
  if (ldrright == 1 && ldrleft == 1) {
    Serial.println("Stop");
    move_stop();
  }
  delay(100);
 }

void move_forward() {
  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, HIGH);
}

void move_backward() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, HIGH);
  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

void turn_right() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, HIGH);
  digitalWrite(LMotor_1, LOW);
  
  digitalWrite(LMotor_2, HIGH);
      Serial.println("R");

}

void turn_left() {
  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
      Serial.println("L");

}

void move_stop() {
  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, LOW);
  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, LOW);
}
// End Light Module Code Block
