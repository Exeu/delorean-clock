#include "Arduino.h"
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "SegmentRow.h"

#include "dcf77.h"
#include "Time.h"

#define DCF_PIN 2	         // Connection pin to DCF 77 device

const uint8_t dcf77_sample_pin = 53;
const uint8_t dcf77_inverted_samples = 1;
// Sortie horloge Rouge
const byte PIN_CLK_Green = A1;   // define CLK pin  
const byte PIN_CLK_Red = A0;   // define CLK pin  
const byte PIN_CLK_Yellow = A2;   // define CLK pin  
// Sortie horloge Verte

const byte PIN_DIO_Y1 = 11;
const byte PIN_DIO_Y2 = 10;
const byte PIN_DIO_Y3 = 9;

const byte PIN_DIO_G1 = 8;
const byte PIN_DIO_G2 = 7;
const byte PIN_DIO_G3 = 6;

const byte PIN_DIO_R1 = 5;
const byte PIN_DIO_R2 = 4;
const byte PIN_DIO_R3 = 3;

SevenSegmentExtended green1(PIN_CLK_Green, PIN_DIO_G1);
SevenSegmentExtended green2(PIN_CLK_Green, PIN_DIO_G2);
SevenSegmentExtended green3(PIN_CLK_Green, PIN_DIO_G3);

SevenSegmentExtended red1(PIN_CLK_Red, PIN_DIO_R1);
SevenSegmentExtended red2(PIN_CLK_Red, PIN_DIO_R2);
SevenSegmentExtended red3(PIN_CLK_Red, PIN_DIO_R3);

SevenSegmentExtended yellow1(PIN_CLK_Yellow, PIN_DIO_Y1);
SevenSegmentExtended yellow2(PIN_CLK_Yellow, PIN_DIO_Y2);
SevenSegmentExtended yellow3(PIN_CLK_Yellow, PIN_DIO_Y3);

SegmentRow rowDestinationTime(red1, red2, red3);
SegmentRow rowPresentTime(green1, green2, green3);
SegmentRow rowDepartedTime(yellow1, yellow2, yellow3);

bool initialized = false;
int lastMinute = -1;
int backlight = 10;

void bootstrapSegments() {
  pinMode(PIN_CLK_Green, OUTPUT);
  pinMode(PIN_DIO_G1, OUTPUT);
  pinMode(PIN_DIO_G2, OUTPUT);
  pinMode(PIN_DIO_G3, OUTPUT);

  pinMode(PIN_CLK_Red, OUTPUT);
  pinMode(PIN_DIO_R1, OUTPUT);
  pinMode(PIN_DIO_R2, OUTPUT);
  pinMode(PIN_DIO_R3, OUTPUT);
  
  pinMode(PIN_CLK_Yellow, OUTPUT);
  pinMode(PIN_DIO_Y1, OUTPUT);
  pinMode(PIN_DIO_Y2, OUTPUT);
  pinMode(PIN_DIO_Y3, OUTPUT);

  rowDepartedTime.start(backlight);
  rowDestinationTime.start(backlight);
  rowPresentTime.start(backlight);
}

void initializeSegments() 
{
}

unsigned long getDCFTime()
{ 
  return 100L;
}

uint8_t sample_input_pin() {
    const uint8_t sampled_data = dcf77_inverted_samples ^ digitalRead(DCF_PIN);
    return sampled_data;
}

void setup() 
{
  using namespace Clock;
  pinMode(DCF_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  bootstrapSegments();
  delay(3000);

  DCF77_Clock::setup();
  DCF77_Clock::set_input_provider(sample_input_pin);

  for (uint8_t state = Clock::useless;
    state == Clock::useless || state == Clock::dirty;
    state = DCF77_Clock::get_clock_state()) {

    // wait for next sec
    Clock::time_t now;
    DCF77_Clock::get_current_time(now);

    // render one dot per second while initializing
    static uint8_t count = 0;
    sprint('.');
    ++count;
    if (count == 60) {
        count = 0;
        sprintln();
    }
  }

  rowDestinationTime.updateRow("0516", random(1900, 2300), 10, 07);
  rowDepartedTime.updateRow("0516", 1987, 14, 07);
  initialized = true;
}

void printTimeToSegments() 
{
  Clock::time_t now;
  DCF77_Clock::get_current_time(now);
  switch (DCF77_Clock::get_clock_state()) {
    case Clock::useless: sprint(F("useless ")); break;
    case Clock::dirty:   sprint(F("dirty:  ")); break;
    case Clock::synced:  sprint(F("synced: ")); break;
    case Clock::locked:  sprint(F("locked: ")); break;
  }
  int currentMinute = now.minute.val;
  if (lastMinute == currentMinute) {
    return;
  }

  char date[] = "";
  sprintf(date, "%02d%02d", now.month.val, now.day.val);
  rowPresentTime.updateRow(date, now.year.val, now.hour.val, now.minute.val);
  lastMinute = currentMinute;
}

void loop() 
{
  printTimeToSegments();
}