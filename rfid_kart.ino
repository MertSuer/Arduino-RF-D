#include <SPI.h> //SPI kütüphanemizi tanımlıyoruz.
#include <MFRC522.h> //MFRC522 kütüphanemizi tanımlıyoruz.
#include <Servo.h> //Servo kütüphanemizi tanımlıyoruz
int RST_PIN = 9; //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 53; //RC522 modülü chip select pinini tanımlıyoruz.
int servoPin = 10; //Servo motor pinini tanımlıyoruz.
int led_r=11;
int led_y=12;
int led_w=13;
Servo motor; //Servo motor için değişken oluşturuyoruz.
MFRC522 rfid(SS_PIN, RST_PIN); //RC522 modülü ayarlarını yapıyoruz.
byte ID[4] = {254,189,241,197}; 
void setup() {
  pinMode(led_r,OUTPUT);
  pinMode(led_y,OUTPUT);
  pinMode(led_w,OUTPUT);
  motor.attach(servoPin); //Servo motor pinini motor değişkeni ile ilişkilendiriyoruz.
  Serial.begin(9600); //Seri haberleşmeyi başlatıyoruz.
  SPI.begin(); //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init(); //RC522 modülünü başlatıyoruz.
}
void loop() {
   if (!rfid.PICC_IsNewCardPresent()) //Yeni kartın okunmasını bekliyoruz.
    return;
  if (!rfid.PICC_ReadCardSerial()) //Kart okunmadığı zaman bekliyoruz.
    return;
  if (rfid.uid.uidByte[0] == ID[0] && //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID[1] && 
    rfid.uid.uidByte[2] == ID[2] &&
    rfid.uid.uidByte[3] == ID[3] ) {
    Serial.println("Kapi acildi");
    ekranaYazdir();
    motor.write(180);//Servo motoru 180 dereceye getiriyoruz.
    digitalWrite(led_y,HIGH);
     digitalWrite(led_r,LOW);
    delay(2000);
    digitalWrite(led_y,LOW);
    digitalWrite(led_w,HIGH);
    Serial.println("Kapınız Kapanıyor");
    delay(1500);
    digitalWrite(led_w,LOW);
    motor.write(0); //Servo motoru 0 dereceye getiriyoruz.
    delay(1000);
    digitalWrite(led_r,HIGH);
  } else { //Yetkisiz girişte içerideki komutlar çalıştırılır.
     digitalWrite(led_r,HIGH);
    Serial.println("Yetkisiz Kart");
    ekranaYazdir();
  }
  rfid.PICC_HaltA();
}
void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}
