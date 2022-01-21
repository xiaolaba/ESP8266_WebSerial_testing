/*
 * modified, SoftwareSerial dummy read and relay
 * xiaolaba, 2022-JAN-21
 */

#include <SoftwareSerial.h>

#ifndef D5
#if defined(ESP8266)
#define D8 (15)
#define D5 (14)
#define D7 (13)
#define D6 (12)
#define RX (3)
#define TX (1)
#endif
#endif


#define SoftwareSerial_BAUD_RATE 9600
auto& usbSerial = Serial;
SoftwareSerial testSerial;





/*
  WebSerial Demo AP
  ------
  This example code works for both ESP8266 & ESP32 Microcontrollers
  WebSerial is accessible at 192.168.4.1/webserial URL.

  Author: Ayush Sharma
  Checkout WebSerial Pro: https://webserial.pro
*/

 
#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

AsyncWebServer server(80);


const char* ssid = "SoftwareSerial relay to WebSerial AP"; // Your WiFi AP SSID 
const char* password = "123456789"; // Your WiFi Password




/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}

void setup() {
 
    Serial.begin(115200);


    testSerial.begin(SoftwareSerial_BAUD_RATE, SWSERIAL_8N1, D7, D8, false, 95, 11); // my weight scale UART output, 9600 N81
    usbSerial.println(PSTR("\nSoftware serial 9600 BAUD test started"));

    
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: "); Serial.println(IP);
    
    // WebSerial is accessible at "<IP Address>/webserial" in browser
    WebSerial.begin(&server);
    
    /* Attach Message Callback */
    WebSerial.msgCallback(recvMsg);
    
    server.begin();
}


String myString="";
char ch;
    
void loop() {
    while (testSerial.available() > 0) {
      ch = (char)(testSerial.read()); // read Software serial     
      switch (ch)
      {
          case 0x02:
            //usbSerial.println("0x02 ");
            break;
      
          case 0x03:
            //usbSerial.println("0x03 ");
            break;
          
          case 0x0d:
            break;
          
          case 0x0a:
//            usbSerial.println();
            usbSerial.println(myString);   // relay to usb Serial
            WebSerial.println(myString);   // relay to web Serial
            myString = "";
            break;
            
          default:
/*
            usbSerial.print("0x");   // relay to usb Serial
            usbSerial.print(ch, HEX);   // relay to usb Serial
            usbSerial.print(" ");   // relay to usb Serial
*/            
            myString += ch; // assemble the string
            break;
      }
      yield();    
    }
}
