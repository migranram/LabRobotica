float ping(int TrgPin, int EchoPin) {
  long duration;
  float distanceCm;

  digitalWrite(TrgPin, LOW);
  delayMicroseconds(4);
  digitalWrite(TrgPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrgPin, LOW);

  duration = pulseIn(EchoPin, HIGH, (unsigned long) 12000); //Timeout de 12 ms

  distanceCm = (float)duration * 10.0 / 292.0 / 2.0;  //convertimos a distancia en cm
  return distanceCm;
}

//Filtro para el ultrasonido. Devuelve 1 si medida valida, 0 si no (En estos modos no lo usamos)
int filtro(float* distAnt, float* distAct, float pingAct, float maxDiff){ 
  int res;
  if(abs(pingAct - *distAnt) <= maxDiff){
      *distAct = pingAct;
      res = 1;
    }
  else{
      *distAct = *distAct;
      res = 0;
    }
  *distAnt = pingAct;
  return res;
}

//Encoder - Calculo de rpm de cada rueda
void medirVelocidad()
{
  unsigned long interv;
  
  timeEncAct = micros();
  interv=timeEncAct-timeEncAnt;

  rpmL = (double)(LenconderCnt*pow(10,6)*60.0)/(interv*4.0*rtL);
  rpmR = (double)(RenconderCnt*pow(10,6)*60.0)/(interv*4.0*rtR); 

  timeEncAnt = micros();
  LenconderCnt = 0;
  RenconderCnt = 0;  
}
