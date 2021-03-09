#include "Arduino.h"
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "SegmentRow.h"

#include "DCF77.h"
#include "Time.h"

#define DCF_PIN 2	         // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0		 // Interrupt number associated with pin

time_t time;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT, false);

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
int backlight = 50;

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


void setup() {
  pinMode(DCF_PIN, INPUT);
  Serial.begin(9600);

  bootstrapSegments();
  delay(3000);
  DCF.Start();
}

bool printTimeToSegments() {
  if (!initialized) {
    return false;
  }

  int currentMinute = minute();
  if (lastMinute == currentMinute) {
    return false;
  }

  char sa[] = "";
  sprintf(sa, "%02d%02d", month(), day());
  green1.print(sa);
  green2.print(year());
  green3.printTime(hour(), minute(), false); 

  lastMinute = currentMinute;

  return true;
}

bool initializeSegments() {
  if (initialized) {
    return false;
  }

  red1.print("0516");
  red2.print(2028);
  red3.printTime(10, 07, false);

  yellow1.print("0516"); 
  yellow2.print(1987);
  yellow3.printTime(14, 07, false);

  initialized = true;

  return true;
}
void loop() {
  time_t DCFtime = DCF.getTime();
  if (DCFtime!=0) {
    Serial.println("Time is updated");
    setTime(DCFtime);
    initializeSegments();
  }	
  
  delay(1000);
  printTimeToSegments();
}