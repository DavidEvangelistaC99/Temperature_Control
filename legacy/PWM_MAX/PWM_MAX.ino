// This example demonstrates continuous conversion mode using the
// DRDY pin to check for conversion completion.

#include <Adafruit_MAX31856.h>
#define DRDY_PIN 4

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(5, 23, 19, 18);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

//**---***//
//Define PIN PWM
int PWM_b = 128;            //Variable 0-255 (8 bits)
int pin_PWM = 16;           //Output Pin

//Define PIN L298N
//OUT1 -> (+)
//OUT2 -> (-)
int in1 = 32; 
int in2 = 33;

//PWM Characteristics
const int channel = 4;
const int frequency = 30000;
const int resolution = 8;
//**---**//

void setup() {
  //**---**//
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  pinMode(pin_PWM, OUTPUT);
  /*
  ledcSetup(channel,frequency,resolution);
  ledcAttachPin(pin_PWM,channel);
  */
  //**---**//
  
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("MAX31856 thermocouple test");

  pinMode(DRDY_PIN, INPUT);

  if (!maxthermo.begin()) {
    Serial.println("Could not initialize thermocouple.");
    while (1) delay(10);
  }

  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);

  Serial.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }

  maxthermo.setConversionMode(MAX31856_CONTINUOUS);
}

void loop() {
  //**---**//
  /*
  ledcWrite(channel,PWM_b);
  */
  analogWrite(pin_PWM, PWM_b);
  //**---**//
  
  // The DRDY output goes low when a new conversion result is available
  int count = 0;
  while (digitalRead(DRDY_PIN)) {
    if (count++ > 200) {
      count = 0;
      Serial.print(".");
    }
  }
  Serial.println(maxthermo.readThermocoupleTemperature());
  delay(10);
}
