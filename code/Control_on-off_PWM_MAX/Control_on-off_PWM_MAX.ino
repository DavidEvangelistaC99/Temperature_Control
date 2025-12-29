//***PWM MAX***//
#include <Adafruit_MAX31856.h>
#define DRDY_PIN 4

Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(5, 23, 19, 18);

int pin_PWM = 16;          //PWM Pin

//Define PIN L298N
//OUT1 -> (+)
//OUT2 -> (-)
int in1 = 32; 
int in2 = 33;

/////////////////

int set = 40;
float t;
float error = 0;

void setup() {
  
  //***PWM MAX***//
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  pinMode(pin_PWM, OUTPUT);
  
  Serial.begin(9600);
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
  
  /////////////////
  
}

void loop() {
  
  //***PWM MAX***//
  // The DRDY output goes low when a new conversion result is available
  int count = 0;
  while (digitalRead(DRDY_PIN)) {
    if (count++ > 200) {
      count = 0;
      Serial.print(".");
    }
  }

  /////////////////

  delay(1000);
  t = maxthermo.readThermocoupleTemperature(); // Leemos la temperatura en grados centígrados

  Serial.print(set);
  Serial.print(",");
  Serial.print(t);
  Serial.println();

  //20-([-80;20])
  //20-37
  //set = 20
  error = set - t; // Error set(valor deseado) - t(temperatura leida por el sensor)

  if (error < 0)analogWrite(pin_PWM, 255);  // Apaga Heating Pad (OFF)
  if (error > 0)analogWrite(pin_PWM, 0);    // Enciende Heating Pad  (ON)
}
