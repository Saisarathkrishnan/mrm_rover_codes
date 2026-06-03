  //#include <analogWrite.h>
#include <HardwareSerial.h>
#include "arm_ik.h"
#include "drive.h"
#include "Wire.h"

//---ESP32_PIN_CONFIG---
//Arm




#define dir_pump 14
#define pwm_pump 13//J7 17 25

//Drive
#define pwm_swivel 23
#define dir_swivel 32 //J4

#define pwm_link1 17


#define dir_link1 16//J1

#define pwm_link2 19//J2
#define dir_link2 18



//Drive
//#define Rdir 19//j11
//#define Ldir 18//J16
//#define Rpwm 27
//#define Lpwm 26

 #define Rdir 26//j6
 #define Ldir 27//J6
 #define Rpwm 33 // j5
 #define Lpwm 25 // j5

// SERIAL
HardwareSerial SerialPort(1); // use UART0
HardwareSerial Sender(0);
const int BUFFER_SIZE = 128;
char rxBuffer[BUFFER_SIZE];
int bufferIndex = 0;


// Pins
const int pwm_pin[] = {pwm_swivel, pwm_link1, pwm_link2, Rpwm, Lpwm, pwm_pump};
const int dir_pin[] = {dir_swivel, dir_link1, dir_link2, Rdir, Ldir, dir_pump};


// Globals
long prevT = 0;
int changeMode = 0;
int r = 0;

// Arm Variables
int set_link[3];
int pwr[3];
int dir[3];
int SW;
int Lone;
int Ltwo;

// Class instances
SM sm;
//FK fk;
Drive drive;



void gripper(int grip, int Mode)
{
  if (Mode == 1){
    Sender.print(rxBuffer);
    }
  else
  {
  switch (grip)
  {
    case 1:
      Sender.print("M0X0Y0P0Q0A1S0R0D0C0E");
      //dusra auger updown
      break;

    case 2: 
      Sender.print("M0X0Y0P0Q0A2S0R0D0C0E");
      //dusra auger updown
      break;

    case 3: 
      Sender.print("M0X0Y0P0Q0A3S0R0D0C0E");
      break;

    case 4: 
      Sender.print("M0X0Y0P0Q0A4S0R0D0C0E");
      break;

    case 5: 
      Sender.print("M0X0Y0P0Q0A5S0R0D0C0E");
      break;

    case 6: 
      Sender.print("M0X0Y0P0Q0A6S0R0D0C0E");
      break;

    case 7: 
      //Sender.write('I');
    Sender.print("M0X0Y0P0Q0A7S0R0D0C0E");
      break;
    default:
     Sender.print("M0X0Y0P0Q0A0S0R0D0C0E");
     break;
  }}
}


   
 
  
  
  
void setup()
{
  Serial.begin(115200);
  Sender.begin(115200, SERIAL_8N1, -1, 1) ;  // Use default serial for debug output
  SerialPort.begin(115200, SERIAL_8N1, 21, 0);//(baud rate,protocol,Tx,Rx)

  // Set Drive Output Pins
  drive.setDrivePins(dir_pin[3], dir_pin[4], pwm_pin[3], pwm_pin[4]);
  sm.setPins(dir_pin[0], pwm_pin[0], dir_pin[1], pwm_pin[1], dir_pin[2], pwm_pin[2], dir_pin[5], pwm_pin[5]);
  Serial.println("Sare pins set hogaye!");
  delay(100);

}

void loop()
{
  // Serial.println("Looping");
  if (SerialPort.available())
  {
    // read the data into the buffer
    while (SerialPort.available())
    {
      rxBuffer[bufferIndex] = (char)SerialPort.read();
      bufferIndex++;
      // Make sure we don't overflow the buffer
      if (bufferIndex >= BUFFER_SIZE)
        bufferIndex = 0;
    }
    //Serial.println(rxBuffer);

    // Find the positions of the "M", "X", "Y", "P", "Q", "A", "S", "R", "D" and "E" characters in the buffer
    char *M_index = strchr(rxBuffer, 'M'); // gear
    char *x_index = strchr(rxBuffer, 'X'); // drive x
    char *y_index = strchr(rxBuffer, 'Y'); // drive y
    char *P_index = strchr(rxBuffer, 'P'); // arm X
    char *Q_index = strchr(rxBuffer, 'Q'); // arm Y
    char *A_index = strchr(rxBuffer, 'A'); // gripper
    char *S_index = strchr(rxBuffer, 'S'); // arm Z
    char *R_index = strchr(rxBuffer, 'R'); // Resets
    char *D_index = strchr(rxBuffer, 'D'); // Mode
    char *C_index = strchr(rxBuffer, 'C');
    char *E_index = strchr(rxBuffer, 'E'); // End

    if (M_index != NULL && x_index != NULL && y_index != NULL && P_index != NULL && Q_index != NULL && A_index != NULL && S_index != NULL && R_index != NULL && D_index != NULL && C_index != NULL && E_index != NULL)
    {
      // Extract the values from the packet
      char m = *(M_index + 1);
      int M = m - '0';
      int x = atoi(x_index + 1);
      int y = atoi(y_index + 1);
      int X = atoi(P_index + 1);
      if (abs(X) < 3)
      {
        X = 0;
      }
      int Y = atoi(Q_index + 1);
      if (abs(Y) < 3)
      {
        Y = 0;
      }
      int grip = atoi(A_index + 1);
      int Z = atoi(S_index + 1);
      if (abs(Z) < 3)
      {
        Z = 0;
      }
      int Resets = atoi(R_index + 1);
      int Mode = atoi(D_index + 1);
      if (Mode == 1){
        Sender.print(rxBuffer);
        }
      drive.MotorCode(x, y, M);
      if (Mode == 0)
      {
        if (changeMode == 1)
        {
        //  Serial.println("DRIVEEEEE");
          changeMode = 0;
        }
        drive.MotorCode(x, y, M);
        sm.beginSM(X, Y, Z, grip);
        gripper(grip, Mode);

      }
      else
    {
      if (changeMode == 0)
      {
        Serial.println("IK");
        changeMode = 1;
      }
        drive.MotorCode(x, y, M);
        
        gripper(grip, Mode);
        
      }
      delay(10);

    }
    else
    {
      Serial.println("Invalid Packet received");
    }
  }
  else
  {
    sm.Stop(); 
    drive.Stop();
  }
  bufferIndex = 0;
}