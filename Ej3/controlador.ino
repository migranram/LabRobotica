//float kp = -4.17540 * 1;
//float kd = -0.93737 * 1;
//float ki = -0.1 * 1;



float controlador1(float Rk, float Yk, float Tm)
{
  float Umax = 5, Umin = -5; /* Valores límite de la señal de control */
  float kp = -0.3; //0.25
  float kd = 0;
  float ki = -0.05;
  
  float Uk, Uks; //Señal de control y señal de control saturada
  float uk; //Señal de control incremental
  float Ueq = 0;
  static float Int_k = 0.0, Int_ks = 0.0;
  static float ek = 0.0, e_k_1 = 0.0;
    
  /*Control*/
  ek = Rk - Yk; //Cálculo del error actual

  //Calculamos la señal de control incremental (PID)
  uk = kp * ek + kd * (ek - e_k_1) / Tm + ki * Int_k;

  e_k_1 = ek; //Actualizamos el error del instante anterior
  Int_k += ek * Tm;
//  if (uk >= 5)
//    Uk = Ueq + uk;
//  else if (uk <= -5)
//    Uk = -Ueq + uk;
//  else
//    Uk = 0;

  Uk=uk;
  /* Saturación de la señal de control */
  if (Uk > Umax) Uks = Umax;
  else if (Uk < Umin) Uks = Umin;
  else Uks = Uk;

  if (Uk >= Umax || Uk <= Umin) //Si la señak de control satura
    Int_k -= ek * Tm;

  return Uks;
}


float controlador2(float Rk, float Yk, float Tm)
{
  float Umax = 70, Umin = -35; /* Valores límite de la señal de control */
  float kp = 15;
  float kd = 0;
  float ki =  0.5;
  float Uk, Uks; //Señal de control y señal de control saturada
  float uk; //Señal de control incremental
  float Ueq = 0;
  static float Int_k = 0.0, Int_ks = 0.0;
  static float ek = 0.0, e_k_1 = 0.0;
  
  /*Control*/
  ek = Rk - Yk; //Cálculo del error actual

  //Calculamos la señal de control incremental (PID)
  uk = kp * ek + kd * (ek - e_k_1) / Tm + ki * Int_k;

  e_k_1 = ek; //Actualizamos el error del instante anterior
  Int_k += ek * Tm;
  
//  if (uk >= 5)
//    Uk = Ueq + uk;
//  else if (uk <= -5)
//    Uk = -Ueq + uk;
//  else
//    Uk = 0;

  Uk=uk;
  /* Saturación de la señal de control */
  if (Uk > Umax) Uks = Umax;
  else if (Uk < Umin) Uks = Umin;
  else Uks = Uk;

  if (Uk >= Umax || Uk <= Umin) //Si la señak de control satura
    Int_k -= ek * Tm;

  return Uks;
}
