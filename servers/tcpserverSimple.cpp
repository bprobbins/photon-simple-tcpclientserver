
//adapted from @Hootie81 & @jon1977 in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);
int serverPort = 6123;
const char replymsg[10] = "TheInMsg";
char clientmsg[10] ="mymsg 2";
uint32_t lastTime;
TCPServer server = TCPServer(serverPort);
TCPClient client;
char inmsg[512];
String myInStr;
char myIpString[24];
char outmsg[50];
int LED = D7;

void out(const char *s) {server.write( (const uint8_t*)s, strlen(s) );  client.stop();}

void in(char *ptr, uint8_t timeout) {
  int pos = 0;
  unsigned long lastdata = millis();
  while ( client.available() || (millis()-lastdata < timeout)) {
    if (client.available()) {
      char c = client.read();
      lastdata = millis();
      ptr[pos] = c;
      pos++;
    }//if (client.available())
    if (pos >= 512 - 1)
    break;
  }//while ( client.available() || (millis()-lastdata < timeout))
  ptr[pos] = '\0';
  while (client.available()) client.read();
  client.flush();
}//void in(char *ptr, uint8_t timeout)

void setup()
{
    WiFi.connect();
    while (!WiFi.ready()) {Particle.process();}
    server.begin(); // begin listening for TCP connections
//    IPAddress myIP = WiFi.localIP();
//    sprintf(myIpString, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
//    Spark.variable("devIP", myIpString, STRING);
    pinMode(LED, OUTPUT);
}//setup()

void loop() {
  if (client.connected())
    {
      in(inmsg,50);
      myInStr =inmsg;
      if (myInStr.indexOf(clientmsg)  >= 0) {
        digitalWrite(D7, 1);          // Flashes the LED
        lastTime = millis();
        while ( millis()-lastTime < 5) { } //50
        digitalWrite(D7, 0);          // Flashes the LED
        out(replymsg);
//THIS WORKS TOO
//                IPAddress myIP = WiFi.localIP();
//                sprintf(outmsg, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
//                out(outmsg);
      }//if (myInStr.indexOf(clientmsg)  >= 0)
//        }//while (client.available())
  }//if (client.connected())
    else
      client = server.available();
}//loop
