#pragma once
#include "Motor.h"
#include "pin.h"

class PidLineTrace {
    private:
    Motor &_motorL, &_motorR;
    int _L, _C, _R;
    int _pErr = 0;
    int _acc = 0;
    const int _threshold = 800;

    void readVal() {
        _L = analogRead(sensor_L);
        _C = analogRead(sensor_C);
        _R = analogRead(sensor_R);
    }

    public:
    int goal = 670;
    float kp = 0.8;
    float ki = 0.2;
    float kd = 0.6;
    float dt = 0.001;

    PidLineTrace(Motor& motorL, Motor& motorR)
        : _motorL(motorL),
          _motorR(motorR){

          };
    void run(int forward_speed) {
        readVal();
        int err;
        err = _L - _R;
        _acc += float(err) * dt;
        int dif = err - _pErr;
        int speed = kp * err + ki * _acc + kd * dif;
        // int _F = analogRead(sensor_Front);
        // Serial.print(_L);
        // Serial.print(" ");
        // Serial.print(_R);
        // Serial.print(" ");
        // Serial.print(_F);
        // Serial.print(" ");
        // Serial.print(_C);
        // Serial.print(" ");
        // Serial.println(680);
        // Serial.print(err);
        // Serial.print(" ");
        // Serial.println(speed);
        if (forward_speed < 0) {
            _motorL.move(forward_speed + speed);
            _motorR.move(forward_speed - speed);
        } else {
            _motorL.move(forward_speed - speed);
            _motorR.move(forward_speed + speed);
        }
    }
};
