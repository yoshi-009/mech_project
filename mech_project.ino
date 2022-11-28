#include "Motor.h"
#include "PidLineTrace.h"
#include "pin.h"
#include "syasyutu.h"

Motor motorL(motorL_A, motorL_B, motorL_EN);
Motor motorR(motorR_A, motorR_B, motorR_EN);

PidLineTrace pid(motorL, motorR);

bool isBlack(int);
void turn(int);

bool debug = false;
int threshold = 100;
int count = 0;

void setup() {
    Serial.begin(115200);
    motorL.move(0);
    motorR.move(0);
}

void loop() {
    static int times = 0;

    int SEN_L, SEN_C, SEN_R, SEN_F;
    SEN_L = analogRead(sensor_L);      //左のセンサ値を読み取り
    SEN_C = analogRead(sensor_C);      //中央のセンサ値を読み取り
    SEN_R = analogRead(sensor_R);      //右のセンサ値を読み取り
    SEN_F = analogRead(sensor_Front);  //前のセンサ値を読み取り

    if (debug) {
        if (Serial.available()) {
            String s1, s2;
            s1 = Serial.readStringUntil(',');
            s2 = Serial.readStringUntil('\n');
            int speed1 = s1.toInt();
            int speed2 = s2.toInt();
            Serial.print(speed1);
            Serial.print(",");
            Serial.println(speed2);
            motorL.move(speed1);
            motorR.move(speed2);
        }
    } else {
        if (isBlack(SEN_C) && isBlack(SEN_R) && isBlack(SEN_F)) {
            times++;
            Serial.println(times);
            switch (times) {
                case 3:
                    turn(1);  // right
                    break;
                case 5:
                    turn(-1);  // left
                    break;
                default:
                    motorL.move(200);
                    motorR.move(200);
                    delay(500);
                    motorL.move(0);
                    motorR.move(0);
                    break;
            }
        }
        if (times == 5) ballsensor();
        pid.run();
        // lineTrace(threshold);
        delay(5);
    }
}

void lineTrace(int threshold) {
    int SEN_L, SEN_C, SEN_R;
    SEN_L = analogRead(sensor_L);  //左のセンサ値を読み取り
    SEN_C = analogRead(sensor_C);  //中央のセンサ値を読み取り
    SEN_R = analogRead(sensor_R);  //右のセンサ値を読み取り

    Serial.print(SEN_L);
    Serial.print(" ");
    Serial.print(SEN_C);
    Serial.print(" ");
    Serial.println(SEN_R);

    //直進「(左右のセンサが白)かつ(中央のセンサが黒)」または「（左右センサのどちらかが黒）かつ（中央のセンサが黒）」
    if (((SEN_L < threshold) && (SEN_R < threshold) && (SEN_C > threshold)) ||
        ((SEN_L > threshold) && (SEN_R < threshold) && (SEN_C > threshold)) ||
        ((SEN_L < threshold) && (SEN_R > threshold) && (SEN_C > threshold))) {
        motorL.move(200);
        motorR.move(200);
        delay(100);
    }
    //左へ(左のセンサが黒) かつ (中央のセンサが白) かつ (右のセンサが白)
    else if ((SEN_L >= threshold) && (SEN_C < threshold) &&
             (SEN_R < threshold)) {
        motorL.move(-20);
        motorR.move(200);
        delay(100);
    }
    //右へ (左のセンサが白) かつ (中央のセンサが白) かつ (右のセンサが黒)
    else if ((SEN_L < threshold) && (SEN_C < threshold) &&
             (SEN_R >= threshold)) {
        motorL.move(200);
        motorR.move(-20);
        delay(100);
    }

    //線を踏む(すべてのセンサが黒)
    else if ((SEN_L > threshold) && (SEN_C > threshold) &&
             (SEN_R > threshold)) {
        count = count + 1;
        if (count == 2) {
            motorL.move(200);
            motorR.move(200);
            delay(1000);
        } else if (count == 2) {
            motorL.move(0);
            motorR.move(0);
        }
    }
}

void turn(int dir) {
    int SEN_F;
    do {
        motorR.move(-200 * dir);
        motorL.move(200 * dir);
        SEN_F = analogRead(sensor_Front);
    } while (!isBlack(SEN_F));
    motorL.move(0);
    motorR.move(0);
}

bool isBlack(int val) { return val > threshold; }
