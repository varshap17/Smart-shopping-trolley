#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h" 

#define DOUT A0
#define CLK A1
HX711 scale(DOUT, CLK);             //Create HX711 instance
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

LiquidCrystal_I2C lcd(0X27, 16, 2);

int buzzerpin=7;              //Buzzer is connected to digital pin 7 of Arduino UNO
int greenpin=2;               //Green LED is connected to digital pin 2 of Arduino UNO
int redpin=3;                 //Red LED is connected to digital pin 3 of Arduino UNO
int buttonpin=2;              //Push Button is connected to digital pin 5 of Arduino UNO

int a=0;                      //to check if its master card or not
int b1=0;                     //to track inserting or removing of 1st item
int b2=0;                     //to track inserting or removing of 2nd item
int b3=0;                     //to track inserting or removing of 3rd item
int b4=0;                     //to track inserting or removing of 4th item
int total=0;                  //to store total amount of items purpased by the customer.
int balance_amt=10;         //Balance amount in the master card of the customer.
int buttonstate=0;            //variable for reading the pushbutton status

float w;                      //to store weight measured from HX711 load amplifier
float tot_weight=0.0;         //to store total calculated weight of items detected.
float item_weight=0.0;        //to store weight of each item.
float calibration_factor = -96650; 
 
void setup() 
{
    Serial.begin(9600);         //Initiate a serial communication
    SPI.begin();                //Initiate SPI bus
    mfrc522.PCD_Init();         //Initiate MFRC522
    lcd.begin();                //Initiate LCD 
    lcd.backlight();
    lcd.clear();
    lcd.clear();
    lcd.setCursor(0,0);     
    lcd.println("  WELCOME TO  ");
    lcd.setCursor(0,1);
    lcd.println("SMART SHOPPING");
    delay(1000);
    Serial.println("WELCOME TO SMART SHOPPING");
    Serial.println();
    Serial.println("Insert Master Card");
    Serial.println();
    pinMode(buzzerpin,OUTPUT);  //Setup buzzer
    digitalWrite(buzzerpin,LOW);
    pinMode(greenpin,OUTPUT);   //Setup Green LED
    pinMode(redpin,OUTPUT);     //Setup Red LED
    pinMode(buttonpin,INPUT);   //Setup push button
    scale.set_scale(220760);    //Calibration Factor obtained after Calibration
    scale.tare();               //To set the value to zero
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("INSERT MASTER");
    lcd.setCursor(0,1);
    lcd.print("    CARD     ");
}
void loop() 
{
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
        return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
        return;
    } 
    String content= "";                                                        //to store UID od RFID tags
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));    //extract UID from data blocks of RFID tags 
        content.concat(String(mfrc522.uid.uidByte[i], HEX));                   //convert UID to hexadecimal values
    }
    content.toUpperCase();                                                     //convert hexadecimal UID to uppercase
    if (content.substring(1) == "33 64 87 97" && a==0)                         //to check if user checks in with his master card
    { 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Name : Ram");                                               //print customer data
        lcd.setCursor(0,1);
        lcd.print("Balance=");                                                 //print customers balance amount in the master card
        lcd.setCursor(8,1);
        lcd.print(balance_amt);
        Serial.println("Name : Ram");
        Serial.println("Balance=");
        Serial.print(balance_amt);
        Serial.println();
        a=1;                                                                    //to indicate master card is detected 
        delay(3000);
    }
    else if(content.substring(1) == "B3 57 16 11" && a==1 && b1==0)             //to check if item is added
    {
        Serial.println("Box Added");
        Serial.println("Price = Rs.100");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Box added");                                                 //to print details of added item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.100");
        delay(1000);
        item_weight=0.180;
        total=total+100;                                                        //adding item price to total amount
        b1=1;                                                                   //to indicate item is added
        delay(3000);
        tot_weight+=item_weight;                                                //adding item's weight to total weight
    }
    else if(content.substring(1) == "B3 57 16 11" && a==1 && b1==1)             //to check if item is removed
    {
        Serial.println("Box Removed");
        Serial.println("Price = Rs.100");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Box removed");                                              //to print details of removed item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.100");
        delay(1000);
        item_weight=0.180;
        total=total-100;                                                       //subtracting item price from total amount
        b1=0;                                                                  //to indicate item is removed
        delay(3000);
        tot_weight-=item_weight;                                               //subtracting item's weight from total weight
    }
    else if(content.substring(1) == "23 4D 6D 0A" && b2==0 && a==1)            //to check if item is added
    {
        Serial.println("Soap Added");
        Serial.println("Price = Rs.50");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Soap added");                                                //to print details of added item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.50");
        delay(1000);
        item_weight=0.084;
        total=total+50;                                                         //adding item price to total amount
        b2=1;                                                                   //to indicate item is added
        delay(3000);
        tot_weight+=item_weight;                                                //adding item's weight to total weight
    }
    else if(content.substring(1) == "23 4D 6D 0A" && b2==1 && a==1)             //to check if item is removed
    {
        Serial.println("Soap Removed");
        Serial.println("Price = Rs.50");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Soap removed");                                              //to print details of removed item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.50");
        delay(1000);
        item_weight=0.084;
        total=total-50;                                                        //subtracting item price from total amount
        b2=0;                                                                   //to indicate item is removed
        delay(3000);
        tot_weight-=item_weight;                                                //subtracting item's weight from total weight
    }
    else if(content.substring(1) == "63 00 30 15" && a==1 && b3==0)                       //to check if item is added
    {
        Serial.println("Peri Peri Added");
        Serial.println("Price = Rs.150");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Peri Peri added");                                           //to print details of added item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.150");
        delay(1000);
        item_weight=0.130;
        total=total+150;                                                        //adding item price to total amount
        b3=1;                                                                   //to indicate item is added
        delay(3000);
        tot_weight+=item_weight;                                                //adding item's weight to total weight
    }
    else if(content.substring(1) == "63 00 30 15" && a==1 && b3==1)                       //to check if item is removed
    {
        Serial.println("Peri Peri Removed");
        Serial.println("Price = Rs.150");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Peri removed");                                              //to print details of removed item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.150");
        delay(1000);
        item_weight=0.130;
        total=total-150;                                                        //subtracting item price from total amount
        b3=0;                                                                   //to indicate item is removed
        delay(3000);
        tot_weight-=item_weight;                                                //subtracting item's weight from total weight
    }
    else if(content.substring(1) == "03 8F A1 15" && a==1 && b4==0)                       //to check if item is added
    {
        Serial.println("Glue Added");
        Serial.println("Price = Rs.10");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Glue added");                                                 //to print details of added item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.10");
        delay(1000);
        item_weight=0.023;
        total=total+10;                                                        //adding item price to total amount
        b4=1;                                                                   //to indicate item is added
        delay(3000);
        tot_weight+=item_weight;                                                //adding item's weight to total weight
    }
    else if(content.substring(1) == "03 8F A1 15" && a==1 && b4==1)                       //to check if item is removed
    {
        Serial.println("Glue Removed");
        Serial.println("Price = Rs.10");
        Serial.println();
        Serial.println();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Glue removed");                                              //to print details of removed item
        lcd.setCursor(0,1);
        lcd.print("Price=Rs.10");
        delay(1000);
        item_weight=0.023;
        total=total-10;                                                        //subtracting item price from total amount
        b4=0;                                                                   //to indicate item is removed
        delay(3000);
        tot_weight-=item_weight;                                                //subtracting item's weight from total weight
    }
    else                                                                        //if first card detected is not the master card
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Insert master");
        lcd.setCursor(0,1);
        lcd.print("card");
        Serial.println("Insert master card"); 
        Serial.println();                                                       //tell the customer to insert master card
        Serial.println();
        delay(3000);
    }
    w=scale.get_units(),3;                                                      //taking input from HX711 load amplifier
    Serial.println("Weight of cart: ");
    Serial.print(scale.get_units(), 3); //Up to 3 decimal points                //Print weight of the cart
    Serial.println(" kg");
    Serial.println();    
    Serial.println(); 

    if(content.substring(1) != "33 64 87 97")
    {
        if((w<=(tot_weight+0.01)) && (w>=(tot_weight-0.01)))            //to check if calculated weight is equal to measured weight with an error of +/- 10g
        {
            Serial.println("Weight matched");                                       //if weight is matched green LED starts to glow
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Weight matched");
            lcd.setCursor(0,1);
            lcd.print("Total=");
            lcd.setCursor(6,1);
            lcd.print(total);
            Serial.println("Total:");
            Serial.print(total);
            digitalWrite(greenpin,HIGH);
            delay(1000);
            digitalWrite(greenpin,LOW);
            Serial.println();
            Serial.println();    
            Serial.println("Continue");
        }
        else
        {
            Serial.println("Weight not matched");                                   //if weight is not matched red LED starts to glow
            Serial.println();    
            Serial.println();    
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Weight not");
            lcd.setCursor(0,1);
            lcd.print(" matched  ");
            digitalWrite(redpin,HIGH);
            digitalWrite(buzzerpin,HIGH);                                           //buzzer sound indicates malpractice
            delay(1000);
            digitalWrite(buzzerpin,LOW);
            delay(2000);
            digitalWrite(redpin,LOW);
        }
    }
 
    buttonstate=digitalRead(buttonpin);                                         //Read the state of pushbutton 
    if(buttonstate==HIGH)                                                       //Check if push Button is pressed.
    {
        if(total<=balance_amt)                                                  //Check if customer has enough balance in master card
        {
            balance_amt-=total;                                                 //Sutracting the total amount from the balance
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("THANKS FOR");
            lcd.setCursor(0,1);
            lcd.print(" SHOPPING ");
            Serial.println("THANKS FOR SHOPPING");
            Serial.println();    
            Serial.println();    
            delay(3000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Total Bill =");
            lcd.setCursor(0,1);
            lcd.print(total); 
            delay(3000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Balance =");
            lcd.setCursor(0,1);
            lcd.print(balance_amt);                                           //Printing the balance amount for customer's reference
            Serial.println("TOTAL BILL AMOUNT = ");
            Serial.print(total);
            Serial.println();    
            Serial.println();    
            Serial.println("BALANCE AMOUNT = ");
            Serial.print(balance_amt);
            Serial.println();    
            Serial.println();    
        }
        else
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("INSUFFICIENT");
            lcd.setCursor(0,1);
            lcd.print("  BALANCE   ");
            Serial.println("INSUFFICIENT BALANCE");
            Serial.println();    
            Serial.println();   
            digitalWrite(redpin,HIGH);
            digitalWrite(buzzerpin,HIGH);                                           //buzzer sound indicates malpractice
            delay(1000);
            digitalWrite(buzzerpin,LOW);
            delay(2000);
            digitalWrite(redpin,LOW); 
            delay(3000);
        }
    }
    
}
