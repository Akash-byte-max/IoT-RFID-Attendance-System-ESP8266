#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <LiquidCrystal_I2C.h>

//------------------- Pin Definitions -------------------
#define RST_PIN  D3
#define SS_PIN   D4
#define BUZZER   D8

MFRC522 mfrc522(SS_PIN, RST_PIN);

//------------------- LCD Setup -------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

//------------------- Google Script URL -------------------
String sheet_url = "https://script.google.com/macros/s/AKfycbyagUVBj6SiPu0tnTF613c5GC7-vtvcy576Jk35Jmj8-2TbfXvHaFEJ9JbmN_kbjAD3pw/exec?name=";

//------------------- WiFi Credentials -------------------
#define WIFI_SSID "************"
#define WIFI_PASSWORD "***********"

//------------------- Track Scanned Cards -------------------
String scannedCards[50];
int scannedCount = 0;

//------------------- Student Info -------------------
String getStudentName(String uid) {
  if (uid == "1313D6E4") return "Halaswamy_S_D";
  if (uid == "C7EAD45") return "*************";
  if (uid == "A38BEE4") return "**********";
  if (uid == "73F7C4E4") return "***********";
  if (uid == "5C40AB4") return "Akash_Hallad";

  return "Unknown";
}

String getUSN(String uid) {
  if (uid == "1313D6E4") return "2JH22EC014";
  if (uid == "C7EAD45") return "2JH22EC007";
  if (uid == "A38BEE4") return "2JH22EC060";
  if (uid == "73F7C4E4") return "2JH22EC024";
  if (uid == "5C40AB4") return "2JH22EC002";
  return "NA";
}

//------------------- Helper Functions -------------------
void beepBuzzer(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(duration);
    digitalWrite(BUZZER, LOW);
    delay(duration);
  }
}

String getUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

bool isDuplicate(String uid) {
  for (int i = 0; i < scannedCount; i++) {
    if (scannedCards[i] == uid) return true;
  }
  return false;
}

void addScannedCard(String uid) {
  if (scannedCount < 50) scannedCards[scannedCount++] = uid;
}

//------------------- Setup -------------------
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  delay(1500);

  SPI.begin();
  mfrc522.PCD_Init();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan Your Card");
}

//------------------- Loop -------------------
void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = getUID();
  String name = getStudentName(uid);
  String usn = getUSN(uid);

  Serial.print("Card UID: ");
  Serial.println(uid);

  // Duplicate Check
  if (isDuplicate(uid)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(name);
    lcd.setCursor(0, 1);
    lcd.print("Already Recorded!");
    beepBuzzer(1, 150);
  } else {
    addScannedCard(uid);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome,");
    lcd.setCursor(0, 1);
    lcd.print(name);
    beepBuzzer(2, 200);

    // Send Data to Google Sheet
    if (WiFi.status() == WL_CONNECTED) {
      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
      client->setInsecure();

      String url = sheet_url + name + "&usn=" + usn;
      HTTPClient https;

      if (https.begin(*client, url)) {
        int httpCode = https.GET();
        if (httpCode > 0) {
          Serial.printf("HTTPS Code: %d\n", httpCode);
          lcd.setCursor(0, 0);
          lcd.print("   Attendance   ");
          lcd.setCursor(0, 1);
          lcd.print("   Recorded!    ");
        } else {
          Serial.println("Failed");
          lcd.setCursor(0, 1);
          lcd.print("Send Failed!");
        }
        https.end();
      }
    }
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan Next Card");
}
