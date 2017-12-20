#include <ESP8266WiFi.h>

// based on http://www.instructables.com/id/Programming-a-HTTP-Server-on-ESP-8266-12E/

WiFiServer server(80); //Initialize the server on Port 80

void setup() {

  WiFi.mode(WIFI_AP);                    // set as an AccessPoint
  WiFi.softAP("Wemos D1", "12345678");  // Provide the (SSID, password); .
  server.begin(); // Start the HTTP Server

  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
  IPAddress HTTPS_ServerIP = WiFi.softAPIP(); // Obtain the IP of the Server
  Serial.print("Server IP is: "); // Print the IP to the monitor window
  Serial.println(HTTPS_ServerIP);

  pinMode(BUILTIN_LED, OUTPUT);   // BUILTIN_LED appears to be 2
  digitalWrite(BUILTIN_LED, LOW); // switch on

}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Looking under the hood
  Serial.println("Somebody has connected :)");

  String request = client.readString();
  Serial.println(request);

  // Handle the Request
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
  delay(1);
  Serial.println("Client disonnected"); //Looking under the hood
}
