#include <Arduino.h>
#include <FastLED.h>
#include <HID-Project.h>
#include <GotchauButton.h>

#define NUM_LEDS 4
#define LED_PIN 8

#define BTN_A 0
#define BTN_B 1
#define BTN_C 2
#define BTN_D 3

#define PIN_BTN_A 7
#define PIN_BTN_B 4
#define PIN_BTN_C 5
#define PIN_BTN_D 6

#define LEDS_PIN 8
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

Button btnA(PIN_BTN_A);
Button btnB(PIN_BTN_B);
Button btnC(PIN_BTN_C);
Button btnD(PIN_BTN_D);
Button btns[] = {btnA, btnB, btnC, btnD};

CRGB leds[NUM_LEDS];
uint32_t lastTime = 0;
uint8_t periodMs = 10; // отвечает за скорость анимации. Меньше - быстрее.
uint8_t btnBrightness[] = {100, 100, 100, 100};

CRGB fadeColor = CRGB::Violet;

void setup()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    btns[i].setDebounceTime(80);
    btns[i].setClampTimeout(100);
  }

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  Consumer.begin();
}

void Fading()
{
  for (int i = 0; i < NUM_LEDS; i++)
    if (btnBrightness[i] > 0)
    {

      leds[i].red = fadeColor.r * btnBrightness[i] / 100;
      leds[i].green = fadeColor.g * btnBrightness[i] / 100;
      leds[i].blue = fadeColor.b * btnBrightness[i] / 100;

      btnBrightness[i]--;
      FastLED.show();
    }
}

void Rainbow()
{
  static uint8_t hue = 0;
  fill_rainbow(&(leds[0]), NUM_LEDS, hue);
  hue++;
  FastLED.show();
}

void loop()
{
  uint32_t now = millis();
  if (now - lastTime > periodMs)
  {
    lastTime = now;
    //Fading();
    Rainbow();
  }

  if (btns[BTN_A].is(Pressed) || btns[BTN_A].is(Clamped))
  {
    Consumer.write(MEDIA_VOLUME_MUTE);

    leds[BTN_A] = fadeColor;
    btnBrightness[BTN_A] = 100;
  }

  if (btns[BTN_B].is(Pressed) || btns[BTN_B].is(Clamped))
  {
    Consumer.write(MEDIA_VOLUME_DOWN);

    leds[BTN_B] = fadeColor;
    btnBrightness[BTN_B] = 100;
  }

  if (btns[BTN_C].is(Pressed) || btns[BTN_C].is(Clamped))
  {
    Consumer.write(MEDIA_VOLUME_UP);

    leds[BTN_C] = fadeColor;
    btnBrightness[BTN_C] = 100;
  }

  if (btns[BTN_D].is(Pressed) || btns[BTN_D].is(Clamped))
  {
    Consumer.write(MEDIA_PLAY_PAUSE);

    leds[BTN_D] = fadeColor;
    btnBrightness[BTN_D] = 100;
  }
}
