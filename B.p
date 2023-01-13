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

//const int REDIR = 10;

long counter = 0;

const int LED_PIN = 13;

int duty = 100;

void stepx() {
  digitalWrite(STEP, HIGH);
  delayMicroseconds(duty);
  digitalWrite(STEP, LOW);
  delayMicroseconds(duty);
}

void stepy() {
  digitalWrite(seSTEP, HIGH);
  delayMicroseconds(duty);
  digitalWrite(seSTEP, LOW);
  delayMicroseconds(duty);
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
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  pinMode(seDIR, OUTPUT);
  pinMode(seSTEP, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

int val = -1;

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
      xdir = CCW;
      xwait = false;
      ywait = true;
      digitalWrite(DIR, CCW);
      delay_time = SLOW_DELAY;
      xcount = one_pixcel;
      ycount = 0;
      break;
    case '2':
      xdir = CW;
      ydir = CCW;
      xwait = false;
      ywait = true;
      digitalWrite(DIR, CW);
      digitalWrite(seDIR, CCW);
      //      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      ycount = one_pixcel;
      break;
    case '3':
      xdir = CW;
      ydir = CW;
      xwait = false;
      ywait = false;
      digitalWrite(DIR, CW);
      digitalWrite(seDIR, CW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      ycount = one_pixcel * HEIGHT;
      break;
    case '4':
      xdir = CCW;
      xwait = false;
      ywait = true;
      digitalWrite(DIR, CCW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      break;
    case '5':
      xdir = CW;
      xwait = false;
      ywait = true;
      digitalWrite(DIR, CW);
      delay_time = FAST_DELAY;
      xcount = one_pixcel * WIDTH;
      break;
    case '6':
      ydir = CCW;
      xwait = true;
      ywait = false;
      digitalWrite(seDIR, CCW);
      delay_time = FAST_DELAY;
      ycount = one_pixcel * HEIGHT;
      break;
    case '7':
      ydir = CW;
      xwait = true;
      ywait = false;
      digitalWrite(seDIR, CW);
      delay_time = FAST_DELAY;
      ycount = one_pixcel * HEIGHT;
      break;
//    case '8':
//       digitalWrite(REDIR, CW);
//       break;
//    case '9':
//       digitalWrite(REDIR, CCW);
//       break;
    default:
      xwait = true;
      ywait = true;
      val = 21;
      break;
  }
  return val;
}

void loop() {
  
  if (Serial.available() > 0 && xcount <1 && ycount < 1) {
    val = checkCommand(Serial.read());
    //Serial.write(val);
  }

  if (val == '1') {
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

   
  int swlval = digitalRead(2);
  int swrval = digitalRead(3);
  int swuval = digitalRead(4);
  int swdval = digitalRead(5);

  if (swlval == 1 && swrval == 1 && swuval == 1 && swdval == 1) {
    Serial.write('0');
  } else if (swlval == 0) {
    Serial.write('11');
  } else if (swrval == 0) {
    Serial.write('12');
  } else if (swuval == 0) {
    Serial.write('13');
  } else if (swdval == 0) {
    Serial.write('14');
  }
