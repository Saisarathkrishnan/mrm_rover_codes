#include <HardwareSerial.h>
#include "driveM.h"
#include "driveA.h"



 #define Rdir 27//j6
 #define Ldir 26//J6
 #define Rpwm 25 // j5
 #define Lpwm 33 // j5


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
  
  SerialPort.begin(115200, SERIAL_8N1, 21, -1);

  driveM.setDrivePins(dir_pin[0], dir_pin[1], pwm_pin[0], pwm_pin[1]);
  Serial.println("Pins set!");
  delay(100);
  pinMode(Rdir, OUTPUT);

ledcSetup(Rchannel, freq, resolution);
ledcAttachPin(Rpwm, Rchannel);
pinMode(Ldir, OUTPUT);

ledcSetup(Lchannel, freq, resolution);
ledcAttachPin(Lpwm, Lchannel);

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

    char *L_index = strchr(rxBuffer, 'L');
    char *R_index = strchr(rxBuffer, 'R');
    char *E_index = strchr(rxBuffer, 'E');
  
      if (L_index) {

        L = atoi(L_index + 1);
      }
      if (R_index) {
        R = atoi(R_index + 1);
      }
      int l1=abs(L * 2.55);
      int r1=abs(R * 2.55);

      Serial.printf("%d:%d:::%d:%d\n", L, R, l1, r1);

      if (L > 0) {
        digitalWrite(Ldir, LOW);
      } else if (L < 0) {
        digitalWrite(Ldir, HIGH);
      } else digitalWrite(Ldir, LOW);

      if (R > 0) digitalWrite(Rdir, LOW);
      else if (R < 0) digitalWrite(Rdir, HIGH);
      else digitalWrite(Rdir, LOW);
      ledcWrite(Rchannel, r1);
      ledcWrite(Lchannel, l1);
    
    bufferIndex = 0;
  }else{
      Serial.println("hi");

  }

}