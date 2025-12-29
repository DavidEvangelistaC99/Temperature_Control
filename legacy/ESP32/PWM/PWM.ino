//Define PIN PWM
int brigth = 255;           //Variable 0-255 (8 bits)
int pin_PWM = 16;           //Output Pin

//Define PIN L298N
//OUT1 -> (+)
//OUT2 -> (-)
int in1 = 32; 
int in2 = 33;

//PWM Characteristics
const int channel = 0;
const int frequency = 30000;
const int resolution = 8;

void setup() {
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(pin_PWM, OUTPUT);

  
  ledcSetup(channel,frequency,resolution);
  ledcAttachPin(pin_PWM,channel);
}

void loop() {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  ledcWrite(channel,brigth);
  /*
  for(int brigth=0;brigth<=255;brigth++){
    ledcWrite(channel,brigth);
    delay(15);
    }
  for(int brigth=255;brigth>=0;brigth--){
    ledcWrite(channel,brigth);
    delay(15);
    }
  */
}
