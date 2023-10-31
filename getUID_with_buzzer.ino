#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Servo.h>
#include <ezButton.h>

#define RST_PIN 9
#define SS_PIN 10
ezButton button(7);

MFRC522 mfrc522(SS_PIN, RST_PIN);

unsigned long getID(){
  unsigned long hex_num = 0;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    hex_num <<= 8;
    hex_num |= mfrc522.uid.uidByte[i];
  }

  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;

}

#define BUZZER 5
int pos = 0;
Servo myservo;

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50);
  myservo.attach(6);
  unsigned long null = 00000000;
  unsigned long AuthorizedTags[] = {};
  unsigned long ownerTag = 3225510982;
  EEPROM.put(0,ownerTag);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID"));
  pinMode(BUZZER, OUTPUT);
}

void loop() {

  button.loop();

  if(button.isPressed()){
    Serial.println("ADAAAAAM");
  };

  if(button.isReleased()){
    Serial.println("NIET ADAAAAM");
  };

  if(!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  unsigned long uid = getID();
  int i;
  for(i=0;i <= 1023; i = i + 8){
    unsigned long uidlst;
    EEPROM.get(i,uidlst);
  
    if(uid == uidlst){
      Serial.print("UID: "); Serial.print(uid); Serial.println(", Status: Authorized");
      turnServo();
      return;
    }
  }

  Serial.print("UID: "); Serial.print(uid); Serial.println(", Status: Unauthorized");
  for(i=0;i < 5; i = i+1){
    buzz();
  }
  
}

void buzz() {
  tone(BUZZER, 5000); //Set the voltage to high and makes a noise
  delay(200);//Waits for 1000 milliseconds
  noTone(BUZZER);//Sets the voltage to low and makes no noise
  delay(200);//Waits for 1000 milliseconds
}

void turnServo(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}