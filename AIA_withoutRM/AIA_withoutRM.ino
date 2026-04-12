#include <HardwareSerial.h>
#include "driveM.h"
#include "driveA.h"



// Drive
//#define Rdir 18
//#define Ldir 19
//#define Rpwm 27m
//#define Lpwm 26

//#define Rdir 16//J8
//#define Rpwm 33
//#define Ldir 32//J12
//#define Lpwm 14

#define Rdir 16//j8
#define Rpwm 14
#define Ldir 33//j12
#define Lpwm 32

//#define Rdir 19
//#define Ldir 27
//#define Rpwm 18
//#define Lpwm 26
// SERIAL
HardwareSerial SerialPort(1);  // use UART1
HardwareSerial Sender(0);      // default serial (UART0)

const int BUFFER_SIZE = 128;
char rxBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Pins
const int pwm_pin[] = {Rpwm, Lpwm };
const int dir_pin[] = {  Rdir, Ldir };

long prevT = 0;
int changeMode = 0;

// Arm variables
float linkLength1 = 39.8, linkLength2 = 37.5;
int L = 0, R = 0, T = 0, U = 0, G = 0;
int freq = 8000, Lchannel = 0, Rchannel = 1, resolution = 8;

// Class instances

DriveM driveM;
DriveA driveA;

const int freqLinks = 8000;      // 5 kHz frequency
const int ledChannelLink1 = 15;  // PWM channel (0-15)
const int resolutionLink1 = 8;

const int ledChannelLink2 = 14;  // PWM channel (0-15)
const int resolutionLink2 = 8;

const int ledChannelSwivel = 12;  // PWM channel (0-15)
const int resolutionSwivel = 8;





void setup() {
  Serial.begin(115200);
  Sender.begin(115200, SERIAL_8N1, 3, 1);
  SerialPort.begin(115200, SERIAL_8N1, 2, 0);

  driveM.setDrivePins(dir_pin[0], dir_pin[1], pwm_pin[0], pwm_pin[1]);
  Serial.println("Pins set!");
  delay(100);

  //ik.set_link_length(linkLength1, linkLength2);
  //ik.set_pid();
  Serial.println("PID set");
  delay(10);
}

void loop() {
  if (SerialPort.available()) {
    // Read incoming data
    while (SerialPort.available()) {
      rxBuffer[bufferIndex++] = (char)SerialPort.read();
      if (bufferIndex >= BUFFER_SIZE) bufferIndex = 0;
    }

    Serial.println(rxBuffer);
    // Parse packet markers
    char *M_index = strchr(rxBuffer, 'M');
    char *x_index = strchr(rxBuffer, 'X');
    char *y_index = strchr(rxBuffer, 'Y');
    char *P_index = strchr(rxBuffer, 'P');
    char *Q_index = strchr(rxBuffer, 'Q');
    char *A_index = strchr(rxBuffer, 'A');
    char *S_index = strchr(rxBuffer, 'S');
    char *J_index = strchr(rxBuffer, 'J');
    char *D_index = strchr(rxBuffer, 'D');
    char *L_index = strchr(rxBuffer, 'L');
    char *R_index = strchr(rxBuffer, 'R');
    char *Z_index = strchr(rxBuffer, 'Z');
    char *E_index = strchr(rxBuffer, 'E');

    // Check if packet has 'Z' == '0' (maybe you want to check value rather than pointer)
    if (Z_index != nullptr && *(Z_index + 1) == '0') {
      if (M_index && x_index && y_index && P_index && Q_index && A_index && S_index && J_index && D_index && E_index) {

        int M = *(M_index + 1) - '0';
        int x = atoi(x_index + 1);
        int y = atoi(y_index + 1);
        int X = atoi(P_index + 1);
        if (abs(X) < 3) X = 0;
        int Y = atoi(Q_index + 1);
        if (abs(Y) < 3) Y = 0;
        int grip = atoi(A_index + 1);
        int Z = atoi(S_index + 1);
        if (abs(Z) < 3) Z = 0;
        int Reset = atoi(R_index + 1);
        int Mode = atoi(D_index + 1);

        driveM.MotorCode(x, y, M);
        if (Mode == 0) {
          if (changeMode == 1) {
            Serial.println("DRIVEEEEE");
            changeMode = 0;
          }
          driveM.MotorCode(x, y, M);




        }
      } else {
        Serial.println("Invalid Packet received");
      }

    } else {  // Else assume driving left/right
      if (L_index) {

        L = atoi(L_index + 1);
      }
      if (R_index) {
        R = atoi(R_index + 1);
      }
      int l1=abs(L * 2.55);
      int r1=abs(R * 2.55);
      ledcWrite(Rchannel, r1);
      ledcWrite(Lchannel, l1);
      Serial.printf("%d:%d:::%d:%d\n", L, R, l1, r1);

      if (L > 0) {
        digitalWrite(Ldir, LOW);
      } else if (L < 0) {
        digitalWrite(Ldir, HIGH);
      } else digitalWrite(Ldir, LOW);

      if (R > 0) digitalWrite(Rdir, LOW);
      else if (R < 0) digitalWrite(Rdir, HIGH);
      else digitalWrite(Rdir, LOW);


    
    }

    bufferIndex = 0;
  }
}