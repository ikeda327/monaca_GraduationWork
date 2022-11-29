#include <M5StickCPlus.h>

#include <SoftwareSerial.h>

const int abutton = G39;

#define COMM_TX 26
#define COMM_RX 25

const String image = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000111000000001000000010000000000000010000001011000000000100010001110001000000001010000000000000000000010000001100011000000000000101000000001100000000000110000010000000101100000000000000000000000000000000000000000000000000000000000000000002"
                     "000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000001000000010000000000000110000001010000000000110011000011111100000000001100000000000000000001001110000001110000000000011100000001100000000000110000010000000100100000000000000000000000000000000000000000000000000000000000000000002"
//                   "000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000000001000000010000000000000110000001100000000000011011011000011110000000100110000100000000000001111000000010011011000000001100000001110000000000110000010000000110110000000000000000000000000000000000000000000000000000000000000000002"
//                   "000000000000000000000000000000000000000000000000000000000000000000000000000000000001110000000001000000010000000000000111000001100000000000011100000110000111000000100001000010000000000000100000000000001110000000000100000001110000000000110000010000000010011000000000000000000000000000000000000000000000000000000000000000002"
//                   "000000000000000000000000000000000000000000000000000000000000000000000000000000000001100000000001000000010000000000000101000011000100001111111111000001100001100000100000110001100000000000100100000011111110000010000010000001110000000000110000010000000011001100000000000000000000000000000000000000000000000000000000000000002"
//                   "000000000000000000000000000000000000000000000000000000000000000000000000000000000011100000000001000000010000000000000101100011110000111111111111110000010000100000000000001000110000000000010000011111111111110000100010000001110000000000110000010000000001000110000000000000000000000000000000000000000000000000000000000000002"
//                   "000000000000000000000000000000000000000000000000000000000000000000000000000000000011100000000001000000010000000000001100100011100011111111111111111100001000010000000000000111011000000000010000111111111111111100010000000000010000000000110000011000000001100111000000000000000000000000000000000000000000000000000000000000002"
                     "000000000000000000000000000000000000000000000000000000000000000000000000000000000111100000000001000000010000000000001100010011000111111000000001111110000000000000000000000000001110000000001011111111000111111110001011000010010000000000110000000000000001100011000000000000000000000000000000000000000000000000000000000000002"
                     "000000000000000000000000000000000000000000000000000000000000000000000000000000001111100000000001000000010000000000001100010010011111100000000000011111000000000100000000000000000000000000001111111000000000011111000011000010010000000000110000010000000000100001100000000000000000000000000000000000000000000000000000000000002"
                     "000000000000000000000000000000000000000000000000000000000000000000000000000000001101100000000001000000010000000000001000001000111110000001111000000111000000000010001000000000000000000000001111100000111000000111110011000000010000000000010000000000000000110000110000000000000000000000000000000000000000000000000000000000002";



const int LED_PIN = G10;
SoftwareSerial sSerial;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  sSerial.begin(9600, SWSERIAL_8N1, COMM_RX, COMM_TX, false);         // シリアル通信の開始(ボーレート9600bps)

  pinMode(abutton, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN,LOW);
}

void loop() {
  if ( digitalRead(abutton) == LOW)
  {
    int index = 0;
    Serial.println("b puress");
    Serial.print("length:");
    Serial.println(image.length());

    while (index < image.length()) {
      sSerial.write(image.charAt(index));
      index++;
      while (!sSerial.available()) {
//        Serial.print(".");
      }
      // 中断
      if ( digitalRead(abutton) == LOW)
      {
        Serial.println("abort");
        return;
      }
      Serial.write(sSerial.read());
    }
    Serial.println("finish");
  }
}