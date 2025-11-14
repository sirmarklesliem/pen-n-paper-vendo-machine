#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


#define coinSlot 9
#define buzzer 13

#define buttonPrinter 5
#define buttonPencil 6
#define buttonPenRed 7
#define buttonPenBlack 8 

#define coinSlotRGBFrame 3
#define printerMotor 4

int coinSlotStatus = 0;
int pulse;
int currentBalance;

int buttonStatePrinter;
int buttonStatePencil;
int buttonStatePenRed;
int buttonStatePenBlack;

boolean userBalance = false;
boolean noCoin = false;

Servo CauponResetServo;  // create servo object to control a servo
Servo PencilServo;  // create servo object to control a servo
Servo PenRedServo;  // create servo object to control a servo
Servo PenBlackServo;  // create servo object to control a servo

// Stock counters
int stockPaper = 50;
int stockPencil = 10;
int stockPenRed = 10;
int stockPenBlack = 10;

char lcdBuffer[20];


void setup(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pen-N-Paper");
  lcd.setCursor(0,1);
  lcd.print("Vendo");
  lcd.setCursor(0,2);
  lcd.print("Machine");
  delay(2000);
  pinMode(buzzer, OUTPUT);
  // pinMode(11, OUTPUT);
  // digitalWrite(11, LOW);

  pinMode(coinSlot, INPUT_PULLUP);
  pinMode(coinSlotRGBFrame, OUTPUT);
  pinMode(buttonPrinter, OUTPUT);
  pinMode(buttonPencil, OUTPUT);
  pinMode(buttonPenRed, OUTPUT);
  pinMode(buttonPenBlack, OUTPUT);
  pinMode(printerMotor, OUTPUT);
  digitalWrite(printerMotor, HIGH);
  digitalWrite(coinSlotRGBFrame, HIGH);

  PenBlackServo.attach(10);  // attaches the servo on pin 10 to the servo objectư
  PenRedServo.attach(11);  // attaches the servo on pin 11 to the servo objectư
  PencilServo.attach(12);  // attaches the servo on pin 12 to the servo objectư
  CauponResetServo.attach(2);  // attaches the servo on pin 3 to the servo objectư
 
  PenBlackServo.write(55);   // rotate slowly servo to 0 degrees immediately
  PenRedServo.write(55);   // rotate slowly servo to 0 degrees immediately
  PencilServo.write(55);   // rotate slowly servo to 0 degrees immediately
  CauponResetServo.write(5);   // rotate slowly servo to 0 degrees immediately

  // Celebration jingle (G5 - B5 - E6)
  tone(buzzer, 784, 150);  // G5
  delay(180);
  tone(buzzer, 988, 150);  // B5
  delay(180);
  tone(buzzer, 1318, 300); // E6 (victory high note)
  delay(350);

  noTone(buzzer);

  lcd.clear();
}

