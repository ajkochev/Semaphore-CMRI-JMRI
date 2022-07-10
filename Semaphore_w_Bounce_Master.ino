/* 
CMRI Three and Two position semaphore signal controler for use with JMRI.
Version 1.0 - Made 7-2022 by Anthony Kochevar 
with the help of a few friends who provided insight into CMRI detection, movement 
and the bounce function.
This sketch can be freely distributed and modified provided there is no cost.
Can be used with my 3D printable N scale semaphore signals found on
Thingiverse.

PLEASE READ ALL COMMENTS IN THE SKETCH THOUROGHLY.

JMRI INFO:  To correctly use three postion semaphore servos with JMRI 
each servo gets three CMRI node addresses.  In the example below we have
8 Three postion servos and 8 two position servos-LEDs.
Since the CMRI address below is 3 the starting CMRI address is 3001 for
the Port 0 (PCA9685 boards start at 0) servo is the first postion, 3002 for 
the middle postion and 3003 for the third postion.  3004 to 3006 is for Port 1 servo.  
3007 to 3009 for Port 2 and so on through all the ports.

    On two position servos they are still assigned three CMRI addresses but only the first one is used.
This was done so that adding or removing different types of semaphores
won't cause you to have to recalulate and reenter all the two position servos in JMRI.
In JMRI you will setup each postion as a "light" first. Then create a internal turnout 
for each "light" then last a light controler linked to the internal turnout with 
"thrown +" being items in JMRI you can now create a three output signal head.

    Be aware of which positon is which and plan accordingly.  In my case with the way
I mounted my three postion semaphore servo in a way that had red as the first and lowest value
position (3001) yellow the middle position (3002) and green as the third and highest
value positon (3003).  I had to plan accordingly when setting things up in JMRI.
In JMRI when a CMRI position address (light) is ON (Thrown) and the servo moves to that position
and sets the other two CMRI postion addresses to OFF (closed).

    A two positon semaphore is much simpler.  It only needs one CMRI address as it can be 
on or off, closed or thrown.  On two position servos they still get assigned three CMRI 
addresses but only the first one is used.  This was done so that adding or removing different 
types of semaphores (three or two position) won't cause you to have to recalulate and reenter 
all the two position servos CMRI addresses in JMRI.

In the example below Port 8 has CMRI address of 3025 to 3027, but only 3025 is used.
Port 9 has addresses 3028 to 3030, but only 3028 is used.  The other addresses become 
active or inactive as you add or remove signals via N_TRISERVO and N_BISERVO below.

You'll setup the two postion semaphores as a "light" just like above and also define a 
internal turnout.  You'll then create a light controler linked to the turnout.
You can then create a double output signal head using the light controled with one position 
as closed and the other as thrown.

Two position CMRI addresses can be used for switch track turnout servos and LEDs in JMRI.

If you change the number of three and two position servos below you will need to calculate the CMRI 
addresses accordingly.


*/
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <CMRI.h>
#include <Auto485.h>
#include <Servo.h>

/*
Below sets up CMRI serial interface and PCA9685 board.
Change CMRI_ADDR as needed for your node you need in JMRI.
*/
#define CMRI_ADDR 3
#define DE_PIN    2  //Ardino pin (TX enable ?)

// Change PCA9685 board address as needed below.  0x40 is the default.
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver (0x40);
Auto485 bus (DE_PIN);
CMRI cmri (CMRI_ADDR, 24, 48, bus);  // addr, # inp,  # out and bus speed


// ------DO NOT CHANGE THESE THREE VALUES----------
#define N_POS     3
#define N_ARRAY   5
#define N_BOUNCE  8
// ------------------------------------------------

/*  
N_TRISERVO and N_BISERVO below must add up to 16 (total ports on PCA9685 board)
You can change the number of each here depending on your needs.  TriServos are for
three postion semaphores and BiServo is for two position semaphores and can also be used for 
turnouts or LEDs.
The three period values can be used to adjust servo movement beyond "steps"
in the servoVal and bounceVal arrays.  A change here affects all servos.
SERVOPERIOD is regular arm movement with out bounce.
BOUNCEMOVEPERIOD controls the speed at which the servo moves to the starting bounce period.
This is used in conjunction with the bounceVal, Steps.  Use these values to simulate
the semaphore spectacle falling fast due to gravity.
BOUNCEPERIOD is used in conjunction with bounceVal, Amplitude and Speed to adjust how 
fast the bounce itself moves.
*/
#define N_TRISERVO 8
#define N_BISERVO 8
#define SERVOPERIOD       30   //Change to speed up(lower value) or slow down(incease value) servos.
#define BOUNCEMOVEPERIOD  20   //Change to speed up(lower value) or slow down(incease value) servos.
#define BOUNCEPERIOD      20   //Change to speed up(lower value) or slow down(incease value) servos.

