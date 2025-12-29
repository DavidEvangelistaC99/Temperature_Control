//Define PIN PWM
int brigth = 0;     //Variable 0-255 (8 bits)
int pin_PWM = 16;    //Output Pin

//Define PIN L298N
//OUT1 -> (+)
//OUT2 -> (-)
int IN1 = 9; 
int IN2 = 10;

//PWM Characteristics
const int channel = 0;
const int frequency = 5000;
const int resolution = 8;

void setup() {
  ledcSetup(channel,frequency,resolution);
  ledcAttachPin(pin_PWM,channel);
}

void loop() {
  for(brigth=0;brigth<=255;brigth++){
    ledcWrite(channel,brigth);
    delay(15);
    }
  for(brigth=255;brigth>=0;brigth--){
    ledcWrite(channel,brigth);
    delay(15);
    }
}
