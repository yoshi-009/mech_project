#pragma once

class Motor {
    private:
    const int _A, _B, _EN;

    public:
    float p = 1;

    Motor(int A, int B, int EN) : _A(A), _B(B), _EN(EN) {
        pinMode(_A, OUTPUT);
        pinMode(_B, OUTPUT);
        digitalWrite(_A, LOW);
        digitalWrite(_B, LOW);
        analogWrite(_EN, 0);
        analogWrite(_EN, 0);
    }

    void move(int speed) {
        speed = int(float(speed) * p);

        if (speed > 255) {
            speed = 255;
        } else if (speed < -255) {
            speed   = -255;
        }

        if (speed >= 0) {
            analogWrite(_EN, speed);
            digitalWrite(_A, HIGH);
            digitalWrite(_B, LOW);
        } else {
            analogWrite(_EN, -speed);
            digitalWrite(_A, LOW);
            digitalWrite(_B, HIGH);
        }
    }
};
