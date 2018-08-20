#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ssid      "WemosWifi"     // WiFi SSID
#define password  "12345678"      // WiFi password
#define GreenLed    D2            // Led Pin

#define  GreenLedOn   "<font color=\"#7fbf7f\"><b>ON</b></font>"
#define  GreenLedOff  "<font color=\"#006600\"><b>OFF</b></font>"

String GreenLedStatus;

// create Objects
ESP8266WebServer server ( 80 );

String getPage(){
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>ESP8266</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Wemos Wifi Basic Server</h1>";
  page += "<h3>Remote GPIO configuration</h3>";
  page += "<form action='/' method='POST'>";
  
  // handle Green LED
  page += "<ul><li>Green Led (Current status: ";
  page += GreenLedStatus;
  page += ")";
  page += "<INPUT type='radio' name='LED1' value='0'>ON";
  page += "<INPUT type='radio' name='LED1' value='1'>OFF</li></ul>";

  // send form
  page += "<INPUT type='submit' value='Send'>";
  page += "</body></html>";
  return page;
}


void handleSubmit() {
  String LED_1_Value = server.arg("LED1");

  // update Green LED
  Serial.print("Set Green Led: ");
  if ( LED_1_Value == "0" ) {
    digitalWrite(GreenLed, 0);
    GreenLedStatus = GreenLedOn;
  } else if ( LED_1_Value == "1" ) {
    digitalWrite(GreenLed, 1);
    GreenLedStatus = GreenLedOff;
  } else {
    Serial.print("No parameter sent, using previous: ");
  }
  Serial.println(!digitalRead(GreenLed));

  server.send ( 200, "text/html", getPage() );
}

void handleRoot(){
  if ( server.hasArg("LED1")) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void setup() {
  pinMode(GreenLed, OUTPUT);            // sets the digital pin D2 as output
  digitalWrite(GreenLed, 1);            // sets the digital pin D2 high
  GreenLedStatus  =   GreenLedOff;

  Serial.begin ( 115200 );

  WiFi.begin ( ssid, password );
  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }

  // WiFi connection is OK
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
