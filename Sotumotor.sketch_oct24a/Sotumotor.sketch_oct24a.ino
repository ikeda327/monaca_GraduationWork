#define PULSE_INTERVAL (800)
#define ACCELERATOR (20)

#define CCW LOW
#define CW HIGH
int direction = CW;

int delay_wait = 1;
int enable = false;
int pulse_count = -1;

int accelerator = ACCELERATOR;
int acce_step = ACCELERATOR;

//motor1
const int DIR = 7;
const int STEP = 6;

//motor2
const int seSTEP = 9;
const int seDIR = 8;

long counter = 0;

const int LED_PIN = 13;

const int RE_SW = 2;





void stepx() {
  digitalWrite(STEP, HIGH);
  delayMicroseconds(100);
  digitalWrite(STEP, LOW);
  delayMicroseconds(100);

}


void stepy() {
  digitalWrite(seSTEP, HIGH);
  delayMicroseconds(100);
  digitalWrite(seSTEP, LOW);
  delayMicroseconds(100);

}

void move(int count, int dir) {
  //Serial.begin(115200);
  digitalWrite(DIR, dir);         // 反時計回り

  int fact = dir == CW ? 1 : -1;

  while (count --) {
    //    digitalWrite(STEP, HIGH);
    //    delayMicroseconds(800);
    //    digitalWrite(STEP, LOW);
    //    delayMicroseconds(800);
    stepx();
    delay(1);

    counter += fact;
    //Serial.println(count);
  }
}


void semove(int count, int sedir) {
  digitalWrite(seDIR, sedir);       // 反時計回り

  while (count --) {
    //    digitalWrite(seSTEP, HIGH);
    //    delay(5);
    //    digitalWrite(seSTEP, LOW);
    //    delay(5);
    stepy();
    delay(1);
  }
}


void fastmove(int count, int fastdir) {
  digitalWrite(DIR, fastdir);       // 反時計回り

  while (count --) {
    //    digitalWrite(STEP, HIGH);
    //    delay(1);
    //    digitalWrite(STEP, LOW);
    //    delay(1);
    stepx();
    delayMicroseconds(350);
  }
}

void fastsemove(int count, int fastsedir) {
  digitalWrite(seDIR, fastsedir);       // 反時計回り

  while (count --) {
    //    digitalWrite(STEP, HIGH);
    //    delay(1);
    //    digitalWrite(STEP, LOW);
    //    delay(1);
    stepy();
    delayMicroseconds(350);
  }
}

void initial() {
  move(15, CW);
  move(15, CCW);
  move(15, CCW);
  move(15, CW);
}



//void mostop(int count,int modir){
//  digitalWrite(DIR,modir);        // 反時計回り
//
//  while(count --){
//    digitalWrite(STEP,HIGH);
//    delay(1);
//    digitalWrite(STEP,HIGH);
//    delay(1);
//  }
//}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  pinMode(seDIR, OUTPUT);
  pinMode(seSTEP, OUTPUT);

  pinMode(RE_SW,INPUT);

  pinMode(2,INPUT);
}

void loop() {

  

    if(Serial.available() > 0){         // 入力された文字が何バイトか調べその回数分繰り返す
      int val = Serial.read();          // 1バイト分のデータを読み取る
      int SWval = digitalRead(2);
//      Serial.write(val);
    

      if(val == '1' && SWval == '1') {  // レーザーをONにして320のうちの1進める
        digitalWrite(13, HIGH);
        digitalWrite(LED_PIN, HIGH);
        move(15, CCW);                 //12CW*320回 = 3840
//        if(SWval == '0') {
//          digitalWrite(13, LOW);
//          move(0, CCW);
//        }else {
//          move(15, CCW);
//        }
      } else if(val == '0'){            //レーザーをOFFにして320のうちの1進める
        digitalWrite(13, LOW);
//        digitalWrite(LED_PIN, LOW);
        move(15, CCW);                 //12CW*320回 = 3840
      } else if(val == '2') {          //レーザーをOFFにして横軸のレーザーを初期位置に戻すその後縦軸を320のうちの1進める
        digitalWrite(13, LOW);
//        digitalWrite(LED_PIN, LOW);
        fastmove(15*320, CW);          //3840
//        move(4000, CW);
//        delay(5000);.
//        delay(10);
        semove(50*9, CCW);
        Serial.write('10');
      } else if(val == '3') {         //レーザーをOFFにして横軸と縦軸を初期位置に戻す
        digitalWrite(13, LOW);
//        digitalWrite(LED_PIN, LOW);
        fastmove(15*320, CW);
        semove(50*9, CW);  
      } else if(val == '8') {        //(テスト用)　縦軸を時計回りに50ステップ動かす
        semove(50, CW);
      } else if(val == '9') {        //(テスト用)　縦軸を反時計回りに50ステップ動かす
        semove(50, CCW);
      }
//      delay(500);  

      Serial.write(val);
//      Serial.println(digitalRead(RE_SW));
    }
}
     
    


  //Serial.println(counter);
  // put your main code here, to run repeatedly:

  //  Serial.begin(115200);
  //  for(int tcount=0; tcount<2; tcount=tcount+1){
  //      move(2200,CW);
  //      move(2200,CCW);
  //
  //
  //    }




//  move(4000, CW);
//
//  fastmove(4000, CCW);
//
//  semove(150, CCW);
