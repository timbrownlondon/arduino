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
time_t t = 0;
int opt = 0; // set by potentiometer - display epoch or date etc.
#define BLANK "                "

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  lcd.init();
  lcd.display();
  lcd.backlight();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    lcd.print(".");
  }
  show_wifi();

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  delay(2000);
}

int getOpt() {
  int val = analogRead(A0);
  return (val * 5) / 1024;
}

void loop() {
  if (timeStatus() != timeNotSet) {
    time_t n = now();
    int o = getOpt();
    //update the display if time or display choice has changed
    if (n != t or o != opt) {
      t = n;
      opt = o;
      Serial.println(opt);

      switch (opt) {
        case 0:
          show_epoch(t);
          break;
        case 1:
          show_time(t);
          break;
        case 2:
          show_approx_time(t);
          break;
        case 3:
          show_day_part(t);
          break;
        case 4:
          show_date(t);
          break;
        case 5:
          show_wifi();
      }
    }
  }
}

void show_wifi() {
  update_lcd(ssid, WiFi.localIP().toString());
}

void show_time(time_t t) {
  update_lcd(timeStr(t), BLANK);
}

void show_epoch(time_t t) {
  update_lcd("unix epoch", epoch(t - timeZone * SECS_PER_HOUR));
}

String ordinal_suffix(byte n) {
  if (n == 1 or n == 21 or n == 31) {
    return "st";
  }
  if (n == 2 or n == 22 ) {
    return "nd";
  }
  if (n == 3 or n == 23) {
    return "rd";
  }
  return "th";
}

void show_date(time_t t) {
  update_lcd(
    String(dayStr(weekday(t))) + " " + String(day(t)) + ordinal_suffix(day(t)),
    String(monthStr(month(t))) + " " + String(year(t))
  );
}

const String hours[] = {
  "one", "two", "three", "four", "five", "six",
  "seven", "eight", "nine", "ten", "eleven", "twelve"
};

void update_lcd(String lineOne, String lineTwo) {
  lcd.setCursor(0, 0);
  lcd.print(centre(lineOne));
  lcd.setCursor(0, 1);
  lcd.print(centre(lineTwo));
}

void show_approx_time(time_t t) {
  String hr =  hours[hourFormat12(t) - 1];
  String next_hr =  hours[hourFormat12(t) % 12];
  byte m = minute(t);

  if (m < 8) {
    update_lcd("about " + hr, "o'clock");
  }
  else if (m < 22) {
    update_lcd("about a quarter", "past " + hr);
  }
  else if (m < 38) {
    update_lcd("about half", "past " + hr);
  }
  else if (m < 54 ) {
    update_lcd("about a quarter", "to " + next_hr);
  }
  else {
    update_lcd("about " + next_hr, "o'clock");
  }
}

void show_day_part(time_t t) {
  update_lcd(day_part(t), BLANK);
}


String day_part(time_t t) {
  int h = hour(t);
  if (h < 6) {
    return "night";
  }
  if (h < 12) {
    return "morning";
  }
  if (h < 17) {
    return "afternoon";
  }
  if (h < 23) {
    return "evening";
  }
  return "night";
}

String leftPad(String str) {
  while (str.length() < 16) {
    str = " " + str;
  }
  return str;
}

String centre(String str) {
  while (str.length() < 16) {
    str = " " + str + " ";
  }
  return str;
}

String epoch(time_t t) {
  time_t units = t % 1000;
  time_t thousands = (t / 1000) % 1000;
  time_t millions = (t / 1000000) % 1000;
  time_t billions = (t / 1000000000);
  return "  " + String(billions) + "," +
         withLeadingZeros(millions) + "," +
         withLeadingZeros(thousands) + "," +
         withLeadingZeros(units) + " ";
}

String timeStr(time_t t) {
  return withLeadingZero(hour(t)) +
         ":" +
         withLeadingZero(minute(t)) +
         ":" +
         withLeadingZero(second(t));
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

// -------- NTP code ----------

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime() {
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  lcd.setCursor(0, 0);
  lcd.print("send NTP request");

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
  lcd.setCursor(0, 1);
  lcd.print("no NTP response ");
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
