/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search_Mode, Hault_Mode, SetState]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG  // debug flag
// for RFID
#include <MFRC522.h>
#include <SPI.h>

/*===========================define pin & create module object================================*/
// BlueTooth
// BT connect to Serial1 (Hardware Serial)
// Mega               HC05
// Pin  (Function)    Pin
// 18    TX       ->  RX
// 19    RX       <-  TX
// TB6612, 請按照自己車上的接線寫入腳位(左右不一定要跟註解寫的一樣)
// TODO: 請將腳位寫入下方
#define MotorR_I1 5     // 定義 A1 接腳（右）
#define MotorR_I2 6     // 定義 A2 接腳（右）
#define MotorR_PWMR 12  // 定義 ENA (PWM調速) 接腳
#define MotorL_I3 2     // 定義 B1 接腳（左）
#define MotorL_I4 3     // 定義 B2 接腳（左）
#define MotorL_PWML 7   // 定義 ENB (PWM調速) 接腳
// 循線模組, 請按照自己車上的接線寫入腳位
#define IRpin_LL 32
#define IRpin_L 34
#define IRpin_M 36
#define IRpin_R 38
#define IRpin_RR 40
// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 9                  // 讀卡機的重置腳位
#define SS_PIN 53                  // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*=====Import header files=====*/
#include "RFID.h"
#include "bluetooth.h"
#include "node.h"
#include "track.h"
/*=====Import header files=====*/

/*============setup============*/
void setup() {
  // bluetooth initialization
  BT.begin(9600);
  // Serial window
  Serial.begin(9600);
  // RFID initial
  SPI.begin();
  mfrc522.PCD_Init();
  // TB6612 pin
  pinMode(MotorR_I1, OUTPUT);
  pinMode(MotorR_I2, OUTPUT);
  pinMode(MotorL_I3, OUTPUT);
  pinMode(MotorL_I4, OUTPUT);
  pinMode(MotorL_PWML, OUTPUT);
  pinMode(MotorR_PWMR, OUTPUT);
  // tracking pin
  pinMode(IRpin_LL, INPUT);
  pinMode(IRpin_L, INPUT);
  pinMode(IRpin_M, INPUT);
  pinMode(IRpin_R, INPUT);
  pinMode(IRpin_RR, INPUT);
#ifdef DEBUG
  Serial.println("Start!");
#endif
}
/*============setup============*/

/*===========================initialize variables===========================*/
int l2 = 0, l1 = 0, m0 = 0, r1 = 0, r2 = 0;  // 紅外線模組的讀值(0->white,1->black)
int _Tp = 90;                                // set your own value for motor power
bool state = false;                          // set state to false to halt the car, set state to true to activate the car
BT_CMD cmd = NOTHING;                        // enum for bluetooth message, reference in bluetooth.h line 2
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();    // search graph
void SetState();  // switch the state
bool is_node();
/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/
BT_CMD command = NOTHING;
bool start = true;// if true, then request command until Bluetooth send some commands
bool in_node = false;
void loop() {
  if (start){
    MotorWriting(0,0);
    command = ask_BT();
    if (command != NOTHING) {
      start = false;
      state = true;
    }
  }
  if (command == STOP) {//Haven't used yet(May be unnecessary)
    state = false;
    MotorWriting(0,0);
  }
  //Read RFID constantly
  byte  idSize;
  byte* id = rfid(idSize);
  if (id) {
    send_byte(id, idSize);
  }
  if (state) {
    tracking();
    if (is_node() && !in_node) {//when you enter a node
      in_node = true;
      command = ask_BT();
      Serial.println("entered a node");
      if (command == LEFT){
        left_turn();
      }
      else if (command == FORWARD) {
        forward();
      }
      else if (command == REVERSE) {
        reverse();
      }
      else if (command == RIGHT) {
        right_turn();
      }
      command = TRACKING;
      tracking();
    }
    if (!is_node() && in_node) {//when you exit a node
      in_node = false;
      Serial.println("exit node");
    }
    delay(100);
  }
}

bool is_node() {
  if (digitalRead(IRpin_LL) && digitalRead(IRpin_L) && digitalRead(IRpin_M) && digitalRead(IRpin_R) && digitalRead(IRpin_RR))
    return true;
  else
    return false;
}
void SetState() {
  return;
}

void Search() {

  return;
  // TODO: let your car search graph(maze) according to bluetooth command from computer(python
  // code)
}
/*===========================define function===========================*/
