#include "Motor.h"
#include "PidLineTrace.h"
#include "pin.h"
#include "syasyutu.h"

Motor motorL(motorL_A, motorL_B, motorL_EN);
Motor motorR(motorR_A, motorR_B, motorR_EN);

PidLineTrace pid(motorL, motorR);

bool isBlack(int);
bool isWhite(int);
void turn(int);

bool debug = false;
int threshold_black = 680;
int threshold_white = 630;
int threshold_back = 950;
int base_speed = 180;
int count = 0;
const int dmilis = 2000;

void setup() {
    Serial.begin(115200);
    motorL.move(0);
    motorL.p = 1.0;
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
        delay(1);
    } else {
        bool is_back = false;
        int speed = base_speed;
        int sen_Ball = analogRead(sensor_Ball);

        if (sen_Ball < threshold_back) speed = -speed;

        pid.run(speed);

        if (isBlack(SEN_L) && isBlack(SEN_C) && isBlack(SEN_R) &&
            isBlack(SEN_F)) {
            //     times++;
            Serial.println("black");
            //     turn(-1);
            switch (times) {
                // case 3:
                //     turn(1);  // right
                //     break;
                // case 5:
                //     turn(-1);  // left
                //     break;
                default:
                    motorL.move(speed);
                    motorR.move(speed);
                    delay(800);
                    motorL.move(0);
                    motorR.move(0);
                    break;
            }
        }
        // if (times == 5) ballsensor();
        if (isWhite(SEN_L) && isWhite(SEN_C) && isWhite(SEN_R) &&
            isWhite(SEN_F)) {
            Serial.println("white");
            motorL.move(speed);
            motorR.move(speed);
        }
        if (sen_Ball < threshold_back) {
            double pp_L = motorL.p;
            double pp_R = motorL.p;
            motorL.p = 1.0;
            motorR.p = 0.8;
            motorL.move(-255);
            motorR.move(-255);
            motorL.p = pp_L;
            motorR.p = pp_R;
        }
        ballsensor();
        // lineTrace(threshold);
        delay(1);
    }
}

void turn(int dir) {
    int SEN_F;
    //    do {
    motorR.move(150 * dir);
    motorL.move(-150 * dir);
    delay(dmilis);
    //        SEN_F = analogRead(sensor_Front);
    //    } while (!isBlack(SEN_F));
    motorL.move(0);
    motorR.move(0);
}

bool isBlack(int val) { return val < threshold_black; }
bool isWhite(int val) { return val > threshold_white; }
