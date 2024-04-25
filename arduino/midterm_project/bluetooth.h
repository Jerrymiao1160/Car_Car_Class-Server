/***************************************************************************/
// File			  [bluetooth.h]
// Author		  [Erik Kuo]
// Synopsis		[Code for bluetooth communication]
// Functions  [ask_BT, send_msg, send_byte]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

#include <SoftwareSerial.h>
SoftwareSerial BT(11, 10);

enum BT_CMD {
  NOTHING,
  RIGHT,
  LEFT,
  FORWARD,
  REVERSE,
  STOP,
  TRACKING,
};

BT_CMD ask_BT() {
  BT_CMD message = NOTHING;
  char cmd;
  if (BT.available()) {
    cmd = BT.read();
    if (cmd == 'r')
      message = RIGHT;
    if (cmd == 'l')
      message = LEFT;
    if (cmd == 'b')
      message = REVERSE;
    if (cmd == 'f')
      message = FORWARD;
    if (cmd == 's')
      message = STOP;
#ifdef DEBUG
    Serial.print("cmd : ");
    Serial.println(cmd);
#endif
  }
  return message;
}  // ask_BT

void send_msg(const char& msg) {
  BT.write(msg);
  BT.write('\n');
}  // send_msg

// send UID back through Serial1(bluetooth serial)
void send_byte(byte* id, byte& idSize) {
  for (byte i = 0; i < idSize; i++) {  // Send UID consequently.
    BT.write(id[i]);
  }
#ifdef DEBUG
  Serial.print("Sent id: ");
  for (byte i = 0; i < idSize; i++) {  // Show UID consequently.
    Serial.print(id[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
#endif
}  // send_byte
