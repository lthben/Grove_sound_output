//LED bar to port D2 and set clock pin to 3, data pin to 2
//sound/loudness sensor to port A0
//LCD to I2C port

#define LOUDNESS_SENSOR 0
#define SOUND_SENSOR 1

//USER SETTINGS
const int sensor_type = SOUND_SENSOR;

const int pinAdc = A0;

#include <Grove_LED_Bar.h>
Grove_LED_Bar bar(3, 2, 0, LED_BAR_10); // Clock pin, Data pin, Orientation

#include "rgb_lcd.h"
rgb_lcd lcd;

void setup()
{
  Serial.begin(9600);

  bar.begin();

  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255); //set RGB colour and brightness here
}

void loop()
{
  float db;

  if (sensor_type == SOUND_SENSOR)
  {
    long sum = 0;

    for (int i = 0; i < 256; i++)
    {
      sum += analogRead(pinAdc);
    }

    sum >>= 8;

    Serial.print(sum); //seems to range from 100 to 600

    db = convert_to_db(sum);
    Serial.print("\t"); Serial.print(db); Serial.println("dB\n");



    delay(10);
  }
  else //loudness sensor
  {
    int loudness = analogRead(A0);
    Serial.println(loudness);
    delay(500);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("sound level:");
  lcd.setCursor(0, 1);
  
  if (db < 80) 
  {
    lcd.print("<80dB - safe");
    set_ledbar_level(7);//8 green bars
  }
  else if (db < 91) 
  {
    lcd.print("<91dB - caution");
    set_ledbar_level(8);//plus 1 orange bar
  }
  else 
  {
    lcd.print(">91dB - danger!");
    set_ledbar_level(9);//plus the red bar
  }
}

//convert raw reading into db
float convert_to_db(long _sum)
{
  if (_sum > 405) return 104.6;
  else if (_sum > 400) return map(_sum, 400, 405, 102.6, 104.6);
  else if (_sum > 390) return map(_sum, 390, 300, 100.8, 102.6);
  else if (_sum > 380) return map(_sum, 380, 390, 98, 100.8);
  else if (_sum > 365) return map(_sum, 365, 380, 95.5, 98);
  else if (_sum > 350) return map(_sum, 350, 365, 93, 95.5);
  else if (_sum > 330) return map(_sum, 330, 350, 90.6, 93);
  else if (_sum > 300) return map(_sum, 300, 330, 87.6, 90.6);
  else if (_sum > 270) return map(_sum, 270, 300, 84.7, 87.6);
  else if (_sum > 245) return map(_sum, 245, 270, 81.2, 84.7);
  else if (_sum > 220) return map(_sum, 220, 245, 77.3, 81.2);
  else if (_sum <= 220) return 77.3;
}

//10 leds so data has to be int range of 0 - 9
void set_ledbar_level (int _data)
{
  switch (_data)
  {
    case (0): bar.setBits(0b000001000000000); break;
    case (1): bar.setBits(0b000001100000000); break;
    case (2): bar.setBits(0b000001110000000); break;
    case (3): bar.setBits(0b000001111000000); break;
    case (4): bar.setBits(0b000001111100000); break;
    case (5): bar.setBits(0b000001111110000); break;
    case (6): bar.setBits(0b000001111111000); break;
    case (7): bar.setBits(0b000001111111100); break;
    case (8): bar.setBits(0b000001111111110); break;
    case (9): bar.setBits(0b000001111111111); break;
  }
  delay(1000); //need at least 1 sec delay
}
