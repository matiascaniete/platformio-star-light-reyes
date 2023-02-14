#include <Arduino.h>

uint8_t pwmCounter = 0;
uint64_t counter = 0;
uint8_t intensities[5] = {0, 0, 0, 0, 0};
uint8_t sen[256];
uint16_t offset = -30;

uint8_t speeds[3] = {128, 64, 32};
uint8_t speedIndex = 1;
uint64_t speedIndexCounter = 0;

void allPins(uint8_t state)
{
  digitalWrite(0, state);
  digitalWrite(1, state);
  digitalWrite(2, state);
  digitalWrite(3, state);
  digitalWrite(4, state);
}

void blinkAll(uint16_t del)
{
  allPins(HIGH);
  delay(del);
  allPins(LOW);
  delay(del);
}

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin 0 as an output.
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  for (size_t i = 0; i <= 255; i++)
  {
    sen[i] = 127 * sin(2.0 * PI * (float)i / 256.0) + 128.0;
  }

  blinkAll(500);
  blinkAll(500);
  blinkAll(500);
}

void setIntensity(uint8_t pin, uint8_t intensity)
{
  intensities[pin] = intensity;
}

void pwm(uint8_t pin, uint8_t intensity)
{
  if (pwmCounter < intensity)
  {
    digitalWrite(pin, HIGH);
  }
  else
  {
    digitalWrite(pin, LOW);
  }
}

void render()
{
  for (size_t pin = 0; pin <= 4; pin++)
  {
    pwm(pin, intensities[pin]);
  }
  pwmCounter++;
}

void pattern(uint8_t divider = 128)
{
  if (counter % divider == 0)
  {
    double tIntensity = sen[(counter / 2048) % 256] + 256;
    double tFrequency = (sen[(counter / 2048) % 256] / 256.0) + 1;
    double tOffset = sen[(counter / 4096) % 256];

    double intensity = tIntensity / 512.0;
    int offset = (tOffset / 2) - 64;

    intensity = 1;
    // offset = 0;
    tFrequency = 1;

    uint64_t divided = tFrequency * counter / divider;

    setIntensity(0, intensity * sen[divided % 256]);
    setIntensity(1, intensity * sen[(divided + offset) % 256]);
    setIntensity(2, intensity * sen[(divided + 2 * offset) % 256]);
    setIntensity(3, intensity * sen[(divided + 3 * offset) % 256]);
    setIntensity(4, intensity * sen[(divided + 4 * offset) % 256]);
  }

  render();
}

// the loop function runs over and over again forever

void loop()
{
  pattern(speeds[speedIndex]);

  counter++;
}
