/***************************************************************************/
// File			  [track.h]
// Author		  [Erik Kuo]
// Synopsis		[Code used for tracking]
// Functions  [MotorWriting, MotorInverter, tracking]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 1 & 6*/
/*feel free to add your own function for convenience*/

/*===========================import variable===========================*/
int extern _Tp;
/*===========================import variable===========================*/

// Write the voltage to motor.
void MotorWriting(double vL, double vR) {
  if (vL > 0.0) {
    digitalWrite(MotorL_I3, HIGH);
    digitalWrite(MotorL_I4, LOW);
  } else {
    digitalWrite(MotorL_I3, LOW);
    digitalWrite(MotorL_I4, HIGH);
    vL = -vL;
  }
  if (vR > 0.0) {
    digitalWrite(MotorR_I1, HIGH);
    digitalWrite(MotorR_I2, LOW);
  } else {
    digitalWrite(MotorR_I1, LOW);
    digitalWrite(MotorR_I2, HIGH);
    vR = -vR;
  }
  analogWrite(MotorR_PWMR, vR);
  analogWrite(MotorL_PWML, vL);

}  // MotorWriting

void reverse() {
    delay(500);
    MotorWriting(255,-255);
    delay(300);
    while(!digitalRead(IRpin_M)) {
        MotorWriting(150,-150);
        delay(10);
    } 
}
void right_turn() {
    delay(500); // Wait until the car goes into the center
    MotorWriting(255,-255);
    delay(200);
    while(!digitalRead(IRpin_M) || digitalRead(IRpin_LL) || digitalRead(IRpin_RR)) {
        MotorWriting(100,-100);
        delay(10);
    }
}
// P/PID control Tracking
int weight[5] = { -2, -1, 0, 1, 2 };
int sensors[5] = { IRpin_LL, IRpin_L, IRpin_M, IRpin_R, IRpin_RR };
void tracking() {
  // TODO: find your own parameters!
  double delta = 0;
  int high_num = 0;
  for (int i = 0; i < 5; i++) {
    delta += weight[i] * digitalRead(sensors[i]);
    high_num += digitalRead(sensors[i]);
  }
  delta /= high_num;
  if (delta > 1) {
    MotorWriting(200, -200);
    return;
  }
  if (delta < -1) {
    MotorWriting(-200, 200);
    return;
  }
  MotorWriting(150 + 80 * delta, 150);
  /*int l2 = digitalRead(IRpin_LL);
  int l1 = digitalRead(IRpin_L);
  int m = digitalRead(IRpin_M);
  int r1 = digitalRead(IRpin_R);
  int r2 = digitalRead(IRpin_RR);
  double _w0;  //
  double _w1;  //
  double _w2;  //
  double _Kp;  // p term parameter
  double _Kd;  // d term parameter (optional)
  double _Ki;  // i term parameter (optional) (Hint: 不要調太大)
  double error = l2 * _w2 + l1 * _w1 + m * _w0 + r1 * (-_w1) + r2 * (-_w2);
  double vR, vL;                // 馬達左右轉速原始值(從PID control 計算出來)。Between -255 to 255.
  double adj_R = 1, adj_L = 1;  // 馬達轉速修正係數。MotorWriting(_Tp,_Tp)如果歪掉就要用參數修正。
  // TODO: complete your P/PID tracking code

  // end TODO
  //MotorWriting(adj_L * vL, adj_R * vR);*/
}  // tracking
