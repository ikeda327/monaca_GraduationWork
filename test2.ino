
/// Wifi接続してWebサーバを起動する
#include <M5StickCPlus.h>
#include <WiFi.h>
#include  <WiFiClient.h>
#include  <WebServer.h>
#include  <ESPmDNS.h>
#include <SoftwareSerial.h>

//unoとのSerial通信
#define COMM_TX 26
#define COMM_RX 25

#define MAX_WIDTH (320)

//a~fを11~16に変更
#define CTON(c) (c >= 'a' && c <= 'f' ? c - 'a' + 10 : c - '0')
#define NTOC(n) (n > 9 ? n + 'a' : n + '0')

SoftwareSerial sSerial;

/**
 * 文字列のバイト配列2桁 ⇒ byte整数への変換 
 */
byte hex2bin(byte data[]) {
  byte result = 0;
  result = CTON(data[0]);
  result <<= 4;
  result |= CTON(data[1]);
  return result;
}

/**
 * 1byteを8ピクセルとしてスキャンする
 *
 *  x:x座標
 *  y:y座標
 *  b:8ピクセルデータ
 *  width:画面幅(8の倍数)
 * 返し値
 *  次のx座標
 */
int scan(int x, int y, byte b, int width) {
  for (byte sd = 0x80; sd > 0; sd >>= 1) {
    if (b & sd) {
//      digitalWrite(LED, ON);
    } else {
//      digitalWrite(LED, OFF);
    }
    Serial.print(b & sd ? 1 : 0);
    x++;
    if (x < width) {
    } else {
      x = 0;
      y++;
    }
    delay(100);
  }
  Serial.println();

  return x;
}

//wifi
const char* ssid = "2F03G";
const char* password = "SILKSUMMER";

//固定ip(学校)
IPAddress ip(10, 10, 21 ,21);
IPAddress gateway(10,10,40,1);
IPAddress subnet(255,255,255,0);

// WebServerの設定 
WebServer server(80);
String hex;
void handleDisplayText(){
  Serial.print("uri:");
  Serial.println(server.uri());
  Serial.print("args:");
  Serial.println(server.args());
  // リクエストBODYが空の場合はエラー
  if (server.args() == 0) {
    server.send(422,"text/html","parameter is incorrect.");
  }
  // リクエストBODYはserver.argにセットされている
  hex = server.arg(0);
  Serial.print("データ:");
  Serial.println(hex);
//   2進数への変換(文字列)
//   for(int i=0; i<hex.length(); i++){
//        if (hex[i] == '0') bin += "0000";
//        else if (hex[i] == '1') bin += "0001";
//        else if (hex[i] == '2') bin += "0010";
//        else if (hex[i] == '3') bin += "0011";
//        else if (hex[i] == '4') bin += "0100";
//        else if (hex[i] == '5') bin += "0101";
//        else if (hex[i] == '6') bin += "0110";
//        else if (hex[i] == '7') bin += "0111";
//        else if (hex[i] == '8') bin += "1000";
//        else if (hex[i] == '9') bin += "1001";
//        else if (hex[i] == 'A' || hex[i] == 'a') bin += "1010";
//        else if (hex[i] == 'B' || hex[i] == 'b') bin += "1011";
//        else if (hex[i] == 'C' || hex[i] == 'c') bin += "1100";
//        else if (hex[i] == 'D' || hex[i] == 'd') bin += "1101";
//        else if (hex[i] == 'E' || hex[i] == 'e') bin += "1110";
//        else if (hex[i] == 'F' || hex[i] == 'f') bin += "1111";
//    }
    
  //2進数表記の表示
//  Serial.println(bin);

//  if(M5.BtnA.wasPressed()){
//    Serial.println("Aボタンが押されました");
//      for(int i=0; i<bin.length(); i++){
//        if(M5.BtnB.wasPressed()){
//          Serial.println("Bボタンが押されました");
//          break;
//          }
//        if(bin[i]==0){
//          digitalWrite(26, LOW);
//        }else{
//          digitalWrite(26, HIGH);
//        }
//        delay(200);
//      }
//    }
}
// NotFound
void handleNotFound(){
  server.send(404,"text/html","Not Found.");
}

void setup(void) {
  WiFi.config(ip, gateway, subnet);
  M5.begin();
  M5.Lcd.setTextSize(3);
  Serial.begin(115200);
  
  //シリアル通信の開始
  sSerial.begin(115200, SWSERIAL_8N1, COMM_RX, COMM_TX, false); 
  
  // WiFi接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("WiFi Connect To: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // 接続出来たら画面に表示
  m5.Lcd.println("connect!"); 

   if (MDNS.begin("m5stickC")) {
     Serial.println("MDNS responder started");
   }

  // ルーティング
  // 第二引数でリクエストメソッドを指定する
  // リクエストパス：/data、 リクエストメソッド：POST
  server.on("/data" ,HTTP_POST, handleDisplayText); 
  server.onNotFound(handleNotFound);
  Serial.println("HTTP server started");

  // Webサーバ起動
  server.begin();

//  pinMode(LED, OUTPUT);
//  digitalWrite(LED, OFF);
}
int x = 0;
void loop() {
  server.handleClient();
  if(hex!=null){
      static byte b[2];
      hex.getBytes(b, 2);
      Serial.print(b[0], HEX);
      Serial.print(b[1], HEX);   
      Serial.print("->");
      hex2bin(b);
      byte bd = hex2bin(b);
      Serial.print(bd);
      Serial.print(":");
      Serial.print(x);
      Serial.println();
      x = scan(x, 0, bd, MAX_WIDTH);
      delay(10);
  }
}
