#include <M5StickCPlus.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

//unoとのSerial通信
#define COMM_TX 26
#define COMM_RX 25

#define WIDTH (200)
//#define HEIGHT(320)
int x;//320ピクセル 0-319
int y;//320ピクセル　0-319

SoftwareSerial sSerial;

// wifi
const char* ssid = "2F03G";
const char* password = "SILKSUMMER";

//固定ip(学校)
IPAddress ip(10, 10, 21 , 21);
IPAddress gateway(10, 10, 40, 1);
IPAddress subnet(255, 255, 255, 0);

// WebServerの設定
AsyncWebServer server(80);


String hex;
String bin;

int is_print = false;
//true:印刷中
//false : 待機中

int wait = false;
//待機中
//1ピクセル印刷する
//１行印刷が終わると改行
//全てを印刷すると初期位置へ戻る

void setup() {

  Serial.begin(115200);
  sSerial.begin(9600, SWSERIAL_8N1, COMM_RX, COMM_TX, false);
  m5.begin();
  WiFi.config(ip, gateway, subnet);


  // SPIFFSのセットアップ
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // WiFiに接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("connect!");
  // ESP32のローカルアドレスを表示
  Serial.println(WiFi.localIP());

  server.on("/data", HTTP_POST, [](AsyncWebServerRequest * request) {
    hex = request->arg("send_data");
    Serial.println(hex);

    //////////     2進数への変換(文字列)
    for (int i = 0; i < hex.length(); i++) {
      if (hex[i] == '0') bin += "0000";
      else if (hex[i] == '1') bin += "0001";
      else if (hex[i] == '2') bin += "0010";
      else if (hex[i] == '3') bin += "0011";
      else if (hex[i] == '4') bin += "0100";
      else if (hex[i] == '5') bin += "0101";
      else if (hex[i] == '6') bin += "0110";
      else if (hex[i] == '7') bin += "0111";
      else if (hex[i] == '8') bin += "1000";
      else if (hex[i] == '9') bin += "1001";
      else if (hex[i] == 'a') bin += "1010";
      else if (hex[i] == 'b') bin += "1011";
      else if (hex[i] == 'c') bin += "1100";
      else if (hex[i] == 'd') bin += "1101";
      else if (hex[i] == 'e') bin += "1110";
      else if (hex[i] == 'f') bin += "1111";
    }
    //2進数表記の表示
    Serial.println(bin);
  });

  // トップページにアクセス
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/1_top.html", String());
  });

  //説明ページにアクセス
  server.on("/1_top.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/1_top.html", "text");
  });

  server.on("/2_detail.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/2_detail.html", "text");
  });

  // style.cssにアクセスされた時のレスポンス
  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/css/style.css", "text/css");
  });

  server.on("/js/app.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/app.js", "text/css");
  });

  server.on("/js/image_select-page.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/image_select-page.js", "text/css");
  });

  server.on("/js/ImageProc.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/ImageProc.js", "text/css");
  });
  server.on("/js/ImageProc.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/ImageProc.js", "text/css");
  });
  server.on("/js/QRcode.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/QRcode.js", "text/css");
  });

  server.on("/js/paint-page.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/paint-page.js", "text/css");
  });

  server.on("/images/icon-erasser.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/images/icon-erasser.png", "text/css");
  });

  server.on("/images/icon-pen.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/images/icon-pen.png", "text/css");
  });

  server.on("/images/icon-backBtn.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/images/icon-backBtn.png", "text/css");
  });

  server.on("/images/pan_back.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/images/pan_back.png", "text/css");
  });

  server.on("/images/backImg1.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/images/backImg1.png", "text/css");
  });

  server.on("/images/icon-forwardBtn.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/images/icon-forwardBtn.png", "text/css");
  });

  // サーバースタート
  server.begin();

  //qrコードの生成
  M5.Lcd.qrcode("http://10.10.21.21/", 10, 10, 120, 6);
}

int isecho = -1;

//焼くメソッド
void doPrint() {
  int pix;
  //横軸が端までいったら
  if (x >= WIDTH) {
    x = 0;
    //縦軸が端までいかない
    if (y < WIDTH) {
      ++y;
      pix = '2';
      //      sSerial.write(2);
      //縦横ともに端までいったら
    } else {
      y = 0;
      pix = '3';
      //      sSerial.write(3);
      is_print = false;
    }
  } else {
    //横も縦も端にいっていない間
    pix = bin.charAt(y * WIDTH + x);
    //    sSerial.write(pix);
    x++;
  }
  sSerial.write(pix);
  isecho = 0;
  Serial.println(pix);

  //unoからのデータを待つ
  wait = true;
}

int cnt = 0;

void loop() {
  //  if (wait) {
  //    Serial.println("wait");
  if (sSerial.available()){ //待ちの状態でコールバックがあり正常以外ならば縦横初期値にして、プリンターを止める
    if(isecho == 0){
      Serial.print("echo:");
      Serial.println(sSerial.read());
      isecho = -1;
      return;
    }
    char r = sSerial.read();
    Serial.print("call:");
    Serial.println(r);
    if (r != '0') {
      is_print = false;
      x = 0;
      y = 0;
    } else {
    }
    wait = false;
  }
  //  }
  //プリンターが動いている状態かつ待ち状態ではないならば焼きメソッドを呼ぶ
  if (is_print && !wait) {
    Serial.println(cnt);
    Serial.println("doPrint");
    doPrint();
    cnt++;
  }
  //M5の状態をチェックする
  M5.update();
  //ボタンに対する処理を記述する
  //Aボタン
  if (M5.BtnA.wasReleased()) {
    Serial.println("A pressed");
    //印刷中または待機中に押しても何も起こらない
    if (is_print || wait) {
    } else {
      is_print = true;
    }
    //Bボタン
  } else if (M5.BtnB.wasReleased()) {
    //印刷中のみ作動する
    if (is_print) {
      Serial.println("B pressed");
      is_print = false;
      wait = true;
      x = y = 0;
      sSerial.write(3);
    } else {
    }
  }

  if (Serial.available()) {
    sSerial.write(Serial.read());
  }
}
