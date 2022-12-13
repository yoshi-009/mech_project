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
    int goal = 700;
    float kp = 0;
    float ki = 0.00;
    float kd = 0.0;
    float dt = 0.001;

    PidLineTrace(Motor& motorL, Motor& motorR)
        : _motorL(motorL),
          _motorR(motorR){

          };
    void run(int forwardSpeed) {
        readVal();
        int err;
        err = -_L + _R;
        _acc += float(err) * dt;
        int dif = err - _pErr;
        int speed = kp * err + ki * _acc + kd * dif;
        // Serial.print(_L);
        // Serial.print(" ");
        // Serial.print(_R);
        // Serial.print(" ");
        // Serial.print(_C);
        // Serial.print(" ");
        // Serial.println(err);
        // Serial.print(" ");
        // Serial.println(speed);
        if (forwardSpeed < 0) {
            _motorL.move(forwardSpeed + speed);
            _motorR.move(forwardSpeed * 1.4 - speed);
        } else {
            _motorL.move(forwardSpeed * 1.3 - speed);
            _motorR.move(forwardSpeed + speed);
        }
    }
};
