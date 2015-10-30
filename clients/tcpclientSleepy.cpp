
//adapted from @Hootie81 & @jon1977 in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);

int serverPort = 6123;
uint32_t lastTime;
const char replymsg[10] = "TheInMsg";
char clientmsg[10] ="mymsg 2";
char inmsg[512];
String myInStr;
char myIpString[24];

byte server[] = {192, 168, 1, 10};
bool complete;
TCPClient client;

char outmsg[50];

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

void out(const char *s) {client.write( (const uint8_t*)s, strlen(s) );}

void setup()
{
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D3,INPUT);
}//setup()

void loop() {
  WiFi.disconnect();
  System.sleep(D3, RISING, 4);
  if (digitalRead(D3)) {digitalWrite(D6,HIGH);delay(200);digitalWrite(D6,LOW);}

    while (!WiFi.ready()) {
      Particle.process();
      WiFi.connect();
      while(WiFi.connecting()){Particle.process();}
    }// while (!WiFi.ready())
    complete = false;
    lastTime = millis();
    while ((!complete) &&  (millis() - lastTime < 10000)){
      if (client.connect( server, serverPort)) {
        if (client.connected()) {
          out(clientmsg);
          lastTime = millis();
          while ((!client.available()) && (millis() - lastTime < 10000)) {Particle.process();}//wait for response
            in(inmsg,50);
            myInStr =inmsg;
            if (myInStr.indexOf(replymsg)  >= 0) {
              digitalWrite(D7, 1);          // Flashes the LED
              lastTime = millis();
              while ( millis()-lastTime < 5) { } //5
              digitalWrite(D7, 0);          // Flashes the LED
              complete = true;
            }//if (myInStr.indexOf(replymsg)  >= 0)
        }//if (client.connected())
      }//if (client.connect( server, serverPort))
    }//while (!complete) //!!!!!!!!!!!!!!!!
  delay(1);
}//loop
