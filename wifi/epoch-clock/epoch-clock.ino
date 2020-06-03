/*
    refer to the Timelib which has example NTP code
      git@github.com:PaulStoffregen/Time.git

    also see bembu/wemos_oled_clock_ntp_synced.ino
    - https://gist.github.com/bembu/04d324cda49f3b279c4eb901ea2e2ce7

    In Arduino IDE I am setting board to WEMOS D1 mini Lite

  TO-DO
  note that using ESP8266WiFi.h appears to cause an Access Point (Wemos D1) to auto start
  that's not desirable so
  try including more specific dependencies
  like WiFiClient.h

*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>


// I2C setup:
// For Wemos D1 use D4, D3 (marked on board as SDA and SDL)
// contructor(I2C address, columns, rows)
// the displays I have use I2C address 0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#include "wifi-credentials.h"

IPAddress timeServer(5, 189, 146, 13); // from 0.pool.ntp.org

// we could toggle timezone by setting a pin hi/lo
const int timeZone = 0; // 0 for GMT, 1 for BST
time_t t = 0;
#define BLANK "                "

WiFiUDP Udp;
unsigned int localPort = 8888;  // listen for UDP packets

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(D2, INPUT);

  lcd.init();
  lcd.display();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    lcd.print(".");
  }
  show_wifi();

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);

  while (timeStatus() != timeNotSet) {
    delay(2000);
  }
}

byte option = 1;    // display mode: epoch or date etc.
unsigned long last_button_press_millis = 0;
#define DEBOUNCE_MILLIS 800
#define NUMBER_OF_OPTIONS 10

boolean optionChanged() {
  // pin 13 is labelled D11/MOSI/D7
  if (digitalRead(13) and millis() - last_button_press_millis > DEBOUNCE_MILLIS) {
    option = (option + 1) % NUMBER_OF_OPTIONS;
    Serial.println(option);
    last_button_press_millis = millis();
    return true;
  }
  return false;
}

void loop() {
  time_t n = now();

  if (n != t or optionChanged()) {
    t = n;

    switch (option) {
      case 0: update_lcd("It's About Time", day_part(t)); break;
      case 1: show_five_mins(t); break;
      case 2: show_about_time(t); break;
      case 3: show_approx_time(t); break;
      case 4: show_time(t); break;
      case 5: show_date(t); break;
      case 6: showDayOfYear(t); break;
      case 7: show_epoch(t); break;
      case 8: showTimsAge(t); break;
      case 9: show_wifi();
    }
  }
}

void show_wifi() {
  update_lcd(ssid, WiFi.localIP().toString());
}

void show_time(time_t t) {
  update_lcd(timeStr(t), String(day(t)) + " " + monthShortStr(month(t)) + " " + String(year(t)));
}

boolean isLeapYear(int y) {
  if (y % 400  == 0) {
    return true;
  }
  if (y % 100 == 0) {
    return false;
  }
  if (y % 4 == 0) {
    return true;
  }
  return false;
}

int dayOfYear(time_t t) {
  // seconds at the begining of this year
  time_t seconds = (year(t) - 1970) * (SECS_PER_DAY * 365);
  for (int y = 1970; y < year(t); y++) {
    if (isLeapYear(y)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  return ((t - seconds) / SECS_PER_DAY) + 1;
}

void showDayOfYear(time_t t) {
  update_lcd((String)"Day " + dayOfYear(t), (String)"of " + year(t));
}


void showTimsAge(time_t t) {
  // To-do: fix before end of 2018
  int ageInDays = dayOfYear(t) + 360;
  update_lcd(String("20,") + ageInDays + " days", "since 4 April 62");
}

void show_epoch(time_t t) {
  update_lcd("Unix Epoch", epoch(t - timeZone * SECS_PER_HOUR));
}

String ordinal_suffix(byte n) {
  switch (n) {
    case 1:
    case 21:
    case 31:
      return "st";
    case 2:
    case 22:
      return "nd";
    case 3:
    case 23:
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
  "One", "Two", "Three", "Four", "Five", "Six",
  "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve"
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

  switch (minute(t)) {
    case 0:
    case 1:
    case 2:
      update_lcd(hr, "O'clock");
      break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      update_lcd("Just gone", hr + " O'clock");
      break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      update_lcd("Almost Quarter", "past " + hr);
      break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
      update_lcd("Quarter", "past " + hr);
      break;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      update_lcd("Gone Quarter", "past " + hr);
      break;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
      update_lcd("Almost Half", "past " + hr);
      break;
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
      update_lcd("Half past", hr);
      break;
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
      update_lcd("Just gone Half", "past " + hr);
      break;
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
      update_lcd("Almost Quarter", "to " + next_hr);
      break;
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
      update_lcd("Quarter", "to " + next_hr);
      break;
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
      update_lcd("Gone Quarter", "to " + next_hr);
      break;
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
      update_lcd("Almost "  + next_hr, "o'clock");
      break;
    case 58:
    case 59:
      update_lcd(next_hr, "o'clock");
      break;
  }
}


void show_five_mins(time_t t) {
  String hr =  hours[hourFormat12(t) - 1];
  String next_hr =  hours[hourFormat12(t) % 12];

  switch (minute(t)) {
    case 0:
    case 1:
    case 2:
      update_lcd(hr, "O'clock");
      break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      update_lcd("Five past", hr);
      break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      update_lcd("Ten past ", hr);
      break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
      update_lcd("Quarter", "past " + hr);
      break;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      update_lcd("Twenty", "past " + hr);
      break;
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
      update_lcd("Twenty-five", "past " + hr);
      break;
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
      update_lcd("Half past", hr);
      break;
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
      update_lcd("Twenty-five", "to " + next_hr);
      break;
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
      update_lcd("Twenty", "to " + next_hr);
      break;
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
      update_lcd("Quarter", "to " + next_hr);
      break;
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
      update_lcd("Ten to", next_hr);
      break;
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
      update_lcd("Five to", next_hr);
      break;
    case 58:
    case 59:
      update_lcd(next_hr, "O'clock");
      break;
  }
}


void show_about_time(time_t t) {
  String hr =  hours[hourFormat12(t) - 1];
  String next_hr =  hours[hourFormat12(t) % 12];
  byte m = minute(t);

  if (m < 8) {
    update_lcd("About " + hr, "O'clock");
  }
  else if (m < 22) {
    update_lcd("About Quarter", "past " + hr);
  }
  else if (m < 38) {
    update_lcd("About Half", "past " + hr);
  }
  else if (m < 54 ) {
    update_lcd("About Quarter", "to " + next_hr);
  }
  else {
    update_lcd("About " + next_hr, "O'clock");
  }
}


String day_part(time_t t) {
  int h = hour(t);
  String g = "Good ";
  if (h < 5) {
    return g + "Night";
  }
  if (h < 12) {
    return g + "Morning";
  }
  if (h < 18) {
    return g + "Afternoon";
  }
  if (h < 22) {
    return g + "Evening";
  }
  return g + "Night";
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
