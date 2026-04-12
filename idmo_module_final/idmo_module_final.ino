//#include <HardwareSerial.h>

//--ESP32-PINS--
#define Left_PWM 16 //J4
#define Left_DIR 26
#define Right_PWM 18 //J2
#define Right_DIR 33
#define Grip_PWM 17 //J3
#define Grip_DIR 25

//dir 26 pwm 16 MD 4
//23 16 MD5
//14 22 MD6
//13 21 MD 7

const int pwm_pin[] = {Left_PWM , Right_PWM , Grip_PWM };
const int dir_pin[] = {Left_DIR , Right_DIR , Grip_DIR };

//--SERIAL--
HardwareSerial SerialPort(0); // use UART0
const int BUFFER_SIZE = 128;
char rxBuffer[BUFFER_SIZE];
int bufferIndex = 0;

//--NUMBER-OF-MOTORS--
#define NMOTORS 3
//int target[NMOTORS];

//--MOTOR-PWM--
const int freq = 5000;
const int channel[NMOTORS] = {0, 1, 2};
const int resolution = 8;

// Globals
//long prevT = 0;
//volatile int posi[] = {0, 0, 0};

// PID class instances
//PID pid[NMOTORS];

void set_Grip_Cmd(char grip) {
  switch (grip)
  {
    case 'Q': // safety
      ledcWrite(channel[0], 0);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 0);
      digitalWrite(dir_pin[1], LOW);
      ledcWrite(channel[2], 0);
      digitalWrite(dir_pin[2], LOW);
      break;

    case 'W':
     // Serial.println("Pitch Down");
      ledcWrite(channel[0], 100);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 100);
      digitalWrite(dir_pin[1], HIGH);
      break;

    case 'E':
    //  Serial.println("Pitch Up");
      ledcWrite(channel[0], 100);
      digitalWrite(dir_pin[0], HIGH);
      ledcWrite(channel[1], 100);
      digitalWrite(dir_pin[1], LOW);
      break;

    case 'R':
      for (int i = 0; i<30;i++){
    //  Serial.println("Roll Right");
      ledcWrite(channel[0], 100);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 100);
      digitalWrite(dir_pin[1], LOW);}
      break;

    case 'T':
//      for (int i = 0; i<30;i++){
    //  Serial.println("Roll Left");
      ledcWrite(channel[0], 100);
      digitalWrite(dir_pin[0], HIGH);  

      ledcWrite(channel[1], 100);
      digitalWrite(dir_pin[1], HIGH);
//      }
      break;

     case 'Y': // safety
      
    //  Serial.println("Gripperzr Close");
      ledcWrite(channel[2], 255);
      digitalWrite(dir_pin[2], HIGH);
      break;

    case 'U': // safety

      //Serial.println("Gripperr Open");
      ledcWrite(channel[2], 255);
      digitalWrite(dir_pin[2], LOW);
      break;

    case 'I': // safety
      // Serial.println("Reset");
      ledcWrite(channel[0], 0);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 0);
      digitalWrite(dir_pin[1], LOW);
      ledcWrite(channel[2], 0);
      digitalWrite(dir_pin[2], LOW);
      break;

    default:
    //  Serial.println("0");
      ledcWrite(channel[0], 0);
      digitalWrite(dir_pin[0], LOW);
      ledcWrite(channel[1], 0);
      digitalWrite(dir_pin[1], LOW);
      ledcWrite(channel[2], 0);
      digitalWrite(dir_pin[2], LOW);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 1, 3) ;   // Use default serial for debug output
  delay(500);

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
  if (SerialPort.available())
  {
    while (SerialPort.available())
    {
      char rec = SerialPort.read();
      Serial.println(rec);
      set_Grip_Cmd(rec);
    }}
}
