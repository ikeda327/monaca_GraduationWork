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
      break;
    case '4':
      xwait = false;
      ywait = true;
      xdir = CCW;
      digitalWrite(DIR, CCW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      break;
    case '5':
      xwait = false;
      ywait = true;
      xdir = CW;
      digitalWrite(DIR, CW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      break;
    case '6':
      xwait = true;
      ywait = false;
      ydir = CCW;
      digitalWrite(seDIR, CCW);
      delay_time = FAST_DELAY;
      ycount = one_pixcel * HEIGHT;
      break;
    case '7':
      xwait = true;
      ywait = false;
      ydir = CW;
      digitalWrite(seDIR, CW);
      delay_time = FAST_DELAY;
      ycount = one_pixcel * HEIGHT;
      break;
    default:
      xwait = true;
      ywait = true;
      val = 21;
      break;
  }
  return val;
}

void loop() {
  if (Serial.available() > 0 && xcount < 1 && ycount < 1) {
    val = checkCommand(Serial.read());
    Serial.write(val);
  }

  if (val == '1' ) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

  while (!Serial.available()) {
  }

  // x軸モーター
  if (xwait == false) {
    if (xcount > 0) {
      stepx();
      //    move(1, xdir);
      xcount --;
      //      Serial.println("x軸モーター1step動きました");
      //      Serial.println(xcount);
    }
    if (xcount < 1) {
      //成功した結果を返す
      //Serial.write('0');
      Serial.println("x軸モーター15step動き終わりました");
      xwait = true;
      ywait = false;
      //      while (!Serial.available()) {
      //      }
    }
  }
  // y軸モーター
  if (ywait == false) {
    if (ycount > 0) {
      stepy();
      //    move(1, ydir);
      ycount --;
      //      Serial.println("y軸モーター1step動きました");
    }
    if (ycount < 1) {
      //Serial.write('0');
      Serial.println("y軸モーター15step動き終わりました");
      xwait = true;
      ywait = true;
      //      while (!Serial.available()) {
      //      }
    }
  }

  if (digitalRead(2) == 1 && digitalRead(3) == 1 && digitalRead(4) == 1 && digitalRead(5) == 1) {
    Serial.write('0');
  } else if (digitalRead(2) == 0) {
    Serial.write('11');
  } else if (digitalRead(3) == 0) {
    Serial.write('12');
  } else if (digitalRead(4) == 0) {
    Serial.write('13');
  } else if (digitalRead(5) == 0) {
    Serial.write('14');
  }




  //  delayMicroseconds(delay_time);
}


//  if (val == '1' ) {
//    digitalWrite(13, HIGH);
//    move(15, CCW);
//  } else if (val == '0') {
//    digitalWrite(13, LOW);
//    move(15, CCW);
//  } else if (val == '2') {
//    digitalWrite(13, LOW);
//    fastmove(15 * 320, CW);
//    semove(50 * 9, CCW);
//    Serial.write('10');
//  } else if (val == '3') {
//    digitalWrite(13, LOW);
//    fastmove(15 * 320, CW);
//    semove(50 * 9, CW);
//  } else if (val == '8') {
//    semove(50, CW);
//  } else if (val == '9') {
//    semove(50, CCW);
//  }
//  //      Serial.write(val);
//}
