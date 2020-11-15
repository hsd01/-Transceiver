/* this is for 4 channel transreciever 
 * i'm not good on writting comments on code and variable might seams off to everyone but i did my best.. 
 * if u want to add more buttons or fnction or i say creat channel make sure to add variables on structure.
 * Hemant_S_D[TAKA]
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*Create a unique pipe out. The receiver has to 
  wear the same unique code*/
  
 //IMPORTANT: The same as in the receiver
const uint64_t pipeOut = 0xA0B0C0D0E0LL;
RF24 radio(9, 10); // select  ce,CSN  pin

// The sizeof this struct should not exceed 32 bytes
// This gives us up to 32 8 bits channals
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
  //This are the start values of each channal
  // ch1 is 0 in order to stop the motors
  //127 is the middle value of the 10ADC.
    
  data.ch1 = 0;
  data.ch2 = 127;
  data.ch3 = 127;
  data.ch4 = 127;
  
}

void setup()
{
  //Start everything up
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  resetData();
}

/**************************************************/

// Returns a corrected value for a joystick position that takes into account
// the values of the outer extents and the middle of the joystick range.
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}

void loop()
{
  // The calibration numbers used here should be measured 
  // for your joysticks till they send the correct values.
  data.ch1 = mapJoystickValues( analogRead(A0), 13, 524, 1015, true );
  data.ch2 = mapJoystickValues( analogRead(A1),  1, 505, 1020, true );
  data.ch3 = mapJoystickValues( analogRead(A2), 12, 544, 1021, true );
  data.ch4 = mapJoystickValues( analogRead(A3), 34, 522, 1020, true );
  Serial.print("   ");
  Serial.println(data.ch1);
  Serial.print("  ");
  Serial.print(data.ch2);
  Serial.print("  ");
  Serial.print(data.ch3);
  Serial.print("  ");
  Serial.print(data.ch4);
  Serial.print("  ");
  
  radio.write(&data, sizeof(MyData));
}
