#include <M5StickCPlus.h>

#include <SoftwareSerial.h>

#define COMM_TX 26
#define COMM_RX 25

//const int LED_PIN = 13;
SoftwareSerial sSerial;
void setup() {
  Serial.begin(115200);

  sSerial.begin(19200, SWSERIAL_8N1, COMM_RX, COMM_TX, false);         // シリアル通信の開始(ボーレート9600bps)
  //  pinMode(LED_PIN, OUTPUT);

}
//byte by = 2;
void loop() {
  int val;
  if (Serial.available() > 0) {
    val=Serial.read();
    sSerial.write(val);    
  }
  if (sSerial.available() > 0) {
    val=sSerial.read();
    Serial.write(val);
    m5.Lcd.println(val);
    Serial.println();
  }
////  sSerial.write('1');
//  if (Serial.available() > 0) {
//    int val=sSerial.read();
//    sSerial.println(val);
//    // digitalWrite(LED_PIN, HIGH);
//    // "1"を送信
//    //  Serial.println(Serial.read());
//  }
//
//  // // digitalWrite(LED_PIN, LOW);
//  //  sSerial.write('0');              // "0"を送信
//  //  delay(1000);
//  //
}
