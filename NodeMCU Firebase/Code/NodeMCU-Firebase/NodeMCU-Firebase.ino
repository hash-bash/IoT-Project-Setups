#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "firebasehost"
#define FIREBASE_AUTH "firebaseauthkey"
#define WIFI_SSID "wifissid"
#define WIFI_PASSWORD "wifipassword"

void setup() {
    Serial.begin(9600);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
    Firebase.setFloat("number", 42.0);
    if (Firebase.failed()) {
        Serial.print("Setting /number failed:");
        Serial.println(Firebase.error());
        return;
    }
    delay(1000);

    Firebase.setFloat("number", 43.0);
    if (Firebase.failed()) {
        Serial.print("Setting /number failed:");
        Serial.println(Firebase.error());
        return;
    }
    delay(1000);

    Serial.print("number: ");
    Serial.println(Firebase.getFloat("number"));
    delay(1000);

    Firebase.remove("number");
    delay(1000);

    Firebase.setString("message", "hello world");
    if (Firebase.failed()) {
        Serial.print("Setting /message failed:");
        Serial.println(Firebase.error());
        return;
    }
    delay(1000);

    Firebase.setBool("truth", false);
    if (Firebase.failed()) {
        Serial.print("setting /truth failed:");
        Serial.println(Firebase.error());
        return;
    }
    delay(1000);

    String name = Firebase.pushInt("logs", n++);
    if (Firebase.failed()) {
        Serial.print("Pushing /logs failed:");
        Serial.println(Firebase.error());
        return;
    }
    Serial.print("Pushed: /logs/");
    Serial.println(name);
    delay(1000);
}
