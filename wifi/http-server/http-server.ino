#include <ESP8266WiFi.h>

// based on http://www.instructables.com/id/Programming-a-HTTP-Server-on-ESP-8266-12E/

WiFiServer server(80); //Initialize the server on Port 80

void setup() {

  WiFi.mode(WIFI_AP);                   // set as an AccessPoint
  WiFi.softAP("Wemos D1", "12345678");  // with SSID, password
  server.begin();                       // start  HTTP server

  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window

  Serial.print("Server IP is: ");
  Serial.println(WiFi.softAPIP());

  pinMode(BUILTIN_LED, OUTPUT);   // BUILTIN_LED is 2 for arduino D1 board
  digitalWrite(BUILTIN_LED, LOW); // switch it on

}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.print("client at ");
  Serial.println(client.remoteIP());
  Serial.println();

  String request = client.readString();
  Serial.println(request);

  // handle request
  if (request.indexOf("/OFF") != -1) {
    digitalWrite(BUILTIN_LED, HIGH);
  }
  else if (request.indexOf("/ON") != -1) {
    digitalWrite(BUILTIN_LED, LOW);
  }

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\n\n";
  s += "<!DOCTYPE HTML>\n";
  s += "<meta name=\"viewport\" content=\"width = device - width, initial - scale = 1.0\">\n";
  s += "<html>\n";
  s += "<br><input type=\"button\" value=\"turn LED on\" onclick=\"location.href='/ON'\">";
  s += "<br><input type=\"button\" value=\"turn LED off\" onclick=\"location.href='/OFF'\">";
  s += "</html>\n";

  client.flush(); //clear previous info in the stream
  client.print(s); // Send the response to the client
}
