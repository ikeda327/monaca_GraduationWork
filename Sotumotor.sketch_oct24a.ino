
#define CCW LOW
#define CW HIGH

//motor1
const int DIR = 7;
const int STEP = 6;

//motor2
const int seSTEP = 9;
const int seDIR = 8;

long counter = 0;

const int LED_PIN = 13;

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
  digitalWrite(DIR, dir); // 反時計回り

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
  digitalWrite(seDIR, sedir); // 反時計回り

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
  digitalWrite(DIR, fastdir); // 反時計回り

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
  digitalWrite(seDIR, fastsedir); // 反時計回り

  while (count --) {
    //    digitalWrite(STEP, HIGH);
    //    delay(1);
    //    digitalWrite(STEP, LOW);
    //    delay(1);
    stepy();
    delayMicroseconds(350);
  }
}



//void mostop(int count,int modir){
//  digitalWrite(DIR,modir);  // 反時計回り
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
}

void loop() {

  

    if(Serial.available() > 0){    // 入力された文字が何バイトか調べその回数分繰り返す
      int val = Serial.read(); // 1バイト分のデータを読み取る
//      Serial.write(val);
    

      if(val == '1'){// "1"ならLEDを点灯,"0"ならLEDを消灯
        digitalWrite(13, HIGH);
//        digitalWrite(LED_PIN, HIGH);
        move(15, CCW);                 //12CW*320回 = 3840
      } else if(val == '0'){
        digitalWrite(13, LOW);
//        digitalWrite(LED_PIN, LOW);
        move(15, CCW);                 //12CW*320回 = 3840
      } else if(val == '2') {
        digitalWrite(13, LOW);
//        digitalWrite(LED_PIN, LOW);
        fastmove(15*320, CW);          //3840
//        move(4000, CW);
//        delay(5000);.
//        delay(10);
        semove(50*9, CCW);
        Serial.write('10');
      } else if(val == '3') {
        digitalWrite(13, LOW);
//        digitalWrite(LED_PIN, LOW);
        fastmove(15*320, CW);
        semove(50*9, CW);  
      } else if(val == '8') {
        semove(50, CW);
      } else if(val == '9') {
        semove(50, CCW);
      }
//      delay(500);  

      Serial.write(val);
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
