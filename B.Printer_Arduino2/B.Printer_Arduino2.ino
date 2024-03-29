#define CCW LOW
#define CW HIGH

#define WIDTH (320)
#define HEIGHT (320)

#define FAST_DELAY (350)
#define SLOW_DELAY (1000)

const int DIR = 7;
const int STEP = 6;
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
  digitalWrite(DIR, dir);

  int fact = dir == CW ? 1 : -1;

  while (count --) {
    stepx();
    //    delay(1);
    counter += fact;
  }
}

void semove(int count, int sedir) {
  digitalWrite(seDIR, sedir);
  while (count --) {
    stepy();
    //    delay(1);
  }
}

void fastmove(int count, int fastdir) {
  digitalWrite(DIR, fastdir);
  while (count --) {
    stepx();
    delayMicroseconds(350);
  }
}

void fastsemove(int count, int fastsedir) {
  digitalWrite(seDIR, fastsedir);
  while (count --) {
    stepy();
    delayMicroseconds(350);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  pinMode(seDIR, OUTPUT);
  pinMode(seSTEP, OUTPUT);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

int val = -1;

int swlval = 1;
int swrval = 1;
int swuval = 1;
int swdval = 1;

int xdir = CW;
int xcount = 0;

int ydir = CW;
int ycount = 0;

long delay_time = SLOW_DELAY;
int one_pixcel = 15;

int xwait = true;
int ywait = true;

int xdis;
int ydis;

int sw = true;

int checkCommand(int val) {
  switch (val) {
    case '0':
    case '1':
      xwait = false;
      ywait = true;
      xdir = CCW;
      digitalWrite(DIR, CCW);
      delay_time = SLOW_DELAY;
      xcount = one_pixcel;
      ycount = 0;
      xdis = 15;
      ydis = 0;
      break;
    case '2':
      xwait = false;
      ywait = true;
      xdir = CW;
      ydir = CCW;
      digitalWrite(DIR, CW);
      digitalWrite(seDIR, CCW);
      //      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      ycount = one_pixcel;
      xdis = 15*320;
      ydis = 15;
      break;
    case '3':
      xwait = false;
      ywait = false;
      xdir = CW;
      ydir = CW;
      digitalWrite(DIR, CW);
      digitalWrite(seDIR, CW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      ycount = one_pixcel * HEIGHT;
      xdis = 15*320;
      ydis = 15*320;
      break;
    case '4':
      xwait = false;
      ywait = true;
      xdir = CCW;
      digitalWrite(DIR, CCW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      xdis = 15*320;
      ydis = 0;
      break;
    case '5':
      xwait = false;
      ywait = true;
      xdir = CW;
      digitalWrite(DIR, CW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      xdis = 15*320;
      ydis = 0;
      break;
    case '6':
      xwait = true;
      ywait = false;
      ydir = CCW;
      digitalWrite(seDIR, CCW);
      delay_time = FAST_DELAY;
      ycount = one_pixcel * HEIGHT;
      xdis = 0;
      ydis = 15*320;
      break;
    case '7':
      xwait = true;
      ywait = false;
      ydir = CW;
      digitalWrite(seDIR, CW);
      delay_time = FAST_DELAY;
      ycount = one_pixcel * HEIGHT;
      xdis = 0;
      ydis = 15*320;
      break;
    default:
      xwait = true;
      ywait = true;
      val = 21;
      break;
  }
  return val;
}
const char str[4];
String bin;

void loop() {

    while(!Serial.available()){
    }

    val2 = Serial.read();
    Serial.println(val2);
    bin = "";
    //十六進数から二進数への変換
    if (val2 == '0') bin += "0000";
      else if (val2 == '1') bin = "0001";
      else if (val2 == '2') bin = "0010";
      else if (val2 == '3') bin = "0011";
      else if (val2 == '4') bin = "0100";
      else if (val2 == '5') bin = "0101";
      else if (val2 == '6') bin = "0110";
      else if (val2 == '7') bin = "0111";
      else if (val2 == '8') bin = "1000";
      else if (val2 == '9') bin = "1001";
      else if (val2 == 'a') bin = "1010";
      else if (val2 == 'b') bin = "1011";
      else if (val2 == 'c') bin = "1100";
      else if (val2 == 'd') bin = "1101";
      else if (val2 == 'e') bin = "1110";
      else if (val2 == 'f') bin = "1111";
      else if (val2 == 'x') bin = "2";
      else if (val2 == 'z') bin = "3";
      for(int i=0;i<4;i++){
        str[i] = bin.charAt(i);
      }
  
  for(int i =0; i<4;i++){
    val = checkCommand(str[i]);
    if (val == '1' ) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }

    // x軸モーター
    if (xwait == false) {
      move(xdis,xdir);
    }

    //リミットスイッチの検出
    if (digitalRead(2) == 1 && digitalRead(3) == 1 && digitalRead(4) == 1 && digitalRead(5) == 1) {
      sw = true;
    } else if (digitalRead(2) == 0) {
      Serial.write('11');
      sw = false;
      break;
    } else if (digitalRead(3) == 0) {
      Serial.write('12');
      sw = false;
      break;
    } else if (digitalRead(4) == 0) {
      Serial.write('13');
      sw = false;
      break;
    } else if (digitalRead(5) == 0) {
      Serial.write('14');
      sw = false;
      break;
    }

    // y軸モーター
    if (ywait == false) {
      move(ydis,ydir);
    }

      //リミットスイッチの検出
      if (digitalRead(2) == 1 && digitalRead(3) == 1 && digitalRead(4) == 1 && digitalRead(5) == 1) {
      sw = true;
    } else if (digitalRead(2) == 0) {
      Serial.write('11');
      sw = false;
      break;
    } else if (digitalRead(3) == 0) {
      Serial.write('12');
      sw = false;
      break;
    } else if (digitalRead(4) == 0) {
      Serial.write('13');
      sw = false;
      break;
    } else if (digitalRead(5) == 0) {
      Serial.write('14');
      sw = false;
      break;
    }
  }
  
  if(i==3 && sw == true){
    Serial.write('0');
  }
}
