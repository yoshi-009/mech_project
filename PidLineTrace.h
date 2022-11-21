#pragma once
#include "Motor.h"
#include "pin.h"

class PidLineTrace {
    private:
    Motor &_motorL, &_motorR;
    int _L, _C, _R;
    int _pErr = 0;
    int _acc = 0;

    void readVal() {
        _L = analogRead(sensor_L);
        _C = analogRead(sensor_C);
        _R = analogRead(sensor_R);
    }

    public:
    int goal = 0;
    int forwardSpeed = 50;
    float kp = 10.0;
    float ki = 1.0;
    float kd = 0.1;
    float dt = 0.01;

    PidLineTrace(Motor& motorL, Motor& motorR)
        : _motorL(motorL),
          _motorR(motorR){

          };
    void run() {
        readVal();
        int err;
        err = abs(goal - _C);
        if (_L < _R) err = -err;
        _acc += float(err) * dt;
        int dif = err - _pErr;
        int speed = kp * err + ki * _acc + kd * dif;
        _motorL.move(forwardSpeed + speed);
        _motorR.move(forwardSpeed - speed);
    }
};
