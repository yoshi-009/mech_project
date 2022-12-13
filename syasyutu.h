#pragma once

#include "Motor.h"
#include "pin.h"

Motor motorSL(motorSL_A, motorSL_B, motorSL_EN);  //
Motor motorSR(motorSR_A, motorSR_B, motorSR_EN);  //

int ballthreshold = 900;

void ballsensor() {
    int sen_Ball;
    sen_Ball = analogRead(sensor_Ball);  //前のセンサ値を読み取り

    //取り込み
    if (sen_Ball > ballthreshold) {
        motorSL.move(255);
        motorSR.move(-255);
        // delay(100);
    }
    //射出
    else if (sen_Ball < ballthreshold) {
        // delay(500);
        motorSL.move(0);
        motorSR.move(0);
        // delay(500);
    }
}
