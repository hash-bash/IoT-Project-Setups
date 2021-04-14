#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;
SoftwareSerial ss(4, 5);
const char *ssid = "wifissid";
const char *password = "wifipassword";
float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str;
int pm;
WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    ss.begin(9600);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    server.begin();
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
}

void loop() {
    while (ss.available() > 0)
        if (gps.encode(ss.read())) {
            if (gps.location.isValid()) {
                latitude = gps.location.lat();
                lat_str = String(latitude, 6);
                longitude = gps.location.lng();
                lng_str = String(longitude, 6);
            }
            if (gps.date.isValid()) {
                date_str = "";
                date = gps.date.day();
                month = gps.date.month();
                year = gps.date.year();
                if (date < 10)
                    date_str = '0';
                date_str += String(date);
                date_str += " / ";

                if (month < 10)
                    date_str += '0';
                date_str += String(month);
                date_str += " / ";
                if (year < 10)
                    date_str += '0';
                date_str += String(year);
            }
            if (gps.time.isValid()) {
                time_str = "";
                hour = gps.time.hour();
                minute = gps.time.minute();
                second = gps.time.second();
                minute = (minute + 30);
                if (minute > 59) {
                    minute = minute - 60;
                    hour = hour + 1;
                }
                hour = (hour + 5);
                if (hour > 23)
                    hour = hour - 24;
                if (hour >= 12)
                    pm = 1;
                else
                    pm = 0;
                hour = hour % 12;
                if (hour < 10)
                    time_str = '0';
                time_str += String(hour);
                time_str += " : ";
                if (minute < 10)
                    time_str += '0';
                time_str += String(minute);
                time_str += " : ";
                if (second < 10)
                    time_str += '0';
                time_str += String(second);
                if (pm == 1)
                    time_str += " PM ";
                else
                    time_str += " AM ";
            }
        }
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS DATA</title> <style>";
    s += "a:link {background-color: YELLOW;text-decoration: none;}";
    s += "table, th, td </style> </head> <body> <h1  style=";
    s += "font-size:300%;";
    s += " ALIGN=CENTER> GPS DATA</h1>";
    s += "<p ALIGN=CENTER style=""font-size:150%;""";
    s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
    s += "width:50%";
    s += "> <tr> <th>Latitude</th>";
    s += "<td ALIGN=CENTER >";
    s += lat_str;
    s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
    s += lng_str;
    s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
    s += date_str;
    s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
    s += time_str;
    s += "</td>  </tr> </table> ";
    s += "</body> </html>";

    client.print(s);
    delay(100);
}
