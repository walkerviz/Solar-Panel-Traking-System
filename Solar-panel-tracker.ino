#include <Servo.h>

const int LDR_TOP_LEFT = A0;
const int LDR_TOP_RIGHT = A1;
const int LDR_BOTTOM_LEFT = A2;
const int LDR_BOTTOM_RIGHT = A3;
const int LIGHT_THRESHOLD = 10;

Servo servo_horizontal;
Servo servo_vertical;
int pos_sh = 90;
int pos_sv = 90;
const int UPPER_LIMIT_POS = 160;  //Límite superior de los servos
const int LOWER_LIMIT_POS = 20;   //Límite inferior de los servos

void setup() {
  Serial.begin(115200);
  servo_vertical.attach(5);
  servo_horizontal.attach(6);
  servo_vertical.write(90);
  servo_horizontal.write(90);
  pos_sv = servo_vertical.read();
  pos_sh = servo_horizontal.read();
}

void loop() {
  int ldr_tl_value = analogRead(LDR_TOP_LEFT) / 10;
  int ldr_tr_value = analogRead(LDR_TOP_RIGHT) / 10;
  int ldr_bl_value = analogRead(LDR_BOTTOM_LEFT) / 10;
  int ldr_br_value = analogRead(LDR_BOTTOM_RIGHT) / 10;

  int average_top = (ldr_tl_value + ldr_tr_value) / 2; //Media de los 2 LDR de arriba
  int average_bottom = (ldr_bl_value + ldr_br_value) / 2; //Media de los 2 LDR de abajo
  int average_left = (ldr_tl_value + ldr_bl_value) / 2; //Media de los 2 LDR de la izquierda
  int average_right = (ldr_tr_value + ldr_br_value) / 2; //Media de los 2 LDR de la derecha

  moveSolarTracker(average_top, average_bottom, average_left, average_right);
  delay(10);
  Serial.println(String(ldr_tl_value) +", "+String(ldr_tr_value) +", "+String(ldr_bl_value) +", "+String(ldr_br_value));
  Serial.println(String(average_top) +", "+String(average_bottom) +", "+String(average_left) +", "+String(average_right));
}

void moveSolarTracker(int average_top, int average_bottom, int average_left, int average_right) {
  //Movemos el solar tracker hacia arriba o hacia abajo
  if ((average_top - average_bottom) > LIGHT_THRESHOLD && pos_sv < UPPER_LIMIT_POS) {
    pos_sv++;
    servo_vertical.write(pos_sv);
    Serial.println("sv= "+String(pos_sv));
  }
  else if ((average_bottom - average_top) > LIGHT_THRESHOLD && pos_sv > LOWER_LIMIT_POS) {
    pos_sv--;
    servo_vertical.write(pos_sv);
    Serial.println("sv= "+String(pos_sv));
  }

  //Movemos el solar tracker hacia la derecha o hacia la izquierda
  if ((average_left - average_right) > LIGHT_THRESHOLD && pos_sh < UPPER_LIMIT_POS) {
    pos_sh++;
    servo_horizontal.write(pos_sh);
    Serial.println("sh= "+String(pos_sh));
  }
  else if ((average_right - average_left) > LIGHT_THRESHOLD && pos_sh > LOWER_LIMIT_POS) {
    pos_sh--;
    servo_horizontal.write(pos_sh);
    Serial.println("sh= "+String(pos_sh));
  }
}
