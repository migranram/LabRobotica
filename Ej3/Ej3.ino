unsigned long PERIODO_MUESTREO_MS = 100;  // Periodo de muestreo para control (ms).

//Ultrasonidos
const int Echo1 = 11;
const int Echo2 = 12;
const int Trig1 = 9;
const int Trig2 = 10;

//Motor Derecho
const int IN1 = 25;
const int IN2 = 26;
const int ENA = 44;

//Motor Izquierdo
const int IN3 = 27;
const int IN4 = 28;
const int ENB = 45;

//MODO DE FUNCIONAMIENTO
int modo = 0;

//Variables de funcionamiento
bool puttyReady = false;
int tact, tant; //Tiempo entre ciclos
float distStop;
float dist1, old_dist1;
float dist2, old_dist2;
int vel1 = 0, vel2 = 0;
int tmAct, tmAnt;
float tm;
float maxDistDif = 0.5; //Threshold para parar el vehiculo en el modo 1 - Histeresis
float ping1, ping2;
float measureDiff = 0.2; //Maxima diferencia entre medidas para el filtro (malfuncionamiento sensor)
int mode = 0;

float dist1Sum, dist2Sum;
int nMed;
int velUmbral = 90;
int velMinG = 85;

// Modo 1 mucho mas sencillo sin controlador PID -  Solo control todo - nada
void modo1() {
  PERIODO_MUESTREO_MS = 50;
  vel1 = 130; //110

  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;
  //col1 = round(controlador1(distStop, dist1, tm));
  col2 = round(controlador2(dist2, dist1, tm));
  vel2 = col2 + vel1;
}

// Modo 2 con PID para cada uno de los motores -  Suponemos que estan desacoplados y funciona bastanta bien.
void modo2() { 
  float distMedia = (dist1+dist2)/2;
  PERIODO_MUESTREO_MS = 50;
  vel1 = 130;

  int col1, col2;
  int velMin1 = velMinG;
  int velMin2 = velMinG;
  col1 = round(controlador1(distStop, distMedia, tm));
  col2 = round(controlador2(dist2, dist1, tm));
  vel2 = col1 + col2 + vel1;
}

void setup() {
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial2.begin(38400);

  Serial.begin(9600); //Debug

  Serial.println("SERIAL STARTED"); //Debug

  tact =  tant =  millis(); //Medidas para el envio de datos por Bluetooth
  tmAct = tmAnt = millis(); //Medidas intervalo control

  old_dist1 = dist1 = ping(Trig1, Echo1); //inicializamos las distancias
  old_dist2 = dist2 = ping(Trig2, Echo2);

  dist1Sum = 0;
  dist2Sum = 0;
  nMed = 0;
}

void loop() {
  //Lectura sensores y filtrado
  ping1 = ping(Trig1, Echo1);
  ping2 = ping(Trig2, Echo2);
  filtro(&old_dist1, &dist1, ping1, measureDiff);
  filtro(&old_dist2, &dist2, ping2, measureDiff);

  dist1Sum += dist1;
  dist2Sum += dist2;
  nMed += 1;

  //Hay mas de 2 Bytes listos para ser leidos - Usamos comandos de 3 parametros para configurarlo
  if (Serial2.available() > 2)
    BTread(&modo, &distStop);

  tmAct = millis();
  if ((tmAct - tmAnt) >= PERIODO_MUESTREO_MS) {
    //Eliminamos el ruido de las muestras con un promediado
    dist1 = dist1Sum / nMed;
    dist2 = dist2Sum / nMed;

    //Resetamos las variables
    dist1Sum = 0;
    dist2Sum = 0;
    nMed = 0;

    tm = (tmAct - tmAnt) / 1000.0; //Se pasa a segundos

    switch (modo) {
      case 0:   //Modo 0 - Parado
        vel1 = 0; vel2 = 0;
        break;

      case 1:   //Modo 1 - Avanzar paralelo a la pared
        modo1();
        break;

      case 2:
        modo2(); //Modo 2 - Avanzar paralelo a la pared a distancia dada
        break;
    }

    
    //Telemetría (Putty)

     if (vel2<velUmbral&&vel2>-velUmbral)
      vel2=0;
    
    if (vel1<velUmbral&&vel1>-velUmbral)
      vel1=0;
    
    tact = millis();
    telemetria(tact - tant, dist2, dist1, distStop, mode, vel2, vel1);
    tant = millis();
    moveRobot(vel1, vel2);

    tmAnt = millis();
  }


}
