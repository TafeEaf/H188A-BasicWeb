#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
#define ssid      "Classroom"   // WiFi SSID
#define password  "12345678"    // WiFi password
#define LEDPIN    D3            // Led Pin

String  LedStatus = "OFF";
 
// create Objects
ESP8266WebServer server ( 80 );
 
String getPage(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>ESP8266 Demo</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>ESP8266 Demo</h1>";
  page += "<h3>GPIO</h3>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>D3 (Current status: ";
  page += LedStatus;
  page += ")";
  page += "<INPUT type='radio' name='LED' value='1'>ON";
  page += "<INPUT type='radio' name='LED' value='0'>OFF</li></ul>";
  page += "<INPUT type='submit' value='Actualiser'>";
  page += "</body></html>";
  return page;
}

void handleRoot(){ 
  if ( server.hasArg("LED") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}
 
void handleSubmit() {
  // Update GPIO 
  String LEDValue;
  LEDValue = server.arg("LED");
  Serial.println("Set GPIO "); Serial.print(LEDValue);
  if ( LEDValue == "1" ) {
    digitalWrite(LEDPIN, 1);
    LedStatus = "On";
    server.send ( 200, "text/html", getPage() );
  } else if ( LEDValue == "0" ) {
    digitalWrite(LEDPIN, 0);
    LedStatus = "Off";
    server.send ( 200, "text/html", getPage() );
  } else {
    Serial.println("Err Led Value");
  }
}
 
void setup() {
  Serial.begin ( 115200 );

  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  
  // WiFi connexion is OK
  Serial.println ( "" ); 
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );
  
  // link to the function that manage launch page 
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" ); 
}
 
void loop() {
  server.handleClient();
  delay(1000);
}
