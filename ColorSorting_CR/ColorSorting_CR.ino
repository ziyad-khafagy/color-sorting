/* A program for a little "Color Sorting Machine"
    This machine will sort the colors of "Skittles"
    A 1st Servo motor will do X steps to put in position the candy,
    then the Color Sensor will perform his job,
    the 2nd Servo will move to the color position
    and then the stepper motor will move again.

    For the project we will use:
    1x Micro Servo: SG90
    1x Servo: MG995
    1x Color Sensor: GY31 TCS3200
    1x Arduino Uno

   
*/

#include<Servo.h>

//Define the pins of the color sensor
#define s0 2 //S0 pin of the sensor on Arduino pin#2 
#define s1 3 //S1 pin of the sensor on Arduino pin#3 
#define s2 4 //S2 pin of the sensor on Arduino pin#4 
#define s3 5 //S3 pin of the sensor on Arduino pin#5
#define sensorOut 6 //Output pin of the sensor on Arduino pin#6 
int micro_servo=0;
int black_servo=0;

//Variable to save the data coming from the sensorOut
int frequency = 0;

// Variable to save the color
byte identifiedColor = 0;

//Declaring Servos

Servo feedingServo;
Servo decisionServo;

//Declaring general delays
byte smallDelay_ms = 5;
byte mediumDelay_ms = 30;
byte bigDelay_ms = 2000;
byte veryBigDelay_ms = 3000;


//Delcaring position of the feederServo
byte pos1FeederServo = 179;
byte pos2FeederServo = 92;
byte pos3FeederServo = 1;

//Delcaring position of the decisionServo
byte decisionServo_RED = 0;
byte decisionServo_ORANGE = 45;
byte decisionServo_GREEN = 90;
byte decisionServo_YELLOW = 135;
byte decisionServo_PURPLE = 180;

void setup() {

  //Set the pins of the Color Sensor
  pinMode (s0, OUTPUT);
  pinMode (s1, OUTPUT);
  pinMode (s2, OUTPUT);
  pinMode (s3, OUTPUT);
  pinMode (sensorOut, INPUT);

  /* The pins S0 & S1 works for the frequency scaling
      L - L = Power Down
      L - H = 2%
      H - L = 20%
      H - H = 100%
  */
  //Using at 100%
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  //Attaching the Servos
  feedingServo.attach (9);
  decisionServo.attach(11);

  //Set the serial communication in bytes per second
  Serial.begin(9600);

}//Close setup

void loop() {

  //Write "steps" of 1 degree to the servo until pos2 (Sensor position)
  for (int i = pos1FeederServo; i > pos2FeederServo ; i--) {

    feedingServo.write (i);
    delay (mediumDelay_ms);

  }//close for loop

  //Delay to stabilize readings
  delay(bigDelay_ms);

  //We launch the Function getTheColor
  identifiedColor = getTheColor();

  //Switch Case to decide wich color are we reading
  /*********IMPORTANT********
   * Please calibrate your Servos and the TCS3200 sensor before uploading this program,
   * Depending on mechanical installation the positions of the servos may change
   * Also the TCS3200 can give you different readings compared to mines
   * because of the ambient lighting (light changes in general)
   */
  switch (identifiedColor) {

    //Case for Red
    case 1:
      decisionServo.write (decisionServo_RED);
      break;

    //Case for Orange
    case 2:
      decisionServo.write (decisionServo_ORANGE);
      break;

    //Case for Green
    case 3:
      decisionServo.write (decisionServo_GREEN);
      break;

    //Case for Yellow
    case 4:
      decisionServo.write (decisionServo_YELLOW);
      break;

    //Case for Purple
    case 5:
      decisionServo.write (decisionServo_PURPLE);
      break;

  }// close Switch Case

  //Delay to exit position
  delay(bigDelay_ms);


  //Move Feeding servo to exit position
  for (int i = pos2FeederServo; i > pos3FeederServo; i--) {

    feedingServo.write (i);
    delay (mediumDelay_ms);

  }//Close for loop - Servo to Exit Position

  //Delay to return servo
  delay(bigDelay_ms);


  //Return the Feeding Servo to initial position
  for (int i = pos3FeederServo; i < pos1FeederServo ; i++) {

    feedingServo.write (i);
    delay (smallDelay_ms);
  }//Close for loop - Servo initial Position


  //Return the Value to 0
  identifiedColor = 0;

  //Delay to begin again
  delay (veryBigDelay_ms);

}//close loop

/*-----User Defined Function-----
   Function Definition:
   getTheColor

   Purpose:
   Apply the filters on the photodiodes to get the R,G&B values

   Return value:
   color      The combination of  R, G , B , C values to decide the color.

    ****Important****
    It is recommended to run the program alone
    With an excel document and the serial monitor open
    check the readings to find the values for each color and change those values below
*/

int getTheColor() {


  //---Read RED values... S2 LOW - S3 LOW---
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  frequency = pulseIn(sensorOut, LOW);

  int redValue = frequency;

  Serial.print("R=");
  Serial.print(redValue);
  Serial.print("  ");

  delay(50);

  //---Read Green values ... S2 HIGH - S3 HIGH---
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  frequency = pulseIn(sensorOut, LOW);

  int greenValue = frequency;

  Serial.print("G=");
  Serial.print(greenValue);
  Serial.print("  ");

  delay(50);


  //---Read Blue values ... S2 LOW - S3 HIGH---
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  frequency = pulseIn(sensorOut, LOW);

  int blueValue = frequency ;

  Serial.print("B=");
  Serial.print(blueValue);
  Serial.print("  ");

  delay(50);


    //---Read Clear values ... S2 HIGH - S3 LOW ---
  digitalWrite(s2, HIGH);
  digitalWrite(s3, LOW);
  frequency = pulseIn(sensorOut, LOW);

  int clearValue = frequency ;

  Serial.print("C=");
  Serial.print(clearValue);
  Serial.println("  ");

  delay(50);


  //////////Parameters for Red//////////
  if (redValue<38 & redValue>14 & clearValue<58 & clearValue>=45 ) {
    identifiedColor = 1; // Red
  }

  //////////Parameters for Orange//////////
  if (redValue<60 & redValue>45  & clearValue<18 & clearValue>25) {
    identifiedColor = 2; // Orange
  }


  //////////Parameters for Green//////////
  if (redValue<70 & redValue>60 & clearValue<25 & clearValue>20 ) {
    identifiedColor = 3; // Green
  }

  //////////Parameters for Yellow//////////
  if (redValue<55 & redValue>40 & clearValue<25 & clearValue>15 ) {
    identifiedColor = 4; // Yellow
  }

  //////////Parameters for Purple//////////
  if ( redValue<85 & redValue>75  & clearValue<28 & clearValue>22) {
    identifiedColor = 5; // Purple
  }


  //--- Return the value found---
  return identifiedColor;

}
