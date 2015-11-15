
//adapted from @Hootie81 & @jon1977 & others in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);

int serverPort = 6123;
uint32_t lastTime;
//const char replymsg[10] = "TheInMsg";
//char clientmsg[10] ="mymsg 2";
const char replymsg[60] = "TheInMsg and then a whole lot more characters than before";
char clientmsg[60] ="mymsg 2 and then a whole lot more characters than before";
char inmsg[512];
String myInStr;
char myIpString[24];
byte server[] = {192, 168, 1, 10};//photon or computer nodejs ip
//byte server[] = {192, 168, 1, 241};//photon or computer nodejs ip
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
  }//while ( client.available() || (millis()-lastdata < timeout))
  client.read();
  client.flush();
}//void in(char *ptr, uint8_t timeout)

void out(const char *s) {client.write( (const uint8_t*)s, strlen(s) );}

void setup()
{
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D3,INPUT);
  while (!WiFi.ready()) {
    Particle.process();
    WiFi.connect();
    while(WiFi.connecting()) {Particle.process();}
  }// while (!WiFi.ready())
}//setup()

void loop() {
    complete = false;
    lastTime = millis();
   while ((!complete) &&  (millis() - lastTime < 10000)){
      if (client.connect( server, serverPort)) {
        if (client.connected()) {
          out(clientmsg);
          lastTime = millis();
           while ((!client.available()) && (millis() - lastTime < 10000)) {Particle.process();}//wait for response
            in(inmsg,10);//10 pure trial and error
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
    }//while (!complete)
  delay(1);
}//loop
