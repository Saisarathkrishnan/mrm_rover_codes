#define Cache_PWM 16//J13
#define Cache_DIR 17

const int pwm_pin[] = {Cache_PWM};
const int dir_pin[] = {Cache_DIR};

HardwareSerial SerialPort(1);
const int BUFFER_SIZE = 128;
char rxBuffer[BUFFER_SIZE];
int bufferIndex = 0;

#define NMOTORS 1

const int freq = 8000;
const int channel[NMOTORS] = {0};
const int resolution = 8;

void setup() 
{
  Serial.begin(115200);
  SerialPort.begin(115200, SERIAL_8N1, 2, 0);   

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
    while (SerialPort.available() && bufferIndex < BUFFER_SIZE - 1)
    {
    rxBuffer[bufferIndex++] = (char)SerialPort.read();
   }

    // Always null-terminate
    rxBuffer[bufferIndex] = '\0';

    Serial.println(rxBuffer);

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
      int c = atoi(C_index + 1);
      if(c == 1){
        digitalWrite(Cache_DIR, HIGH);
       ledcWrite(channel[0], 120);

      }
      else if(c == 0){
        digitalWrite(Cache_DIR, LOW);
       ledcWrite(channel[0], 0);

      }
      
    }
  }
  bufferIndex = 0;
}
