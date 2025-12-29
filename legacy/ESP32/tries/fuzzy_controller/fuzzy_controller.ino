/*
 * Programa creado por Elmer Meneses
 * Ingeniero Mecatrónico, Universidad de Pamplona
 * Programa para un control fuzzy con una sola entrada
 */
#include <fuzzy.h>
//Objeto tipo Fuzzy
fuzzy controlfuzzy;
//Definimos conjunto para variable de entrada
float UIN[] = {10,50};              // Definimos universo de discurso de la variable de entrada
float ENP[] = {10,20,30,'T'};       // Definimos los conjuntos difusos, colocando 'T' si es triangular, o 'R' si es trapezoidal
float EC[] = {20,30,40,'T'};        // Conjuntos difusos de entrada
float EPP[] = {30,40,50,'T'};

// Definimos conjuntos variable de salida
float USAL[] = {0,100}; 
float DT[] = {-10,0,35,60,'R'};
float ZE[] = {35,60,85,'T'};  
float AT[] = {60,85,100,110,'R'};

float paso = 2;                     // Se define la cantidad mínima de variación para los numeros del conjunto de salida
                                    // OJO: La entrada no debe llegar a los valores extremos de su conjunto
float setpoint = 30, rpm = 11, error;
const int tam = controlfuzzy.calc_size(USAL,paso); // Tamaño del vector del conjunto de salida con universo salida y "paso"

//Configuración
void setup() { 
  Serial.begin(115200);
}

void loop() {
  float B[tam];                     // Creamos el vector que guardará los numeros del conjunto de salida
  controlfuzzy.inicio(B,tam);       // Inicializamos el objeto fuzzy
  error = setpoint - rpm;           // Calculamos la variable error
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
  rpm = rpm + res;                  // Se asigna ese resultado a la variable control.
  Serial.println(rpm);              //Se imprime el valor de rpm que debería converger al valor de setpoint
	delay(500);
  /*
   * En este ejemplo el valor de rpm deberia seguir el valor definido en setpoint
   */
}
