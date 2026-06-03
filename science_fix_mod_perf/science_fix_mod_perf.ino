//#include <HardwareSerial.h>

//--ESP32-PINS--
#define Left_PWM 17 //J8
#define Left_DIR 16

#define Right_PWM 19 //J9
#define Right_DIR 18

#define Grip_PWM 22//J10
#define Grip_DIR 21

#define Gantry_PWM 32//J11
#define Gantry_DIR 23

#define AGantry_PWM 25 //j12
#define AGantry_DIR 33


#define Auger_PWM 27//J13
#define Auger_DIR 26


#define tray_PWM 13//J4
#define tray_DIR 14


const int pwm_pin[] = {Left_PWM , Right_PWM , Grip_PWM, Gantry_PWM, AGantry_PWM, Auger_PWM, tray_DIR};
const int dir_pin[] = {Left_DIR , Right_DIR , Grip_DIR, Gantry_DIR, AGantry_DIR, Auger_DIR, tray_PWM};

//--SERIAL--
HardwareSerial SerialPort(0); // use UART0
HardwareSerial Sender(1);
const int BUFFER_SIZE = 128;
char rxBuffer[BUFFER_SIZE];
int bufferIndex = 0;

//--NUMBER-OF-MOTORS--
#define NMOTORS 7
//int target[NMOTORS];

//--MOTOR-PWM--
const int freq = 8000;
const int channel[NMOTORS] = {0, 1, 2, 3, 4, 5, 6};
const int resolution = 8;


void set_Grip_Cmd(int grip) {
  switch (grip)
  {
    case 0:
      ledcWrite(channel[0], 0);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 0);
      digitalWrite(dir_pin[1], LOW);
      ledcWrite(channel[2], 0);
      digitalWrite(dir_pin[2], LOW);
      // ledcWrite(channel[3], 0);
      // digitalWrite(dir_pin[3], LOW);
      // ledcWrite(channel[4], 0);
      // digitalWrite(dir_pin[4], LOW);
      // ledcWrite(channel[5], 0);
      // digitalWrite(dir_pin[5], LOW);
      ledcWrite(channel[6], 0);
      digitalWrite(dir_pin[6], LOW);
      break;

    case 1:
      ledcWrite(channel[0], 200);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 200);
      digitalWrite(dir_pin[1], HIGH);
      //Serial.println("bevel");
      break;

    case 2:
      ledcWrite(channel[0], 200);
      digitalWrite(dir_pin[0], HIGH);
      ledcWrite(channel[1], 200);
      digitalWrite(dir_pin[1], LOW);
      //Serial.println("bevel");
      break;

    /*case 3:
      ledcWrite(channel[0], 200);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 200);
      digitalWrite(dir_pin[1], LOW);
      //Serial.println("bevel");
      break;

    case 4:
      ledcWrite(channel[0], 200);
      digitalWrite(dir_pin[0], HIGH); 
      ledcWrite(channel[1], 200);
      digitalWrite(dir_pin[1], HIGH);
      //Serial.println("bevel");
      break;*/

    case 5:
      ledcWrite(channel[2], 255);
      digitalWrite(dir_pin[2], HIGH);
      //Serial.println("grip");
      break;

    case 6:
      ledcWrite(channel[2], 255);
      digitalWrite(dir_pin[2], LOW);
      //Serial.println("grip");
      break;

    case 3:
      ledcWrite(channel[6], 255);
      digitalWrite(dir_pin[6], LOW);
      break;
    
    case 4:
      ledcWrite(channel[6], 255);
      digitalWrite(dir_pin[6], HIGH);
      break;

    default:
      ledcWrite(channel[0], 0);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 0);
      digitalWrite(dir_pin[1], LOW);
      ledcWrite(channel[2], 0);
      digitalWrite(dir_pin[2], LOW);
      // ledcWrite(channel[3], 0);
      // digitalWrite(dir_pin[3], LOW);
      // ledcWrite(channel[4], 0);
      // digitalWrite(dir_pin[4], LOW);
      // ledcWrite(channel[5], 0);
      // digitalWrite(dir_pin[5], LOW);
      //ledcWrite(channel[6], 0);
      //digitalWrite(dir_pin[6], LOW);
      break;
  }
}
void beginSM(int X, int Y, int Z)
{
  if (X < 0  || Y < 0 || Z < 0)
    {
      digitalWrite(dir_pin[5], LOW);
      digitalWrite(dir_pin[3], LOW);
      digitalWrite(dir_pin[4], LOW);
      int s = map(abs(X), 0, 11, 0, 200);
      int l1 = map(abs(Y), 5, 10, 0, 255);
      int l2 = map(abs(Z), 0, 11, 0, 255);
      ledcWrite(channel[5], l2);
      ledcWrite(channel[3], s);
      ledcWrite(channel[4], l1);
    }
  else if (X > 0 || Y > 0 || Z > 0)
    {
      digitalWrite(dir_pin[5], HIGH);
      digitalWrite(dir_pin[3], HIGH);
      digitalWrite(dir_pin[4], HIGH);
      int s = map(abs(X), 0, 11, 0, 200);
      int l1 = map(abs(Y), 5, 10, 0, 255);
      int l2 = map(abs(Z), 0, 11, 0, 255);
      ledcWrite(channel[5], l2);
      ledcWrite(channel[3], s);
      ledcWrite(channel[4], l1);

    }
  else
    {
      digitalWrite(dir_pin[5], LOW);
      digitalWrite(dir_pin[3], LOW);
      digitalWrite(dir_pin[4], LOW);
      ledcWrite(channel[5], 0);
      ledcWrite(channel[3], 0);
      ledcWrite(channel[4], 0);
    }
}

void setup() 
{
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 3, -1) ;   // Use default serial for debug output
  //Sender.begin(115200, SERIAL_8N1, 2, 0);
  for (int k = 0; k < NMOTORS; k++)
  {
    pinMode(dir_pin[k], OUTPUT);
    ledcSetup(channel[k], freq, resolution);
    ledcAttachPin(pwm_pin[k], channel[k]);
    ledcWrite(channel[k], 0);
  }
  Serial.println("Sare pins set hogaye!");
}

void loop()
{
  //  ledcWrite(channel[2], 255);
  if (SerialPort.available())
  {
    while (SerialPort.available())
    {
      // char rec = SerialPort.read();
      // Serial.println(rec);
      // set_Grip_Cmd(rec);
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
    char *C_index = strchr(rxBuffer,'C');  // cache
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
      Sender.print(rxBuffer);
      beginSM(X,Y,Z);
      set_Grip_Cmd(grip);
    }
  }
  bufferIndex = 0;
}
