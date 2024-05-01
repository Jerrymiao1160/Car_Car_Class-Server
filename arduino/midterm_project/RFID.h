/***************************************************************************/
// File       [RFID.h]
// Author     [Erik Kuo]
// Synopsis   [Code for getting UID from RFID card]
// Functions  [rfid]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

/*===========================don't change anything in this file===========================*/

#include <MFRC522.h>  // 引用程式庫
#include <SPI.h>
#include <string.h>
/* pin---- SDA:9 SCK:13 MOSI:11 MISO:12 GND:GND RST:define on your own  */

String rfid(byte& idSize) {
  // 確認是否有新卡片
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte* id = mfrc522.uid.uidByte;  // 取得卡片的UID
    idSize = mfrc522.uid.size;       // 取得UID的長度
  
    String S;
    for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
      
      int omega = int(id[i]);
      int gamma = omega/16;
      int delta = omega%16;
      if(gamma<=9){
        S = S + String(char(gamma+48));
      }
      else if(gamma>=10){
        S = S + String(char(gamma+55));
      }
      if(delta<=9){
        S = S + String(char(delta+48));
      }
      else if(delta>=10){
        S = S + String(char(delta+55));
      }
    }
// Serial.print("PICC type: ");      // 顯示卡片類型
//  根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
// MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
// Serial.println(mfrc522.PICC_GetTypeName(piccType));
#ifdef DEBUG
    Serial.print("UID Size: ");  // 顯示卡片的UID長度值
    Serial.println(idSize);
    //String K;
    for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
      Serial.print("id[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(id[i], HEX);  // 以16進位顯示UID值
      /*
      int omega = int(id[i]);
      int gamma = omega/16;
      int delta = omega%16;
      if(gamma<=9){
        K = K + String(char(gamma+48));
      }
      else if(gamma>=10){
        K = S + String(char(gamma+55));
      }
      if(delta<=9){
        S = S + String(char(delta+48));
      }
      else if(delta>=10){
        S = S + String(char(delta+55));
      }
      */
    }
    Serial.println(S);
#endif
    mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
    Serial.print("sent:");
    Serial.println(S);
    return S + ' ';
  }
  return "";
}
