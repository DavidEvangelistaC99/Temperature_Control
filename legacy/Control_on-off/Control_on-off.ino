// Incluimos librería
#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 2 // Definimos el pin digital donde se conecta el sensor
#define DHTTYPE DHT11 // Dependiendo del tipo de sensor


DHT dht(DHTPIN, DHTTYPE); // Inicializamos el sensor DHT11

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13; // Variables para la conexion del display LCD1602
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


volatile long contador = 0;
int interruptPin = 3;


double K = 60000; // constante
float n = 2; // numero de pulsos por revolución
float tiempo_T = 0; // Tiempo T
float RPM = 0; // revoluciones por minuto

float tiempo_inicial = 0; // tiempo el muestreo para la medicion de las RPM
float pulsos_inicial = 0;
bool tiempo_muestra = false;
float error = 0;


int set = 20;
double signal_control;
int pwm;
float t;
float error_max;
float error_min;

/* Variables para la visualización en puerto Serial */

float time_seconds = 0;

/* Variables para la visualización en el display LCD */

String temp = "T: ";
String RPMS = "RPM";
String setp = "set: ";

void setup() {

  Serial.begin(9600); // Inicializamos comunicación serie
  dht.begin(); // Comenzamos el sensor DHT
  attachInterrupt(digitalPinToInterrupt(interruptPin), lectura_encoder, RISING); // Configuración interrupción
  lcd.begin(16, 2);
}

void loop() {

  delay(3000); // Retraso de lectura debido a las limitaciones del sensor
  t = dht.readTemperature(); // Leemos la temperatura en grados centígrados (por defecto)
  time_seconds = (millis()) / 1000;


  Serial.print("Tiempo en segundos: ");
  Serial.print(time_seconds);
  Serial.print(" Temperatura: ");
  Serial.println(t);

  impirmir_valores();

  error = set - t; // Error set(valor deseado) - t(temperatura leida por el sensor)

  if (error < 0)analogWrite(5, 255); // Enciende ventilador (ON)
  if (error > 0)analogWrite(5, 0); // Apaga ventilador (OFF)
}




float calculate_RPM() { // funciona para calular RPM

  if (tiempo_muestra == true) {

    tiempo_inicial = millis();
      pulsos_inicial = abs(contador);
        tiempo_muestra = false;
  }

   tiempo_T = millis() - tiempo_inicial;



  if (tiempo_T >= 3000) { //


    RPM = K * (abs(contador) - pulsos_inicial) / (n * tiempo_T); // puso final- pulso inicial
      tiempo_muestra = true; // habilita muestra
        contador = 0;
        return RPM;
  }

}

void lectura_encoder() { // funcion de la interrupción

  contador++;
}

void impirmir_valores() { //Imprime valores en el display LCD 1602



  lcd.setCursor(9, 0);
  lcd.print(setp + set);
  
     lcd.setCursor(0, 0);
     lcd.print(temp + t);
     
        lcd.setCursor(7, 1);
        lcd.print("RPM:");
        
          lcd.setCursor(12, 1);
           lcd.print(calculate_RPM(), 3);
           
              lcd.setCursor(0, 1);
                lcd.print("U");
                
                   lcd.setCursor(2, 1);
                    lcd.print(error);

}
