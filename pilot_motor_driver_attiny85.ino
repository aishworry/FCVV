unsigned char m[4] = {0, 64, 127, 255};
unsigned long int  t[4] = {0, 0, 0, 0};
int it = 0;
unsigned char flag = 0;

unsigned long int looptime = 0;
unsigned long int timertime = 0;
unsigned long int frq_loop = 2000000;
unsigned long int convtr = 0;

void setup() {
  // put your setup code here, to run once:
  DDRB |= 0x37;
  convtr = (frq_loop / 255);
  looptime = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (it = 0; it < 4; it++)
  {
    t[it] = m[it] * convtr;
  }
  while (timertime <= frq_loop)
  {
    timertime = micros() - looptime;
  }
  looptime = micros();
  flag = 0x17;
  PORTB |= 0x17;
  while (flag > 0)
  {
    timertime = micros() - looptime;
    if (timertime >= t[0]) {
      PORTB &= 0xFE;
      flag &= 0xFE;
    }
    if (timertime >= t[1]) {
      PORTB &= 0xFD;
      flag &= 0xFD;
    }
    if (timertime >= t[2]) {
      PORTB &= 0xFB;
      flag &= 0xFB;
    }
    if (timertime >= t[3]) {
      PORTB &= 0xE0;
      flag &= 0xE0;
    }
  }
}
