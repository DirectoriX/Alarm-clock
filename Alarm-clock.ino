#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const byte probKeep = 99;
const byte ddelta = 16;
const byte ledNum = 15;

byte state[ledNum] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
int ledPWM[ledNum] = {2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048};
int8_t delta[ledNum] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte rnd, less;

void setup()
{
  srand(analogRead(A0));
  pwm.begin();
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);
}

void loop()
{
  for (byte i = 0; i < ledNum; i++)
    {
      rnd =  rand() % 100;
      less = ledPWM[i] / 41;

      if (rnd < probKeep)
        {
          state[i] = 2;
        }
      else
        {
          rnd =  rand() % 100;
          
          if (rnd < less)
            {
              state[i] = 1;
            }
          else
            {
              state[i] = 3;
            }
        }

      switch (state[i])
        {
          case 1:
          {
            delta[i] -= ddelta;

            if (delta[i] == -2 * ddelta)
              {
                delta[i] = ddelta;
              }

            break;
          }

          case 3:
          {
            delta[i] += ddelta;

            if (delta[i] == 2 * ddelta)
              {
                delta[i] = -ddelta;
              }

            break;
          }
        }

      ledPWM[i] += delta[i];

      if (ledPWM[i] < 0)
        {
          ledPWM[i] = 0;
          state[i] = 2;
        }

      if (ledPWM[i] > 4095)
        {
          ledPWM[i] = 4095;
          state[i] = 2;
        }

      pwm.setPin(i, ledPWM[i] / (i % 3 == 2 ? 1 : 4), false);
    }

//  delay(1);
}
