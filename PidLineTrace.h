#pragma once
#include "Motor.h"
#include "pin.h"

class PidLineTrace {
    private:
    Motor &_motorL, &_motorR;
    int _L, _C, _R, _F;          //センサの値
    int _pErr = 0;               //前回の偏差
    int _acc = 0;                //偏差の累積
    const int _threshold = 800;  //閾値

    //センサ値を読み取る
    void readVal() {
        _L = analogRead(sensor_L);
        _C = analogRead(sensor_C);
        _R = analogRead(sensor_R);
        _F = analogRead(sensor_Back);
    }

    public:
    //目標値
    int goal = 70;

    //ゲイン
    float kp = 0.8;
    float ki = 0.1;
    float kd = 0.4;

    //制御周期
    float dt = 0.001;

    PidLineTrace(Motor& motorL, Motor& motorR)
        : _motorL(motorL),
          _motorR(motorR){

          };
    void run(int forward_speed) {
        readVal();
        int err;

        //左センサと右センサとの値の差をラインの位置からのずれと考える
        err = goal - (_C - _R);

        //偏差の累積を計算（積分に相当）
        _acc += float(err) * dt;

        //前回の偏差との差を計算（微分に相当）
        int dif = err - _pErr;

        //これらをゲインをかけて足すことで補正値を得る
        int speed = kp * err + ki * _acc + kd * dif;
        // int _F = analogRead(sensor_Front);
        Serial.print(_L);
        Serial.print(" ");
        // Serial.print(_R);
        // Serial.print(" ");
        // Serial.print(_F);
        // Serial.print(" ");
        Serial.print(_C);
        Serial.print(" ");
        // Serial.println(630);
        Serial.println(err);
        // Serial.print(" ");
        // Serial.println(speed);

        //前進/後退を続けつつ，ラインからの位置を補正
        if (forward_speed < 0) {
            _motorL.move(forward_speed + speed);
            _motorR.move(forward_speed - speed);
        } else {
            _motorL.move(forward_speed - speed);
            _motorR.move(forward_speed + speed);
        }
    }
};
