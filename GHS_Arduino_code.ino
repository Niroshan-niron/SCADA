#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
#include <LiquidCrystal.h>

#define mainLedPin 6
#define fanPin 7  
#define heaterPin 5
#define pump 2  
#define startdummy 3

const int tempPin = A0;
const int humiPin = A1;
const int ldr = A2;
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned long previousMillis = 0;
const long lcdUpdateInterval = 1000;

int manualstat = 0;

modbusDevice regBank;
modbusSlave slave;

void setup()
{   
  pinMode(heaterPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(mainLedPin,OUTPUT);
  pinMode(pump,OUTPUT);

  pinMode(tempPin,INPUT);
  pinMode(ldr,INPUT);
  pinMode(humiPin,INPUT);
  
  lcd.begin(16, 2);
  
//Assign the modbus device ID.  
  regBank.setId(1);

//Add Digital Output registers 00001-00016 to the register bank
  regBank.add(1);
  regBank.add(2);
  regBank.add(3);
  regBank.add(4);
  regBank.add(5);
  regBank.add(6);
  regBank.add(7);
  regBank.add(8);
  regBank.add(9);
  regBank.add(10);
  regBank.add(11);
  regBank.add(12);
  regBank.add(13);
  regBank.add(14);
  regBank.add(15);
  regBank.add(16);

//Add Digital Input registers 10001-10008 to the register bank
  regBank.add(10001);  
  regBank.add(10002);  
  regBank.add(10003);  
  regBank.add(10004);  
  regBank.add(10005);  
  regBank.add(10006);  
  regBank.add(10007);  
  regBank.add(10008);  

//Add Analog Input registers 30001-10010 to the register bank
  regBank.add(30001);  
  regBank.add(30002);  
  regBank.add(30003);  
  regBank.add(30004);  
  regBank.add(30005);  
  regBank.add(30006);  
  regBank.add(30007);  
  regBank.add(30008);  
  regBank.add(30009);  
  regBank.add(30010);
  regBank.add(30011);  
  regBank.add(30012);  
  regBank.add(30013);  
  regBank.add(30014);  
  regBank.add(30015);  
  regBank.add(30016); 

//Add Analog Output registers 40001-40020 to the register bank
  regBank.add(40001);  
  regBank.add(40002);  
  regBank.add(40003);  
  regBank.add(40004);  
  regBank.add(40005);  
  regBank.add(40006);  
  regBank.add(40007);  
  regBank.add(40008);  
  regBank.add(40009);  
  regBank.add(40010);  
  regBank.add(40011);  
  regBank.add(40012);  
  regBank.add(40013);  
  regBank.add(40014);  
  regBank.add(40015);  
  regBank.add(40016);  
  regBank.add(40017);  
  regBank.add(40018);  
  regBank.add(40019);  
  regBank.add(40020);  

  slave._device = &regBank;  
  
  slave.setBaud(9600);   
}
void loop()
{
//put some data into the registers
  regBank.set(1, 0);  
  regBank.set(2, 0);  
  regBank.set(3, 0);  
  regBank.set(4, 0);  
  regBank.set(5, 0);  
  regBank.set(6, 0);  
  regBank.set(7, 0);  
  regBank.set(8, 0);

  regBank.set(10001, 0);
  regBank.set(10002, 0);  
  regBank.set(10003, 0);  
  regBank.set(10004, 1);  
  regBank.set(10005, 0);  
  regBank.set(10006, 0);  
  regBank.set(10007, 0);  
  regBank.set(10008, 0);  

  regBank.set(30001,0);
  regBank.set(30002,4);
  regBank.set(30003,0);
  regBank.set(30004,4);
  regBank.set(30005,4);
  regBank.set(30006,4);
  regBank.set(30007,4);
  regBank.set(30008,4);
  regBank.set(30009,4);
  regBank.set(30010,0);
  regBank.set(30011,0);  
  regBank.set(30012,0);  
  regBank.set(30013,0);  
  regBank.set(30014,0);  
  regBank.set(30015,0);  
  regBank.set(30016,0);

  regBank.set(40001,1);
  regBank.set(40002,2);
  regBank.set(40003,2);
  regBank.set(40004,4);
  regBank.set(40005,5);
  regBank.set(40006,6);
  regBank.set(40007,7);
  regBank.set(40008,8);
  regBank.set(40009,9);
  regBank.set(40010,10);
  
 while(1)
  {
    int start = regBank.get(40001);// turn on the system
    int set = regBank.get(0006);// manual or auto
    int fix = regBank.get(0005);// set the temperature and humidity

    if(start >= 1) // turn on the system and components
  {

    int sensorValue1 = analogRead(tempPin);
    int temperature = (sensorValue1 / 7);
    regBank.set(30001, temperature);

    int sensorValue2 = analogRead(humiPin);
    int humidity = (sensorValue2 / 7);
    regBank.set(30002, humidity);

    int sensorValue3 = analogRead(ldr);
    int light = (sensorValue3);
    regBank.set(30003, light);


    if(set >= 1)// System is auto mode
  { 

    if (humidity > 50)
    {
        regBank.set(40004, 1);
        digitalWrite(fanPin, HIGH);
        regBank.set(40007, 4);
        digitalWrite(pump, LOW);
    }
    if (humidity < 30)
    {
        regBank.set(40007, 1);
        digitalWrite(pump, HIGH);
        regBank.set(40004, 4);
        digitalWrite(fanPin, LOW);
    }
    if (humidity == 40)
    {
        regBank.set(40007, 4);
        digitalWrite(pump, LOW);
        regBank.set(40004, 4);
        digitalWrite(fanPin, LOW);
    }
    if (light < 500)
    {
        regBank.set(40005, 1);
        digitalWrite(mainLedPin, HIGH);
    }
    if(light > 500)
    {
        regBank.set(40005, 4);
        digitalWrite(mainLedPin, LOW);
    }
    if (temperature > 40)
    {
        regBank.set(40004, 1);
        regBank.set(40006, 4);
        digitalWrite(fanPin, HIGH);
        digitalWrite(heaterPin, LOW);
    }
    if (temperature == 30)
    {
        regBank.set(40004, 4);
        regBank.set(40006, 4);
        digitalWrite(fanPin, LOW);
        digitalWrite(heaterPin, LOW);
    }
    if (temperature < 15)
    {
        regBank.set(40004, 4);
        regBank.set(40006, 1);
        digitalWrite(fanPin, LOW);
        digitalWrite(heaterPin, HIGH);
    }
    }//end of the auto

  
    if(set < 1)//System is manual
  {

    if (regBank.get(0001) >= 1)
    {
        regBank.set(40004, 1);
        digitalWrite(fanPin, HIGH);
    }
    if (regBank.get(0001) < 1)
    {
        regBank.set(40004, 0);
        digitalWrite(fanPin, LOW);
    }
    if (regBank.get(0002) >= 1)
    {
        regBank.set(40005, 1);
        digitalWrite(mainLedPin, HIGH);
    }
    if (regBank.get(0002) < 1)
    {
        regBank.set(40005, 0);
        digitalWrite(mainLedPin, LOW);
    }
    if (regBank.get(0003) >= 1)
    {
        regBank.set(40006, 1);
        digitalWrite(heaterPin, HIGH);
    }
    if (regBank.get(0003) < 1)
    {
        regBank.set(40006, 0);
        digitalWrite(heaterPin, LOW);
    }
    if (regBank.get(0004) >= 1)
    {
        regBank.set(40007, 1);
        digitalWrite(pump, HIGH);
    }
    if (regBank.get(0004) < 1)
    {
        regBank.set(40007, 0);
        digitalWrite(pump, LOW);
    }
    } //end of manual

   
    if(fix >= 1) // set the temp and humidity value
  {
    if (temperature < regBank.get(40002))
      {
        regBank.set(40006, 1);
        digitalWrite(heaterPin, HIGH);// Turn on the heater
      }
      else
      {
        regBank.set(40006, 0);
        digitalWrite(heaterPin, LOW);// Turn off the heater
      }
      if (humidity < regBank.get(40003))
      {
        regBank.set(40007, 1);
        digitalWrite(pump, HIGH);// Turn on the pump
      }
      else
      {
        regBank.set(40007, 0);
        digitalWrite(pump, LOW);// Turn off the pump
      }
    }// end of fix option
    
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= lcdUpdateInterval) 
  {
    previousMillis = currentMillis;

  lcd.clear(); // Clear the LCD screen

  lcd.setCursor(0, 0);
  lcd.print("FN: ");
  lcd.print(regBank.get(40004) == 1 ? "on   " : "off  ");

  lcd.setCursor(0, 1);
  lcd.print("HT: ");
  lcd.print(regBank.get(40006) == 1 ? "on   " : "off  ");

  lcd.setCursor(8, 0);
  lcd.print("BLB: ");
  lcd.print(regBank.get(40005) == 1 ? "on   " : "off  ");

  lcd.setCursor(8, 1);
  lcd.print("SPR: ");
  lcd.print(regBank.get(40007) == 1 ? "on   " : "off  ");  
   }
    
    slave.run();

  }
   if(start < 1)// Turn off all components when the system is off
  {
    digitalWrite(mainLedPin, LOW);
    regBank.set(40005, 0);
    digitalWrite(fanPin, LOW);
    regBank.set(40004, 0);
    digitalWrite(heaterPin, LOW);
    regBank.set(40006, 0);
    digitalWrite(pump, LOW);
    regBank.set(40007, 0);
    lcd.noDisplay();
} 
else
{
  lcd.display();
}
slave.run();
}
}
