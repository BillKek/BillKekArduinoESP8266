#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "YOUR-wifi-name"
#define STAPSK  "YOUR-pass-to-wifi"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


//const int led = 13;
const int led = 16; // ESP-12E

static const char handleRootStr[] PROGMEM =  R"EOF(
<!DOCTYPE html><html><body>Choose:<br/><a href="./option1">ESP-12E LED ON</a><br/><a href="./option2">ESP-12E LED OFF</a></body></html>
)EOF";


static const char handleoption1Str[] PROGMEM =  R"EOF(
<!DOCTYPE html><html><body>Choose:<br/><a href="./option1">* ESP-12E LED ON</a><br/><a href="./option2">ESP-12E LED OFF</a></body></html>
)EOF";

static const char handleoption2Str[] PROGMEM =  R"EOF(
<!DOCTYPE html><html><body>Choose:<br/><a href="./option1">ESP-12E LED ON</a><br/><a href="./option2">* ESP-12E LED OFF</a></body></html>
)EOF";

void handleRoot() {
  server.send(200, "text/html", handleRootStr);
}

void handleoption1() {
  digitalWrite(led, 0);//ONN
  server.send(200, "text/html", handleoption1Str);
}

void handleoption2() {
  digitalWrite(led, 1);//OFF
  server.send(200, "text/html", handleoption2Str);
}



void handleNotFound(){
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/option1", handleoption1);

  server.on("/option2", handleoption2);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
}

void loop(void){
  server.handleClient();
}
