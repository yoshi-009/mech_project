#pragma once

#include "Motor.h"
#include "pin.h"

Motor motorSL(motorSL_A, motorSL_B, motorSL_EN);  //
Motor motorSR(motorSR_A, motorSR_B, motorSR_EN);  //


int ryohold = 1000;

void nagiTrace(){
  int sen_Front;
  sen_Front = analogRead(sensor_Front);  //前のセンサ値を読み取り

  Serial.println(sen_Front);
  
  //取り込み
  if (sen_Front > ryohold)
  {
    motorSL.move(-50);
    motorSR.move(50);
    delay(100);
  }
  //射出
  else if (sen_Front < ryohold)
  {
    motorSL.move(250);
    motorSR.move(-250);
    delay(100);
  }

}