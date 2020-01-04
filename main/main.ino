/*

Power consumption meter

- Max: 20V supply
- Max: 400mA load (optional 1A)
*/

//#define MAX400mA

/*
- 0.1mA resolution
- Measure frequency 70Hz
Shows:
- mA and mW
- average mA and mW since start
- mAh and mW since start
- load V
- measure frequency
- run time


Ardunio code included



Parts needed:
- Arduino Nano - ATmega328P
- Adafruit Feather INA219
- Display 0.91 inch 128x32 i2c SSD1306

Optional:
- 1A Schottky diode
- 100uF 30V


*/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>

#define BUTTON 2



Adafruit_SSD1306 display;
Adafruit_INA219 ina219;



void setup()   {

  Serial.begin(115200);
  Serial.println("Starting up...");

  pinMode(BUTTON, INPUT_PULLUP);
  
  ina219.begin();

#ifdef MAX400mA  
  ina219.setCalibration_16V_400mA();
#else
  ina219.setCalibration_32V_1A();     // OPTIONAL RANGE. BUT STILL MAX 20V to the Atmega328 Arduino Nano board power supply "VIN" pin
#endif

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Wire.setClock(400000);
  
  display.clearDisplay();
  display.display();
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.clearDisplay();
  display.setCursor(0,0);
#ifdef MAX400mA  
  display.setCursor(0,6);
  display.print("Max: 400mA load");
  display.setCursor(0, 21);
  display.print("0.1mA resolution");
#else
  display.setCursor(0,6);
  display.print("Max: 1A load");
  display.setCursor(0, 21);
  display.print("0.2mA resolution");
#endif
  display.display();
  delay(1500);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("- Now:");
  display.setCursor(0, 12);
  display.print("- Total Average:");
  display.setCursor(0, 25);
  display.print("- Total:");
  display.display();
  delay(2000);

  Serial.println("Power consumption meter");
  Serial.println("Max: 20V supply");
#ifdef MAX400mA  
  Serial.println("Max: 400mA load");
  Serial.println("0.1mA resolution");
#else
  Serial.println("Max: 1A load");
  Serial.println("0.2mA resolution");
#endif
  Serial.println();
  Serial.print("Runtime ");
  Serial.print("\t");
  Serial.print("Sec");
  Serial.print("\t");
  Serial.print("Volt ");
  Serial.print("\t");
  Serial.print("mA   ");
  Serial.print("\t");
  Serial.print("avg mA");
  Serial.print("\t");
  Serial.print("mW   ");
  Serial.print("\t");
  Serial.print("avg mW");
  Serial.print("\t");
  Serial.print("mAh  ");
  Serial.print("\t");
  Serial.print("mWh  ");
  Serial.println();
}