void loop(){
  buttonStatePrinter = digitalRead(buttonPrinter);
  buttonStatePencil = digitalRead(buttonPencil);
  buttonStatePenRed = digitalRead(buttonPenRed);
  buttonStatePenBlack = digitalRead(buttonPenBlack);

  //FOR PAPER DISPENSER
  if(buttonStatePrinter == 1 && pulse > 0 && userBalance && stockPaper > 0){
    for(int i = pulse; i>0 ; i--){
      if (stockPaper <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Paper");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
      //turn on printer motor
      digitalWrite(printerMotor, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Dispensing Paper...");
      lcd.setCursor(0,1);
      lcd.print(i);
      delay(2500);
      tone(buzzer, 2700, 500);
      delay(500);
      noTone(buzzer);
      delay(500);
      //turn off printer motor
      digitalWrite(printerMotor, HIGH);
      delay(1000);
      CauponResetServo.write(55);  
      delay(1150); 
      CauponResetServo.write(5); 
      delay(1150); 
      stockPaper--;
    }
    digitalWrite(printerMotor, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Paper Dispensed!");
    lcd.setCursor(0,1);
    sprintf(lcdBuffer, "Stock: %d left", stockPaper);
    lcd.print(lcdBuffer);
    delay(3000);

    // Celebration jingle (G5 - B5 - E6)
    tone(buzzer, 784, 150);  // G5
    delay(180);
    tone(buzzer, 988, 150);  // B5
    delay(180);
    tone(buzzer, 1318, 300); // E6 (victory high note)
    delay(350);
    noTone(buzzer);
    pulse = 0;
    noCoin = false;
  }    
  else {
    if(buttonStatePrinter == 1 && pulse < 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Not Enough Balance!");
      lcd.setCursor(0,1);
      lcd.print("Need 1 for Paper");
      lcd.setCursor(0,2);
      lcd.print("Please Refill!");
      // // Smooth and friendly melody (G - A - B - C - B - A - G)
      tone(buzzer, 784, 200);   // G5
      delay(250);
      tone(buzzer, 880, 200);   // A5
      delay(250);
      tone(buzzer, 988, 200);   // B5
      delay(250);
      tone(buzzer, 1046, 300);  // C6 (top note)
      delay(350);
      tone(buzzer, 988, 150);   // B5
      delay(200);
      tone(buzzer, 880, 150);   // A5
      delay(200);
      tone(buzzer, 784, 300);   // G5
      delay(350);
      noTone(buzzer); // Stop playing
      lcd.clear();
      pulse = 0;
      noCoin = false;
    }
    if (stockPaper <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Paper");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }

  }



  //FOR PEN PENCIL DISPENSER
  if(buttonStatePencil == 1 && pulse >= 5 && userBalance && stockPencil> 0){
    for(int i = pulse; i >= 5; i -= 5){
      if (stockPencil <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Pencil");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Dispensing Pencil");
      lcd.setCursor(0,1);
      lcd.print(i);
      tone(buzzer, 2700, 500);
      delay(500);
      noTone(buzzer);
      delay(500);
      PencilServo.write(5);  
      delay(1150); 
      PencilServo.write(55); 
      delay(1150); 
      stockPencil --;
    }
    pulse = 0;
    noCoin = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pencil Dispensed!");
    lcd.setCursor(0,1);
    sprintf(lcdBuffer, "Stock: %d left", stockPencil);
    lcd.print(lcdBuffer);
    lcd.setCursor(0,2);
    lcd.print("Thank you!");
    delay(3000);
    lcd.clear();
  }
  else {
      if(buttonStatePencil == 1 && pulse < 5){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Not Enough Balance!");
        lcd.setCursor(0,1);
        lcd.print("Need 5 for Pencil");
        lcd.setCursor(0,2);
        lcd.print("Please Refill!");
        // // Smooth and friendly melody (G - A - B - C - B - A - G)
        tone(buzzer, 784, 200);   // G5
        delay(250);
        tone(buzzer, 880, 200);   // A5
        delay(250);
        tone(buzzer, 988, 200);   // B5
        delay(250);
        tone(buzzer, 1046, 300);  // C6 (top note)
        delay(350);
        tone(buzzer, 988, 150);   // B5
        delay(200);
        tone(buzzer, 880, 150);   // A5
        delay(200);
        tone(buzzer, 784, 300);   // G5
        delay(350);

        noTone(buzzer); // Stop playing
        lcd.clear();

        pulse = 0;
        noCoin = false;
      }
      if (stockPencil <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Pencil");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
  }



  //FOR PEN BLACK DISPENSER
  if(buttonStatePenBlack == 1 && pulse >= 10 && userBalance && stockPenBlack > 0){
    for(int i = pulse; i >= 10; i -= 10){
      if (stockPenBlack <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Pen Black");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Dispensing Pen Black");
      lcd.setCursor(0,1);
      lcd.print(i);
      tone(buzzer, 2700, 500);
      delay(500);
      noTone(buzzer);
      delay(500);
      PenBlackServo.write(5);  
      delay(1150); 
      PenBlackServo.write(55); 
      delay(1150); 
      stockPenBlack --;
    }
    pulse = 0;
    noCoin = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pen Black Dispensed!");
    lcd.setCursor(0,1);
    sprintf(lcdBuffer, "Stock: %d left", stockPenBlack);
    lcd.print(lcdBuffer);
    lcd.setCursor(0,2);
    lcd.print("Thank you!");
    delay(3000);
    lcd.clear();
  }
  else {
      if(buttonStatePenBlack == 1 && pulse < 10){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Not Enough Balance!");
        lcd.setCursor(0,1);
        lcd.print("Need 10 for Pen");
        lcd.setCursor(0,2);
        lcd.print("Please Refill!");
        // // Smooth and friendly melody (G - A - B - C - B - A - G)
        tone(buzzer, 784, 200);   // G5
        delay(250);
        tone(buzzer, 880, 200);   // A5
        delay(250);
        tone(buzzer, 988, 200);   // B5
        delay(250);
        tone(buzzer, 1046, 300);  // C6 (top note)
        delay(350);
        tone(buzzer, 988, 150);   // B5
        delay(200);
        tone(buzzer, 880, 150);   // A5
        delay(200);
        tone(buzzer, 784, 300);   // G5
        delay(350);

        noTone(buzzer); // Stop playing
        lcd.clear();

        pulse = 0;
        noCoin = false;
      }
      if (stockPenBlack <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Pen Black");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
  }



  //FOR PEN BLUE DISPENSER
  if(buttonStatePenRed == 1 && pulse >= 10 && userBalance && stockPenRed > 0){
    for(int i = pulse; i >= 10; i -= 10){
      if (stockPenRed <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Pen Blue");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Dispensing Pen Blue!");
      lcd.setCursor(0,1);
      lcd.print(i);
      tone(buzzer, 2700, 500);
      delay(500);
      noTone(buzzer);
      delay(500);
      PenRedServo.write(5);  
      delay(1150); 
      PenRedServo.write(55); 
      delay(1150); 
      stockPenRed --;
    }
    pulse = 0;
    noCoin = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pen Blue Dispensed!");
    lcd.setCursor(0,1);
    sprintf(lcdBuffer, "Stock: %d left", stockPenRed);
    lcd.print(lcdBuffer);
    lcd.setCursor(0,2);
    lcd.print("Thank you!");
    delay(3000);
    lcd.clear();
  }
  else {
      if(buttonStatePenRed == 1 && pulse < 10){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Not Enough Balance!");
        lcd.setCursor(0,1);
        lcd.print("Need 10 for Pen");
        lcd.setCursor(0,2);
        lcd.print("Please Refill!");
        // // Smooth and friendly melody (G - A - B - C - B - A - G)
        tone(buzzer, 784, 200);   // G5
        delay(250);
        tone(buzzer, 880, 200);   // A5
        delay(250);
        tone(buzzer, 988, 200);   // B5
        delay(250);
        tone(buzzer, 1046, 300);  // C6 (top note)
        delay(350);
        tone(buzzer, 988, 150);   // B5
        delay(200);
        tone(buzzer, 880, 150);   // A5
        delay(200);
        tone(buzzer, 784, 300);   // G5
        delay(350);

        noTone(buzzer); // Stop playing
        lcd.clear();

        pulse = 0;
        noCoin = false;
      }
      if (stockPenRed <= 0) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Item: Pen Blue");
        lcd.setCursor(0,1);
        lcd.print("Out of Stock!");
        lcd.setCursor(0,2);
        lcd.print("Refill first!");
        tone(buzzer, 400, 200);
        delay(200);
        tone(buzzer, 300, 200);
        delay(200);
        noTone(buzzer);
        delay(3000);
        return; 
      }
  }

 if (!noCoin){
    noCoin = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Insert Coins!");
    digitalWrite(coinSlotRGBFrame, HIGH);
    lcd.setCursor(0,0);
    sprintf(lcdBuffer, "Paper    : %d left", stockPaper);
    lcd.print(lcdBuffer);
    lcd.setCursor(0,1);
    sprintf(lcdBuffer, "Black Pen: %d left", stockPenBlack);
    lcd.print(lcdBuffer);
    lcd.setCursor(0,2);
    sprintf(lcdBuffer, "Blue Pen : %d left", stockPenRed);
    lcd.print(lcdBuffer);
    lcd.setCursor(0,3);
    sprintf(lcdBuffer, "Pencil   : %d left", stockPencil);
    lcd.print(lcdBuffer);
  }

  coinSlotStatus =  digitalRead(coinSlot);
  delay(30);
  if(coinSlotStatus == 0){
    digitalWrite(coinSlotRGBFrame, LOW);
    tone(buzzer, 2700, 500);
    userBalance = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Check Balance and");
    lcd.setCursor(0,1);
    lcd.print("Press a Button!");
    pulse += 1;
    sprintf(lcdBuffer, "Bal. Php: %d.00",pulse);
    lcd.setCursor(0,2);
    lcd.print(lcdBuffer);
    delay(30);
  }
  noTone(buzzer);

}