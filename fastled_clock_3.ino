
#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 60

CRGB leds[NUM_LEDS];
const int NUM_ = NUM_LEDS - 1;
int lay = 10;
int lay2 = 1200;
int b1 = 65;

//_______________________________________________colours
//______________________hue red: 0, green: 85, blue: 170;
CHSV pink(250, 150, b1); // pink 250,150,255
CHSV warm(20, 255, b1);  // warm
CHSV white(0, 0, b1);    // white

void solidRGBA(int x, int y, int z, int a)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(x, y, z);
    }
    FastLED.setBrightness(a);
    FastLED.show();
}
// solid[show,r,g,b]
void solidSRGB(bool s, int x, int y, int z)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(x, y, z);
    }
    if (s == true)
    {
        FastLED.show();
    }
}
void colorTesting(char correctionType, int hue)
{
    /*
    correctionType {
      UncorrectedColor, TypicalLEDStrip,
      TypicalPixelString, TypicalSMD5050
    }
    color {
      [set color via any named HTML web color]
      examples:
      Red,
        DarkMagenta, DeepPink, DarkOrange, Orange, OrangeRed,
      Green,
        DarkGreen, Gold, Goldenrod, SeaGreen, LightSeaGreen, Teal, Yellow,
      Blue,
        BlueViolet, DodgerBlue, Aqua, DarkViolet, Indigo, MidnightBlue
      white
    }
    */

    hue = hue * (32 * 1.7);
    FastLED.setCorrection(correctionType);
    // white
    for (int i = 0; i < NUM_LEDS / 6; i++)
    {
        leds[i] = white;
    }
    leds[0] = CHSV(hue, 255, b1);
    FastLED.show();
    delay(lay2);
}
void p(char *title, int value)
{
    Serial.print(title);
    Serial.print(value);
}
void pln(char *title, int value)
{
    Serial.print(title);
    Serial.println(value);
}
void setupTest()
{
    // test each led with white
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = white;
        FastLED.show();
        delay(lay);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(lay / 2);
    }
}

void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    // test each led with white
    // setupTest();
}

//________________________body__________________________
// insert current time into time[] {Hour, Minute, Second}
// Seconds initial value is 9 to compensate for startup
int time[] = {6, 0, 9};

unsigned long interval[3] = {0, 0, 0};
long second = 1000; // calibrate seconds speed
long minute = second * 60;
long hour = minute * 60;

int timeH = time[0];
long s = time[2];
long m = time[1];
long h = 5 * timeH;

// pixels
const int pixelCount = NUM_LEDS;
int frameCount[pixelCount];
int index[pixelCount];
bool pixelIO[pixelCount];
int dx1 = 100;

void loop()
{
    // clear field
    solidSRGB(false, 0, 0, 0);
    // one second
    if (millis() - interval[0] > second)
    {
        interval[0] = millis();
        // seconds overflow
        if (s > NUM_)
        {
            s = 0;
            clearPixels();
            m = dxM();
            h = dxH(m);
        }
        insertPixel(s);
        s++;
    }

    // draw
    for (int i = 0; i < s; i++)
    {
        pixel(
            i, 1, 6,
            50, 0, 0);
    }

    for (int i = 0; i < NUM_LEDS / 5; i++)
    {
        leds[i * 5 + 4] = CRGB::White;
    }
    for (int i = 0; i < 4; i++)
    {
        leds[i * 15 + 14] = CRGB::Indigo;
    }
    leds[NUM_ - m] = CRGB::Blue;
    leds[NUM_ - h] = CRGB::Green;

    //  display
    FastLED.setBrightness(2);
    FastLED.show();
}

int dxM()
{
    m++;
    if (m > NUM_)
    {
        m = 0;
    }
    return m;
}
int dxH(int m)
{
    if (m == 0)
    {
        h += 5;
        if (h > NUM_)
        {
            h = 0;
        }
    }
    return h;
}

//____________seconds pixel_________________
void insertPixel(int id)
{
    // set index [0]
    pixelIO[id] = true;
    //
}

void clearPixels()
{
    for (int i = 0; i < pixelCount; i++)
    {
        pixelIO[i] = false;
        index[i] = 0;
        frameCount[i] = 0;
    }
}

void pixel(
    int id, int l, int s,
    int r, int g, int b)
{
    // zero-index id for array
    // id = id - 1;

    // check pixelIO
    if (pixelIO[id] == true)
    {
        // check current frameCount
        if (frameCount[id] > s)
        {
            // reset frameCount, increment index if < NUM_LEDS
            frameCount[id] = 0;
            int idProp;
            if (id >= NUM_LEDS)
            {
                idProp = id - NUM_LEDS;
            }
            else
            {
                idProp = id;
            }
            if (index[id] < NUM_ - idProp)
            {
                index[id]++;
            }
        }
        else
        {
            // increment frameCount
            frameCount[id]++;
        }
        // draw
        draw(id, l, r, g, b);
    }
}

void draw(int id, int l, int r, int g, int b)
{
    for (int i = 0; i < l; i++)
    {
        // if index < NUM_LEDS
        if (index[id] + i < NUM_LEDS)
        {
            leds[index[id] + i] = CRGB(r, g, b);
        }
    }
}
