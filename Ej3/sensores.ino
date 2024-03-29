//Mismo que en el ejercicio anterior
float ping(int TrgPin, int EchoPin) {
  long duration;
  float distanceCm;

  digitalWrite(TrgPin, LOW);
  delayMicroseconds(4);
  digitalWrite(TrgPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrgPin, LOW);

  duration = pulseIn(EchoPin, HIGH, (unsigned long) 12000); //Timeout de 20 ms = 20000 micro segundos

  distanceCm = (float)duration * 10.0 / 292.0 / 2.0;  //convertimos a distancia en cm
  return distanceCm;
}

int filtro(float* distAnt, float* distAct, float pingAct, float maxDiff){ //Filtro para el ultrasonido. Devuelve 1 si medida valida, 0 si no.
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
