#include <WiFi.h> 
#include <Firebase_ESP_Client.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "addons/TokenHelper.h"  // Pour le callback de token
#include "addons/RTDBHelper.h"  // Pour le debug si nécessaire

// ==== Identifiants WiFi ====
#define WIFI_SSID " "
#define WIFI_PASSWORD " "

// ==== Identifiants Firebase ====
#define API_KEY " "
#define DATABASE_URL " "

// ==== Capteurs & Matériel ====
#define DHTPIN 15
#define DHTTYPE DHT11
#define SOIL_PIN 34
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==== Objets Firebase ====
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connexion WiFi...");

  // ---- Connexion WiFi ----
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n✅ WiFi Connectée !");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connectee");

  // ---- Configuration Firebase ----
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Sign Up Anonyme
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("✅ Sign up anonyme effectué !");
  } else {
    Serial.println("❌ Sign up anonyme échoué !");
    Serial.println(config.signer.signupError.message.c_str());
  }

  // Callback de status du token
  config.token_status_callback = tokenStatusCallback;

  // Initialisation de Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // ---- Initialisation du DHT ----
  dht.begin();

  lcd.clear();
}

void loop() {
  if (Firebase.ready() && (millis() - lastSend > 5000)) {
    lastSend = millis();

    float temperature = dht.readTemperature();
    float humiditeAir = dht.readHumidity();
    int soilRaw = analogRead(SOIL_PIN);
    int humiditeSol = map(soilRaw, 4095, 1500, 0, 100);
    humiditeSol = constrain(humiditeSol, 0, 100);

    float besoinEau = 0.0;
    if (humiditeSol < 40) {
      besoinEau = 1.5;
    } else if (humiditeSol < 60) {
      besoinEau = 1.0;
    } else {
      besoinEau = 0.0;
    }

    // ---- Affichage sur le LCD ----
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print("C H:");
    lcd.print(humiditeAir, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Sol:");
    lcd.print(humiditeSol);
    lcd.print("% E:");
    lcd.print(besoinEau, 1);

    // ---- Envoi à Firebase ----
    if (Firebase.RTDB.setFloat(&fbdo, "/donnees/temperature", temperature)) {
      Serial.println("✅ Température envoyée");
    } else {
      Serial.println("❌ Erreur température: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "/donnees/humidite_air", humiditeAir)) {
      Serial.println("✅ Humidité air envoyée");
    } else {
      Serial.println("❌ Erreur humidité air: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "/donnees/humidite_sol", humiditeSol)) {
      Serial.println("✅ Humidité sol envoyée");
    } else {
      Serial.println("❌ Erreur humidité sol: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "/donnees/besoin_eau", besoinEau)) {
      Serial.println("✅ Besoin eau envoyé");
    } else {
      Serial.println("❌ Erreur besoin eau: " + fbdo.errorReason());
    }
  }
}



