#include "Motor.h"
#include "PidLineTrace.h"
#include "pin.h"
#include "syasyutu.h"

Motor motorL(motorL_A, motorL_B, motorL_EN);
Motor motorR(motorR_A, motorR_B, motorR_EN);

PidLineTrace pid(motorL, motorR);

bool isBlack(int);
bool isWhite(int);
void backLT(int);

const bool debug = false;

//白または黒と判定される閾値（全センサ共通）
int threshold_black = 680;
int threshold_white = 690;

//ボールがあると判定して後退を始める閾値
int threshold_back = 950;
int threshold_back_sensor_black = 400;

//ライントレースの前進スピード
int base_speed = 220;
int base_corr = 20;

//交差点を乗り越えるための前進/後退時間
int delay_millis = 800;

void setPWMFreq() {}

void setup() {
    //通信開始
    Serial.begin(115200);
    // setPWMFrequency(10, PWM_DIV1);
    // setPWMFrequency(11, PWM_DIV1);
    // setPWMFrequency(3, PWM_DIV1);
    // setPWMFrequency(5, PWM_DIV1);

    // PWM周波数の調整（デフォルトでは低すぎて低効率だし，可聴域でもある）

    //モータ出力の補正のためにかける係数
    motorR.p = 1.0;
    motorL.p = 1.0;

    //モータを止めておく
    motorL.move(0);
    motorR.move(0);
}

void loop() {
    //黒線の交差点を踏んだ回数
    static int times = 0;

    int SEN_L, SEN_C, SEN_R, SEN_F;    //センサ値
    SEN_L = analogRead(sensor_L);      //左のセンサ値を読み取り
    SEN_C = analogRead(sensor_C);      //中央のセンサ値を読み取り
    SEN_R = analogRead(sensor_R);      //右のセンサ値を読み取り
    SEN_F = analogRead(sensor_Front);  //前のセンサ値を読み取り

    if (debug) {
        //デバッグ用モード
        // Arduinoのシリアルモニタから任意のモータを自由に動かせる
        if (Serial.available()) {
            Motor motorSL(motorSL_A, motorSL_B, motorSL_EN);
            Motor motorSR(motorSR_A, motorSR_B, motorSR_EN);
            String s[4];
            int speed[4] = {0, 0, 0, 0};
            for (int i = 0; i < 3; i++) s[i] = Serial.readStringUntil(',');
            s[3] = Serial.readStringUntil('\n');
            for (int i = 0; i < 4; i++) {
                speed[i] = s[i].toInt();
                Serial.println(speed[i]);
            }
            Serial.println();
            motorL.move(speed[0]);
            motorR.move(speed[1]);
            motorSL.move(speed[2]);
            motorSR.move(speed[3]);
        }
        delay(1);
        return;
    }

    int speed = base_speed;
    int sen_Ball = analogRead(sensor_Ball);

    double pp_L = motorL.p;
    double pp_R = motorR.p;

    //ボールを回収して，後退する場合の処理
    if (sen_Ball < threshold_back) {
        //モータ出力の元の補正値を記録しておく
        speed = -speed;

        //後退時の偏りに合わせてモータ出力を調整する
        motorL.p = 0.9;
        motorR.p = 1.0;

        //全速力で後退
        backLT(speed);
    } else {
        //ライントレースする
        pid.run(speed);
    }
    //黒線の交差点を踏んだとき
    if (isBlack(SEN_L) && isBlack(SEN_C) && isBlack(SEN_R)) {
        times++;
        Serial.println("black");

        //交差点を乗り越えるまで前進/後退する
        motorL.move(speed);
        motorR.move(speed);
        delay(delay_millis);

        motorL.move(0);
        motorR.move(0);
    }

    //線がないとき
    if (isWhite(SEN_L) && isWhite(SEN_C) && isWhite(SEN_R)) {
        //ボールのエリアに入ったと考えて，まっすぐ前進する
        Serial.println("white");
        motorL.move(speed);
        motorR.move(speed);
    }

    //ボールの有無に応じて回収機構を動かす
    ballsensor();

    //補正値を元に戻す
    motorL.p = pp_L;
    motorR.p = pp_R;

    delay(1);
}

void backLT(int speed) {
    int corr = 0;
    if (analogRead(sensor_Back) > threshold_back_sensor_black) {
        corr = base_corr;
        if (analogRead(sensor_R) > analogRead(sensor_L)) corr = -corr;
    }
    motorL.move(speed + corr);
    motorR.move(speed - corr);
}

bool isBlack(int val) { return val < threshold_black; }
bool isWhite(int val) { return val > threshold_white; }