/*
The servoVal array controls regular arm movement without bounce.  Startup sets the start up position of
the servo, 1 (min), 2 (mid) or 3 (max) when the Arduino is booted.  
Steps is the number of step size to move the servo if a move is
needed and the time in SERVOPERIOD is reached.
Two position servos are defined here as well.  The third positon in the array is ignored in this case.
Servos min and max positions can vary usually 100 to 1900 is a good range.
LEDs can go from 0 (off) to 4096 (max briteness).
LEDs use bigger steps usually as they will fade on and off.  A smaller value does this 
as well but much slower.

      --Positions--   --StartUp--  --Steps--  */
int servoVal[N_TRISERVO + N_BISERVO][N_ARRAY] = {
    { 810,  950,   1150,   2,         5 },    // Port 0 servo on PCA board
    { 800,  1020,  1220,   3,         5 },
    { 900,  1000,  1100,   1,         5 },
    { 900,  1000,  1100,   1,         5 },

    { 900,  1000,  1100,   1,         5 },   // Port 4 
    { 900,  1000,  1100,   1,         5 },
    { 900,  1000,  1100,   1,         5 },
    { 900,  1000,  1100,   1,         5 },

    { 900,  1080,  1080,   2,         5 },   // Port 8 Begining of BiServos (two positions)
    { 900,  1000,  1000,   2,         5 },
    { 900,  1000,  1000,   1,         5 },
    { 800,  1000,  1000,   1,         5 },

    { 0,    4000,  4000,   2,         200 },  // Port 12 LED Light Example
    { 0,    4000,  4000,   2,         200 },  // LED Light
    { 0,    4000,  4000,   2,         200 },  // LED Light
    { 0,    4000,  4000,   2,         200 }   // LED Light
};

/*
If you want bounce enabled, positions below should be the same as servoVal above.
A "0" disables bounce for that position.  Be sure to orient servos and armature 
so bounce works correctly.  Pick a standard for all servos and mount them all 
the saem way under layout.
Bounce will only work going from larger values to smaller values in the code.
Setting REVERSE to 1 inverts the bounce if needed.  Not used for now. Possibly in future versions.
STEPS here along with BOUNCEMOVEPERIOD
defines the speed of the movement towards the starting bounce position.  Use BOUCEMOVEPERIOD
and STEPS here to simulate a semaphore spectacle falling fast due to gravity.
AMPLITUDE defines how aggressive the bounce is (higher is more).
SPEED defines how fast the bounce processes (higher is slower).
BOUNCES defines the total number of bounces to create.
BOUNCEPERIOD above can also be adjusted to tune bounce movement but affects all servos.

       --Positions--  --Amplitude-- --Speed-- --Bounces-- --Reverse-- --Steps--      */
int bounceVal[N_TRISERVO + N_BISERVO][N_BOUNCE] = { 
    {   810,  950,  0,     200,        350,        3,          0,        15 },  // Port 0 servo on PCA board
    {   800,  1020, 0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },

    {   0,    0,    0,     200,        350,        3,          0,        15 },  // Port 4
    {   0,    0,    0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },

    {   900,  0,    0,     200,        350,        3,          0,        15 },  // Port 8 Start of BiServos
    {   0,    0,    0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },

    {   0,    0,    0,     200,        350,        3,          0,        15 },  // Port 12 Bounce turned off as these are LEDs
    {   0,    0,    0,     200,        350,        3,          0,        15 },
    {   0,    0,    0,     200,        350,        3,          0,        15 },  
    {   0,    0,    0,     200,        350,        3,          0,        15 },


};

int   servoPos[N_TRISERVO + N_BISERVO];
int   servoTarg[N_TRISERVO + N_BISERVO];
int   bouncePos[N_TRISERVO + N_BISERVO];
bool  bounceNow[N_TRISERVO + N_BISERVO];

unsigned long moveMSec;
unsigned long bounceMSec;
unsigned long bouncePMSec;
double millisStart[N_TRISERVO + N_BISERVO];
// -----------------------------------------------------------------------------


