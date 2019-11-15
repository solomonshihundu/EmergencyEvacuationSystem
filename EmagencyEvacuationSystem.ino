
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>


//indicates smoke level is above safe value
int redLed = 8;

//indicates smoke level is within the safe value
int amberLed = 10;

//indicates smoke level is well below safe value
int greenLed = 9;

int smokePin = A0;
int fan = 7;
int fanLed = 3;

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial GSMSerial(5,6 );

// Your threshold value
int criticalSmokeLevel = 400;
int norminalSmokeLevel = 350;
int smokeLevel;
String level = " LEVEL : ";
String levelOutput  = level+ (String)smokeLevel;
String levelPrint = "";

String myPhone = "+254729893875";

String fireMsg = "111";
String freshAir = "000";
String roomStuffy = "010";
String criticalSmoke = "101";

int flamePin = 12;
bool flamePresent = LOW;

    void setup()
    {
      lcd.begin();
      lcd.backlight();
      lcd.print("Stand by");
      
      pinMode(redLed, OUTPUT);
      pinMode(greenLed, OUTPUT);
      pinMode(amberLed, OUTPUT);
      pinMode(smokePin, INPUT);
      pinMode(flamePin,INPUT);
      pinMode(fan,OUTPUT);
      pinMode(fanLed,OUTPUT);
      Serial.begin(9600);
      GSMSerial.begin(9600);

      GSMSerial.begin(9600);   // Setting the baud rate of GSM Module  
      
      //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
      Serial.begin(9600);
      
      //Begin serial communication with Arduino and SIM800L
      GSMSerial.begin(9600);
      
      Serial.println("Initializing..."); 
      delay(1000);
      
      GSMSerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      
    }

void loop() 
{

        digitalWrite(fan, LOW);
        digitalWrite(fanLed, HIGH);
        
        delay(10000);
        
        digitalWrite(fanLed, LOW);
        digitalWrite(fan, HIGH);
        
        delay(3000);
        
        smokeLevel = analogRead(smokePin);
        flamePresent = digitalRead(flamePin);
        
        //flame sensor output is active LOW
        if(!flamePresent)
        {
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" FIRE ALERT !");
        
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, HIGH);
        digitalWrite(amberLed, HIGH);
        sendMessage(fireMsg,myPhone);
       
        delay(100);
        
        } 
        
        else if(smokeLevel <= norminalSmokeLevel)
        {
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
        digitalWrite(amberLed, LOW);
        
        lcd.clear();
        lcd.noAutoscroll();
        lcd.setCursor(0,0);
        lcd.print(" FRESH N CLEAN");
        
        levelOutput = (String)smokeLevel;
        levelPrint = level + levelOutput;
        lcd.setCursor(0,1);
        lcd.print(levelPrint);
        Serial.println(levelPrint);

        sendMessage(freshAir,myPhone);
        
        delay(100);
        
        }
        
        else if(!(smokeLevel > criticalSmokeLevel) && smokeLevel > norminalSmokeLevel)
        {
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, LOW);
        digitalWrite(amberLed, HIGH);
        
        lcd.clear();
        lcd.noAutoscroll();
        lcd.setCursor(0,0);
        lcd.print(" ROOM IS STUFFY");
        levelOutput = (String)smokeLevel;
        levelPrint = level + levelOutput;
        lcd.setCursor(0,1);
        lcd.print(levelPrint);
        Serial.println(levelPrint);
        
        sendMessage(roomStuffy,myPhone);
        
        delay(100);
        
        }
        
        else if ((smokeLevel >= criticalSmokeLevel))
        {
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, LOW);
        digitalWrite(amberLed, LOW);
        
        lcd.setCursor(0,0);
        lcd.print(" STATUS CRITICAL EVACUATE IMMEDIATELY BUILDING IS ON FIRE ");
        lcd.autoscroll();
        
        sendMessage(criticalSmoke,myPhone);
        
        delay(100);
        
        }
        
        
        delay(1000);
}

  void sendMessage(String message,String phone)
{
  GSMSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  GSMSerial.println("AT+CMGS=\""+phone+"\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  GSMSerial.print(message); //text content
  updateSerial();
  GSMSerial.write(26);
  delay(1000);
}


 void RecieveMessage()
{
  GSMSerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    GSMSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(GSMSerial.available()) 
  {
    Serial.write(GSMSerial.read());//Forward what Software Serial received to Serial Port
  }
}
