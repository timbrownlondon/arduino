/*
    refer to the Timelib which has example NTP code
      git@github.com:PaulStoffregen/Time.git

    also see bembu/wemos_oled_clock_ntp_synced.ino
    - https://gist.github.com/bembu/04d324cda49f3b279c4eb901ea2e2ce7
*/

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// The standard pins for Uno (according to Wire.h, I guess)
// SDA = Analog 4 (serial data)
// SCL = Analog 5 (serial clock)
// For wemos D1 use D4, D3 (marked on board as SDA and SDL)
// contructor(I2C address, columns, rows)
// the displays I have use I2C address 0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#include "wifi-credentials.h"

IPAddress timeServer(5, 189, 146, 13); // from 0.pool.ntp.org

const int timeZone = 1; // 1 hour = BST

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.display();
  lcd.backlight();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    lcd.print(".");
  }
  lcd.setCursor(0, 0);
  lcd.print(WiFi.localIP());

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  delay(2000);
}

time_t prevDisplay = 0;

void loop() {
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      lcd.setCursor(0, 0);
      switch (now() / 10 % 4) {
        case 0:
          lcd.print(dayStr());
          break;
        case 1:
          lcd.print(dateStr());
          break;
        case 2:
          lcd.print(timeStr());
          break;
        case 3:
          lcd.print(WiFi.localIP());
          lcd.print("      ");
      }
      lcd.setCursor(3, 1);
      // now() is adjusted for timeZone - we have to undo that to show the real epoch value
      lcd.print(epochDisplay(now() - timeZone * SECS_PER_HOUR));
    }
  }
}

String leftPad(String str) {
  while (str.length() < 16) {
    str = " " + str;
  }
  return str;
}

String dayStr() {
  return leftPad(String(dayStr(weekday())));
}
String dateStr() {
  return leftPad(String(day()) +
                 " " +
                 String(monthStr(month())) +
                 " " +
                 String(year()));
}

String epochDisplay(time_t epoch) {
  time_t units = epoch % 1000;
  time_t thousands = (epoch / 1000) % 1000;
  time_t millions = (epoch / 1000000) % 1000;
  time_t billions = (epoch / 1000000000);
  return String(billions) + "," + withLeadingZeros(millions) + "," + withLeadingZeros(thousands) + "," + withLeadingZeros(units);
}

String timeStr() {
  return leftPad(withLeadingZero(hour()) +
                 ":" +
                 withLeadingZero(minute()) +
                 ":" +
                 withLeadingZero(second()));
}

// add one zero padding if need be
String withLeadingZero(byte digits) {
  return digits < 10 ?
         "0" + String(digits) :
         String(digits);
}

// add two zeros padding if need be
String withLeadingZeros(time_t digits) {
  if (digits < 10) {
    return "00" + String(digits);
  }
  if (digits < 100) {
    return "0" + String(digits);
  }
  return String(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime() {
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress & address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;            // Stratum, or type of clock
  packetBuffer[2] = 6;            // Polling Interval
  packetBuffer[3] = 0xEC;         // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
