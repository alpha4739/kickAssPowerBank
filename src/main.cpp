#include <Arduino.h>
#include <math.h>
#include <Battery.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
// #include <ArduinoIoTCloud.h>

#ifdef ESP8266
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
#define STMPE_CS 32
#define TFT_CS   15
#define TFT_DC   33
#define SD_CS    14

#else
// Anything else, defaults!
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#endif


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

float cell1V = 0.00;
float cell2V = 0.00;
float cell3V = 0.00;
float cell4V = 0.00;
float calV = 0.00;
float supplyV = 0.00;
float cell1Cal = 50.00 - 2.80;
float cell2Cal = 50.00 - 6.65;
float cell3Cal = 50.00 - 7.85;
float cell4Cal = 50.00 - 16.90;
float zenerV = 4.0;
float cell1AverageVal = 0.00;
float cell2AverageVal = 0.00;
float cell3AverageVal = 0.00;
float cell4AverageVal = 0.00;
float cell1AverageV = 0.00;
float cell2AverageV = 0.00;
float cell3AverageV = 0.00;
float cell4AverageV = 0.00;
float totalV = 0.00;

// float balanceVal = 4.20; FUTURE CELL BALANCER

int zenerVal = 0;
int cell1Val = 0;
int cell2Val = 0;
int cell3Val = 0;
int cell4Val = 0;
int averages = 100;

/* FUTURE CELL BALANCER
int cell1Bal = 2;
int cell1Bal = 3;
int cell1Bal = 4;
int cell1Bal = 5;
*/

    unsigned long tftText(){
    unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  //tft.setTextSize(1.0);
  tft.print("1-");
  tft.print(cell1AverageV);
  tft.print("v");
  tft.print(" | ");

 // tft.setCursor(0, 20);
  //tft.setTextSize(1.0);
  tft.print("2-");
  tft.print(cell2AverageV);
  tft.print("v");
  tft.println(" ");
  tft.println(" ");

  //tft.setCursor(0, 40);
  //tft.setTextSize(1.0);
  tft.print("3-");
  tft.print(cell3AverageV);
  tft.print("v");
  tft.print(" | ");

  //tft.setCursor(0, 60);
  //tft.setTextSize(1.0);
  tft.print("4-");
  tft.print(cell4AverageV);
  tft.print("v");
  tft.println(" ");
  tft.println(" ");

 // tft.setCursor(0, 80);
  //tft.setTextSize(1.0);
  tft.print("Total - ");
  tft.print(totalV);
  tft.println("v");
    return micros() - start;
    }
//tft.print("zener Value = ");
//tft.println(zenerVal);

void setup() {
  Serial.begin(115200);
  tft.begin(); // FUTURE TFT SCREEN
  tft.fillScreen(ILI9341_BLACK); // FUTURE TFT SCREEN
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3.0);
  uint8_t rotation = 1; { 
  tft.setRotation(rotation);
  }

  

 /* FUTURE CELL BALANCER
 pinMode(cell1Bal, OUTPUT);
  pinMode(cell2Bal, OUTPUT);
  pinMode(cell3Bal, OUTPUT);
  pinMode(cell4Bal, OUTPUT);
  */
}

void loop() {

for (int i = 0; i < averages; i++){
    zenerVal = analogRead(A4);
    supplyV = (zenerV * 4095.00) / zenerVal;
    calV = (supplyV - 5.00) * 10.00;

    cell1Val = analogRead(A0);
    cell2Val = analogRead(A1);
    cell3Val = analogRead(A5);
    cell4Val = analogRead(A6);

    cell1V = (cell1Val * (cell1Cal + calV)) / 4095;
    cell2V = (cell2Val * (cell2Cal + calV)) / 4095;
    cell3V = (cell3Val * (cell3Cal + calV)) / 4095;
    cell4V = (cell4Val * (cell4Cal + calV)) / 4095;

    cell1AverageVal = cell1AverageVal + cell1V;
    cell2AverageVal = cell2AverageVal + cell2V;
    cell3AverageVal = cell3AverageVal + cell3V;
    cell4AverageVal = cell4AverageVal + cell4V;
   
    delay(10);
    }
cell1AverageV = cell1AverageVal / averages;
cell2AverageV = cell2AverageVal / averages;
cell3AverageV = cell3AverageVal / averages;
cell4AverageV = cell4AverageVal / averages;

cell2AverageV = cell2AverageV - cell1AverageV;
cell3AverageV = cell3AverageV - (cell2AverageV + cell1AverageV);
cell4AverageV = cell4AverageV - (cell3AverageV + cell2AverageV);

totalV = cell1AverageV + cell2AverageV + cell3AverageV + cell4AverageV;

tftText();
// OPTIONAL - SERIAL MONITOR FOR DEBUGGING
Serial.print("Cell 1 Voltage = ");
Serial.println(cell1AverageV);
Serial.print("Cell 2 Voltage = ");
Serial.println(cell2AverageV);
Serial.print("Cell 3 Voltage = ");
Serial.println(cell3AverageV);
Serial.print("Cell 4 Voltage = ");
Serial.println(cell4AverageV);

Serial.println(" ");

Serial.print("Supply Voltage = ");
Serial.println(supplyV);

Serial.println(" ");

Serial.print("Cell 1 Value = ");
Serial.println(cell1Val);
Serial.print("Cell 2 Value = ");
Serial.println(cell2Val);
Serial.print("Cell 3 Value = ");
Serial.println(cell3Val);
Serial.print("Cell 4 Value = ");
Serial.println(cell4Val);

Serial.println(" ");

Serial.print("Zen Val = ");
Serial.println(zenerVal);

cell1AverageVal = 0;
cell2AverageVal = 0;
cell3AverageVal = 0;
cell4AverageVal = 0;

}




  
