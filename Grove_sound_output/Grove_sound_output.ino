//LED bar to port D2 and set clock pin to 3, data pin to 2
//sound sensor to port A0
//LCD to I2C port

const int pinAdc = A0;

#include <Grove_LED_Bar.h>
Grove_LED_Bar bar(3, 2, 0, LED_BAR_10); // Clock pin, Data pin, Orientation

#include "rgb_lcd.h"
rgb_lcd lcd;

void setup()
{
    Serial.begin(115200);
    Serial.println("Grove - Sound Sensor Test...");
    
    bar.begin();

    lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255); //set RGB colour and brightness here
}

void loop()
{
    long sum = 0;
    
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinAdc);
    }

    sum >>= 5;

    Serial.println(sum); //seems to range from 100 to 600

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sound level:");
    lcd.setCursor(0,1);
    lcd.print(sum);

    sum = constrain(sum, 0.0, 600.0);
    float data = map(sum, 0.0, 600.0, 0.0, 9.0); 
    set_ledbar_level(data);
    
    delay(10);
}

//10 leds so data has to be int range of 0 - 9
void set_ledbar_level (int _data) 
{
   switch(_data) 
    {
        case(0): bar.setBits(0b000001000000000); break;
        case(1): bar.setBits(0b000001100000000); break;
        case(2): bar.setBits(0b000001110000000); break;
        case(3): bar.setBits(0b000001111000000); break;
        case(4): bar.setBits(0b000001111100000); break;
        case(5): bar.setBits(0b000001111110000); break;
        case(6): bar.setBits(0b000001111111000); break;
        case(7): bar.setBits(0b000001111111100); break;
        case(8): bar.setBits(0b000001111111110); break;
        case(9): bar.setBits(0b000001111111111); break;
   }
   delay(1000); //need at least 1 sec delay
}
