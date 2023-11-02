#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Servo.h>
#include <stdint.h>
#include <string.h>

#define RST_PIN 9
#define SS_PIN 10
#define BUZZER 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

unsigned long uid;
unsigned long null = 00000000;
int pos;
Servo myservo;
bool unlocked = false;

void setup() {
  // Empty authorized users and reset count
  /*int g;
  for(g = 8; g < 1024; g += 1){
    EEPROM.write(g,0);
  }
  EEPROM.write(1023,1);*/ 


// Set up connection
  Serial.begin(9600);
  
// Set up servo motor
  myservo.attach(6);
  myservo.write(135);

// Set up storage
  //unsigned long AuthorizedTags[] = {};
  unsigned long ownerTag = 3225510982;
  EEPROM.put(0,ownerTag);
  
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(BUZZER, OUTPUT);
}

void loop() {

  if(Serial.available() > 0){
    String data;
    data = Serial.readString();
    String res = caesar_decrypt(data,1);
    unsigned long hexValue = strtoul(res.c_str(), nullptr, 16);
    buzz();
    addEEPROM(hexValue);
    Serial.end();
    Serial.begin(9600);
    
  }

  if(mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      uid = getID();
      int i;
      for(i=0;i <= 1023; i = i + 8){
        unsigned long uidlst;
        EEPROM.get(i,uidlst);
  
        if(uid == uidlst){
          Serial.print("UID: "); Serial.print(uid); Serial.println(", Status: Authorized");
          if(unlocked){
            lockServo();
            unlocked = false;
          } else {
            openServo();
            unlocked = true;
          }
        return;
        }
      }
      Serial.print("UID: "); Serial.print(uid); Serial.println(", Status: Unauthorized");
      for(i=0;i < 5; i = i+1){
        buzz();
      }    
    } 
  }
}

void buzz() {
  tone(BUZZER, 5000); //Set the voltage to high and makes a noise
  delay(200);//Waits for 1000 milliseconds
  noTone(BUZZER);//Sets the voltage to low and makes no noise
  delay(200);//Waits for 1000 milliseconds
}

void lockServo(){
   // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(135);              // tell servo to go to position in variable 'pos'
    delay(500);                       // waits 15ms for the servo to reach the position
}
void openServo(){                    // waits 15ms for the servo to reach the position
   // goes from 180 degrees to 0 degrees
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(500);                       // waits 15ms for the servo to reach the position
  
}

unsigned long getID(){
  unsigned long hex_num = 0;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    hex_num <<= 8;
    hex_num |= mfrc522.uid.uidByte[i];
  }

  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}

String caesar_decrypt(String text, int shift) {
  String result = "";

  for (char character : text) {
    if (isAlpha(character)) {
      bool isUpper = isUpperCase(character);
      character = toLowerCase(character);
      int charCode = character - 'a';
      charCode = (charCode - shift + 26) % 26 + 'a';

      if (isUpper) {
        character = toupper(charCode);
      } else {
        character = charCode;
      }
    } else if (isDigit(character)) {
      int digit = character - '0';
      digit = (digit + shift) % 10 + '0';
      character = digit;
    }

    result += character;
  }

  return result;
}

void addEEPROM(unsigned long uid){
  int i = isAdded(uid);
  int count = EEPROM.read(1023);
  if (i > 0){ // Remove tag
    
    if (i/8 < count){
      Serial.println('tag removed and other tags moved!');
      int j;
      unsigned long temp;
      for(j = i/8; j < count-1; j += 1){
        EEPROM.get((j+1)*8,temp);
        EEPROM.put(j*8,temp);   
      }
      EEPROM.put((j)*8,null);

      Serial.println(j);
    } else {
      Serial.println('tag removed');
      EEPROM.put(i,null);
    }

    EEPROM.write(1023,count-1);

  } else if (i == -1) { // Add tag
    EEPROM.put(count*8,uid);
    EEPROM.write(1023,count+1);
    Serial.println('tag added!');
  }
  
}

int isAdded(unsigned long uid){
  int count = EEPROM.read(1023);
  int i;
  for(i=0;i < 8*count; i = i + 8){
    unsigned long uidlst;
    EEPROM.get(i,uidlst);

    if(uid == uidlst){
      return i;
    }
  }
  return -1;
}