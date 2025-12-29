// This example demonstrates continuous conversion mode using the
// DRDY pin to check for conversion completion.

#include <Adafruit_MAX31856.h>
#define DRDY_PIN 4

//**--Fuzzy--**//
#include <fuzzy.h>
fuzzy controlfuzzy;

//**--Fuzzy--**//
//OJO: Ajustar los conjuntos de acuerdo a lo que se quiere alcanzar 
//Definimos conjunto para variable de entrada
float UIN[] = {-40,40};              // Definimos universo de discurso de la variable de entrada
float ENP[] = {-40,-25,0,'T'};       // Definimos los conjuntos difusos, colocando 'T' si es triangular, o 'R' si es trapezoidal
float EC[] = {-15,0,15,'T'};        // Conjuntos difusos de entrada
float EPP[] = {0,25,40,'T'};

// Definimos conjuntos variable de salida
float USAL[] = {-100,100}; 
float DT[] = {-100,-50,0,'T'};
float ZE[] = {-25,0,25,'T'};  
float AT[] = {0,25,100,'T'};

float paso = 2;                     // Se define la cantidad mínima de variación para los numeros del conjunto de salida
                                    // OJO: La entrada no debe llegar a los valores extremos de su conjunto
float setpoint = 35, PWM, error;
const int tam = controlfuzzy.calc_size(USAL,paso); // Tamaño del vector del conjunto de salida con universo salida y "paso"

//**--MAX--**//
// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(5, 23, 19, 18);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

//**---***//
//Define PIN PWM
int PWM_bit;                        //Variable 0-255 (8 bits)
int pin_PWM = 16;                   //Output Pin

//Define PIN L298N
//OUT1 -> (+)
//OUT2 -> (-)
int in1 = 32; 
int in2 = 33;

//PWM Characteristics
const int channel = 0;
const int frequency = 30000;
const int resolution = 8;
//**---**//

void setup() {
  //**---**//
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(pin_PWM, OUTPUT);
  ledcSetup(channel,frequency,resolution);
  ledcAttachPin(pin_PWM,channel);
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
  
  // The DRDY output goes low when a new conversion result is available
  int count = 0;
  while (digitalRead(DRDY_PIN)) {
    if (count++ > 200) {
      count = 0;
      Serial.print(".");
    }
  }

  //**--Fuzzy--**//
  PWM = maxthermo.readThermocoupleTemperature();
  Serial.println(PWM);
  float B[tam];                     // Creamos el vector que guardará los numeros del conjunto de salida
  controlfuzzy.inicio(B,tam);       // Inicializamos el objeto fuzzy
  error = setpoint - PWM;           // Calculamos la variable error
  /*
   * Se empieza con la inferencia, y para ello las reglas. Para llamar a una regla con una entrada se utiliza la función "regla_simple"
   * la cual recibe como parametros (Conjunto entrada,Universo de entrada,Variable a evaluar,Conjunto salida,Universo salida,Vector salida,Tamaño
   * vector salida)
   */
   
  //Regla correspondiente
  controlfuzzy.regla_simple(ENP,UIN,error,DT,USAL,B,tam);
  controlfuzzy.regla_simple(EC,UIN,error,ZE,USAL,B,tam);
  controlfuzzy.regla_simple(EPP,UIN,error,AT,USAL,B,tam);
  
  //Desfusificación
  float res = controlfuzzy.defusi(B,USAL,tam);
  //PWM = PWM + res;                  // Se asigna ese resultado a la variable control.
  /*
   * En este ejemplo el valor de rpm deberia seguir el valor definido en setpoint
   */
  PWM_bit = int(res*(255/100));
  //**---**//
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  ledcWrite(channel,PWM_bit);
  //**---**//

}