void loop ()
{

    // Process 1 of 4 - Check CMRI bit for servos and lights
    cmri.process ();

    // Check Three Position Servos (Three CMRI addresses per Servo)
    for (int s = 0; s < N_TRISERVO; s++)  {
        for (int p = 0; p < N_POS; p++) {
            int bit = (s*N_POS) + p;
            if (cmri.get_bit (bit))  {
                //Check and set bounce if only comming from one direction.
                //Will bounce going from high position values to lower only.
                if ((bounceVal[s][p] != 0) && (bounceVal[s][p] < servoPos[s])) {
                    bounceNow[s] = true;
                    bouncePos[s] = bounceVal[s][p];
                    servoTarg[s] = servoVal[s][p];
                    millisStart[s] = millis();
                    break;   // on one position bit should be set
                }
                else  {
                    servoTarg[s] = servoVal[s][p];
                    break;   // on one position bit should be set
                }
            }
         }  //end for p
     }  //end for s

     /* 
     Check Two Position Servos and LEDs (One CMRI Address per Servo)
     Check every three CMRI addresses and only use the first
     Every three are used so removeing or adding Tri and BiServo numbers
     won't cause you to redo ALL the JMRI addresses for BiServos.
     */
     int b = 0;
     for (int i = N_TRISERVO; i < (N_TRISERVO + N_BISERVO); i++) {
          int bit = (N_TRISERVO * N_POS) + b;
          if (cmri.get_bit (bit) == 1)   {
              servoTarg[i] = servoVal[i][1];
          }
          else if (cmri.get_bit (bit) == 0)  {
              //Check and set bounce if enabled and only comming from one direction
              //Will bounce going from high values to lower only.
              if ((bounceVal[i][0] != 0) && (bounceVal[i][0] < servoPos[i])) {
                bounceNow[i] = true;
                bouncePos[i] = bounceVal[i][0];
                servoTarg[i] = servoVal[i][0];
                millisStart[i] = millis();
              }
              else  {
                servoTarg[i] = servoVal[i][0];
              }
          }  //end else if cmri
          b = b + 3;
      }  //end for i
     // end 1 of 4 -----------------------------------------------

    // Process 2 of 4 - Check and move servos if a change is needed and no bounce set
    unsigned long msec = millis ();
    if ( (msec - moveMSec) >= SERVOPERIOD)  {
        moveMSec = msec;
        // adjust each servo toward target
        for (int s = 0; s < (N_TRISERVO + N_BISERVO); s++)  {
            if ((servoTarg[s] > servoPos[s]) && (bounceNow[s] == false)) {
                servoPos[s] += servoVal[s][4];
                if (servoPos[s] >= servoTarg[s])  {
                  servoPos[s] = servoTarg[s]; 
                }
                pwm1.writeMicroseconds (s, servoPos[s]);
             }  //end if servoTarg
            else if ((servoTarg[s] < servoPos[s]) && (bounceNow[s] == false)) {
                servoPos[s] -= servoVal[s][4];
                if (servoPos[s] <= servoTarg[s])  {
                  servoPos[s] = servoTarg[s];
                }
                pwm1.writeMicroseconds (s, servoPos[s]);
             }  //end else if servoTarg
        } //end for s loop
     } //end if msec
     // end 2 of 4 -----------------------------------------------
    
    // Process 3 of 4 - Move servos to starting bounce position if bounce is needed
    unsigned long bmsec = millis ();
    if ( (bmsec - bounceMSec) >= BOUNCEMOVEPERIOD)  {
        bounceMSec = bmsec;
        // adjust each servo toward target
        for (int s = 0; s < (N_TRISERVO + N_BISERVO); s++)  {
            if ((bouncePos[s] < servoPos[s]) && (bounceNow[s] == true)) {
                servoPos[s] -= bounceVal[s][7];
                if (servoPos[s] <= bouncePos[s])  {
                  servoPos[s] = bouncePos[s];
                  bouncePos[s] = servoPos[s]; 
                }
                pwm1.writeMicroseconds (s, servoPos[s]);
             }  //end if bouncePos
        }  //end for s
    }  //end if bmsec
    //end of 3 of 4 -----------------------------------------------

    //  Process 4 of 4 - Check and run the actual bounce
    unsigned long bpmsec = millis ();
    if ( (bpmsec - bouncePMSec) >= BOUNCEPERIOD)  {
       bouncePMSec = bpmsec;
       for (int s = 0; s < (N_TRISERVO + N_BISERVO); s++)  {
          if ((bounceNow[s] == true) && (servoPos[s] == bouncePos[s]))
              bounceRun(s);
          }  //end for s
    }
    //end of 4 of 4 -----------------------------------------------

}  //end void loop

void bounceRun(int s){ 
   double time = millis() - millisStart[s]; 
   float bounce = abs(sin(time / bounceVal[s][4]) * bounceVal[s][3]); 
   bounce *= max(0, -time/(bounceVal[s][4] * bounceVal[s][5] * 3.14159) + 1); // bounce limit 
   //if(bounceVal[s][6] == 1) 
   //   bounce *= -1; 
   pwm1.writeMicroseconds (s, (bouncePos[s] + bounce)); 
   /* Below if statement assumes each bounce defined in bounceVal, Bounces 
      will each take up to one second and then end the bounce on that servo. 
      This can be changed here if needed. */ 
   if (time >= (bounceVal[s][5] * 1000)) {  
      bounceNow[s] = false;
   }
}  //end void bounceRun


// -----------------------------------------------------------------------------
void setup ()
{
    Serial.begin (19200);
    bus.begin (19200);
    pwm1.begin ();
    pwm1.setPWMFreq (50);
    
    //Set Startup positions for servos and values
    for (int s = 0; s < (N_TRISERVO + N_BISERVO); s++) {
         if (servoVal[s][3] == 1) {
           servoPos[s] = servoVal[s][0];
           servoTarg[s] = servoVal[s][0];
           bouncePos[s] = servoVal[s][0];
         }
         if (servoVal[s][3] == 2) {
           servoPos[s] = servoVal[s][1];
           servoTarg[s] = servoVal[s][1];
           bouncePos[s] = servoVal[s][1];
         }
         if (servoVal[s][3] == 3) {
           servoPos[s] = servoVal[s][2];
           servoTarg[s] = servoVal[s][2];
           bouncePos[s] = servoVal[s][2];
         }
         bounceNow[s] = false;
         pwm1.writeMicroseconds (s, servoPos[s]);
    }  //end for s
 
}
