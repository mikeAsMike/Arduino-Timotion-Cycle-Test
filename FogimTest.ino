#include <LiquidCrystal_I2C.h>

//Pinout for Arduino and relay:
//Arduino D2 to Relay IN1
//Arduino D4 to Relay IN2
//Arduino D7 to Relay IN3
//Arduino D8 to Relay IN4
//Ground both relay GND to Arduino GND
//Arduino D11 is constant power for relay bank VCC
//Arduino Vin to relay JD-VCC

//Pinout for LED with I2C:
//Arduino A5 to I2C SDL
//Arduino A4 to I2C SDA
//Arduino 5V to I2C VCL
//Arduino GND to I2C GND

//Runs two Timotion linear actuators
//Must tap into up and down signal wires in hand control
//Relays emulate holding the hand control up and down buttons

//  set address to 0x27 for the 20 chars and 4 lines
LiquidCrystal_I2C lcd(0x27, 20, 4);

// set variables for the pins that control the relays
int pinUp = 2;
int pinDown = 4;

int pinUpTwo = 7;
int pinDownTwo = 8;

// declare variable for pin to provide constant power to relay bank
int pinPower = 11;

// declare variable to be incremented in the while loop
int count = 0;

// single variable to set cycle count
int cycles = 5;

// cycle time for going up. Make sure time exceeds amount of time to lift
int upTime = 3;

// cycle time for coming down. Make sure time exceeds amount of time to come down
int downTime = 3;

// message variable
String setStatus;

//Clears status portion of the LCD screen to prepare it to change message
void ClearStatus() 
    {    
    delay(150);
    lcd.setCursor(0,2);
    lcd.print("                    ");   //Takes 20 charactors to fully erase previous chars 
    delay(150);
      }

//Sets text to the LCD screen
void SetStatusText()
    {
     lcd.setCursor(0,2);
     lcd.print(setStatus);  
      }

//when called, will lift the electric actuators by emulating holding the "up" button on hand control
void goUp()
    {
      digitalWrite(pinUp, LOW);
      digitalWrite(pinDown, HIGH);
      digitalWrite(pinUpTwo, LOW);
      digitalWrite(pinDownTwo, HIGH);
      // use upTime variable
      delay(upTime * 1000);      
      digitalWrite(pinUp, HIGH);
      digitalWrite(pinUpTwo, HIGH);
      delay(1000);
      }

//when called, will lower the electric actuators by emulating holding the "down" button on hand control
void goDown()
    {
      digitalWrite(pinUp, HIGH);
      digitalWrite(pinDown, LOW);
      digitalWrite(pinUpTwo, HIGH);
      digitalWrite(pinDownTwo, LOW);
      // use downTime variable
      delay(downTime * 1000); 
      digitalWrite(pinDown, HIGH);
      digitalWrite(pinDownTwo, HIGH);
      delay(1000);
      }

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  lcd.init();

  pinMode(pinUp, OUTPUT);
  pinMode(pinUpTwo, OUTPUT);

  pinMode(pinDown, OUTPUT);
  pinMode(pinDownTwo, OUTPUT);

  pinMode(pinPower, OUTPUT);
  

  // LCD screen details
  lcd.backlight();

  // set cycle number
  lcd.setCursor(0,0);
  lcd.print("Cycle number: ");

  // set status number
  setStatus = "Status: Initializing";
  lcd.setCursor(0,2);
  lcd.print(setStatus); 
  
  // begin all pins high which means normally-closed relay banks are open. LOW actuates relays, HIGH turns them off.
  digitalWrite(pinUp, HIGH);
  digitalWrite(pinDown, HIGH);
  digitalWrite(pinUpTwo, HIGH);
  digitalWrite(pinDownTwo, HIGH);

  // change status on LCD screen from "Initializing" to "Testing"
  setStatus = "Status: Testing";
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  
  ClearStatus();
  SetStatusText();

  // energize Arduino pin D11 to provide constant power to the relay board
  digitalWrite(pinPower, HIGH);

  // the test cycle
  while (count < cycles) {

    goUp();
    //delay(1000);    
    goDown();

    count = count + 1;
    Serial.print("Cycle count: ");
    Serial.println(count);

    lcd.setCursor(14,0);
    lcd.print(count);

      digitalWrite(pinDown, HIGH);
      digitalWrite(pinDownTwo, HIGH);

    // end test cycle
    if (count == cycles){
      Serial.println("Test is done.");    
      ClearStatus();
      setStatus = "Status: Finished";
      SetStatusText();
      }
            
    }
}