void loop() {
  
  static unsigned long period_cntr = 0;
  static unsigned long total_cntr = 0;
  static unsigned long total_tenths = 0;

  static float period_sum_V = 0.0;  
  static float period_sum_mA = 0.0;  
  static float period_sum_mW = 0.0;  
  
  static float total_sum_V = 0.0;
  static float total_sum_mA = 0.0;
  static float total_sum_mW = 0.0;
  
  float now_V;  
  float now_mA;
  float now_mW;

  float period_average_V;  
  float period_average_mA;  
  float period_average_mW;  

  float total_average_V;  
  float total_average_mA;  
  float total_average_mW;  

  float total_mAh;
  float total_mWh;


  ////////////////////////////////////////////

  unsigned long now = millis();
    
  total_cntr++;
  period_cntr++;

  now_V = ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV()/1000);  
  now_mA = ina219.getCurrent_mA();
  now_mW = now_V * now_mA;

  period_sum_V += now_V;
  period_sum_mA += now_mA;
  period_sum_mW += now_mW;

  total_sum_V += now_V;
  total_sum_mA += now_mA;
  total_sum_mW += now_mW;



  #define TIMER 500
  static unsigned long timer = now;
  if (now-timer >= TIMER) {
    
    total_tenths += (now-timer)/100;
    timer = now;
  
    period_average_V  = period_sum_V  / period_cntr;  
    period_average_mA = period_sum_mA / period_cntr;  
    period_average_mW = period_sum_mW / period_cntr;  
    
    total_average_V  = total_sum_V  / total_cntr;  
    total_average_mA = total_sum_mA / total_cntr;  
    total_average_mW = total_sum_mW / total_cntr;  

    total_mAh = total_average_mA * total_tenths / 36000;  
    total_mWh = total_average_mW * total_tenths / 36000;  


    //////////// DISPLAY ///////////

    display.clearDisplay();
    if (digitalRead(BUTTON)) {
      display.setCursor(0,0);
      printSIValue(period_average_mA/1000, "A", 4, 10);
      display.setCursor(0, 12);
      printSIValue(total_average_mA/1000, "A", 4, 10);
      display.setCursor(0, 25);
      printSIValue(total_mAh/1000, "Ah", 4, 10);
      display.setCursor(68,0);
      printSIValue(period_average_mW/1000, "W", 4, 10);
      display.setCursor(68, 12);
      printSIValue(total_average_mW/1000, "W", 4, 10);
      display.setCursor(68, 25);
      printSIValue(total_mWh/1000, "Wh", 4, 10);
      
    }
    else {
      display.setCursor(17, 0);
      printMinutes((float)total_tenths);
      display.setCursor(0,25);
      printSIValue(period_average_V, "V", 3, 10);
      display.setCursor(68,25);
      printSIValue((int)(10*total_cntr/total_tenths), "Hz", 0, 10);
    }
    display.display();



    //////////// SERIAL OUTPUT ///////////

    if (total_tenths/36000<10) {
      Serial.print('0');
    }
    Serial.print(total_tenths/36000);
    Serial.print(":");
    
    if (total_tenths/600%60<10) {
      Serial.print('0');
    }
    Serial.print(total_tenths/600%60);
    Serial.print(":");
    
    if ((total_tenths/10)%60<10) {
      Serial.print('0');
    }
    Serial.print((total_tenths/10)%60);
    Serial.print("\t");

    Serial.print(total_tenths/10);
    
    Serial.print("\t");
    Serial.print(period_average_V, 3);
    Serial.print("\t");
    Serial.print(period_average_mA, 2);
    Serial.print("\t");
    Serial.print(total_average_mA, 2);
    Serial.print("\t");
    Serial.print(period_average_mW, 2);
    Serial.print("\t");
    Serial.print(total_average_mW, 2);
    Serial.print("\t");
    Serial.print(total_mAh, 3);
    Serial.print("\t");
    Serial.print(total_mWh, 3);
    Serial.println();


    period_cntr = 0;
    period_sum_V = 0;  
    period_sum_mA = 0;  
    period_sum_mW = 0;  


  }
}





// ------------------------------------------------------------------------------------------------------------

void printSIValue(float orgvalue, char* units, int precision, int maxWidth) {
  display.setTextSize(1);

  float value = fabs(orgvalue);
  if (fabs(value) < 1.0) {
    maxWidth -= 1; // for "m"
    value *= 1000.0;
    precision = max(0, precision-3);
  }
  maxWidth -= strlen(units);
  int digits = ceil(log10(fabs(value)));  // Find how many digits are in value
  if (fabs(value) <= 1.0) {
    digits = 1;
  }
  maxWidth -= 1; // for extra space between value and units
  int actualPrecision = constrain(maxWidth-digits-1, 0, precision);
  actualPrecision = constrain(actualPrecision, 1, 3);
  int padding = maxWidth-digits-1-actualPrecision;
  for (int i=0; i < padding; ++i) {
    display.print(' ');
  }
  display.print(value, actualPrecision);
  display.print(' ');
  if (fabs(orgvalue) < 1.0) {
    display.print('m');
  }
  display.print(units);
}



// ------------------------------------------------------------------------------------------------------------

void printMinutes(unsigned long total_tenths) {
  display.setTextSize(2);

  if (total_tenths/36000<10) {
    display.print('0');
  }
  display.print(total_tenths/36000);
  display.print(":");
  
  if (total_tenths/600%60<10) {
    display.print('0');
  }
  display.print(total_tenths/600%60);
  display.print(":");
  
  if ((total_tenths/10)%60<10) {
    display.print('0');
  }
  display.print((total_tenths/10)%60);
  
}



// ------------------------------------------------------------------------------------------------------------
