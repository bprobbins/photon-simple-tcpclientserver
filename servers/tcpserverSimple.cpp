
//adapted from @Hootie81 & @jon1977 & others in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);
int serverPort = 6123;

const char replymsg[60] = "TheInMsg and then a whole lot more characters than before";
char clientmsg[60] ="mymsg 2 and then a whole lot more characters than before";
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
  }//while ( client.available() || (millis()-lastdata < timeout))
  client.read();
  client.flush();
}//void in(char *ptr, uint8_t timeout)

void setup()
{
    WiFi.connect();
    while (!WiFi.ready()) {Particle.process();}
    server.begin(); // begin listening for TCP connections
    pinMode(LED, OUTPUT);
}//setup()

void loop() {
  if (client.connected())
    {
      in(inmsg,40); //40 pure trial and error and longer than in client
      myInStr =inmsg;
      if (myInStr.indexOf(clientmsg)  >= 0) {
        digitalWrite(D7, 1);          // Flashes the LED
        lastTime = millis();
        while ( millis()-lastTime < 5) { } //50
        digitalWrite(D7, 0);          // Flashes the LED
        out(replymsg);
      }//if (myInStr.indexOf(clientmsg)  >= 0)
  }//if (client.connected())
    else
      client = server.available();
}//loop
