/* this is for 4 channel transreciever 
 * i'm not good on writting comments on code and variable might seams off to everyone but i did my best.. 
 * if u want to add more buttons or fnction or i say creat channel make sure to add variables on structure.
 * Hemant_S_D[TAKA]
 */

#include <Servo.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

 //Remember that this code is the same as in the transmitter
const uint64_t pipeIn = 0xA0B0C0D0E0LL;
RF24 radio(9, 10); 
Servo myServo,myServo1,myServo2,esc;

//int val;
//We could use up to 32 channels
struct MyData 
{
byte ch1;
byte ch2;
byte ch3;
byte ch4;
};

MyData data;

void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.ch1 = 145;
data.ch2 = 127;
data.ch3 = 127;
data.ch4 = 127;

}

/**************************************************/

void setup()
{
Serial.begin(9600); //Set the speed to 9600 bauds if you want.
//You should always have the same speed selected in the serial monitor
resetData();
radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.openReadingPipe(1,pipeIn);
//we start the radio comunication
radio.startListening();

myServo.attach(4);
myServo1.attach(6);
myServo2.attach(3);
//pinMode(5,OUTPUT);
esc.attach(5);



}

/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); //here we receive the data
}
}

/**************************************************/

void loop()
{
  
  Serial.println("....");
  //Serial.println(lastRecvTime);
 // Serial.println(now);
//Serial.println(".************.......");

recvData();
unsigned long now = millis();

//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 5000 ) {
// Signal lost?
Serial.println("If....:");
resetData();

}
//Serial.println("Milli");

int val = map(data.ch1,135,0,1000,2000);
Serial.println(val);
int val1 = map(data.ch4,0,255,0,180);

int val2 = map(data.ch3,0,255,0,180);

int val3 = map(data.ch2,0,255,0,180);

esc.writeMicroseconds(val);
myServo.write(val1);
myServo1.write(val2);
myServo2.write(val3);
//esc.write(val);

/*if(val > 0)
{
//analogWrite(5,val);
esc.writeMicroseconds(val);
}
else
{
  esc.writeMicroseconds(val);
  //analogWrite(5,0);
}*/


//Serial.println();
Serial.print("ch1: "); Serial.print(data.ch1);  Serial.print("    ");
Serial.print("ch2: "); Serial.print(data.ch2);  Serial.print("    ");
Serial.print("ch3: "); Serial.print(data.ch3);  Serial.print("    ");
Serial.print("ch4: "); Serial.print(data.ch4);  Serial.print("\n");

}
