
//adapted from @Hootie81 & @jon1977 in community.particle.io
#include "application.h"
SYSTEM_MODE(MANUAL);

int serverPort = 6123;
uint32_t lastTime;
char read_char;
char clientmsg = 'x';
const char replymsg = '9';
char inmsg[512];
String myInStr;
char myIpString[24];
byte server[] = {192, 168, 1, 10};//photon or computer nodejs ip
//byte server[] = {XXX,XXX,XXX,XXX};//photon or computer nodejs ip
bool complete;
TCPClient client;

char outmsg[50];

void in(char *ptr, uint8_t timeout) {
        int pos = 0;
        unsigned long lastTime = millis();
        while( client.available()==0 && millis()-lastTime<timeout) {}
        unsigned long lastdata = millis();
        while ( client.available() || (millis()-lastdata < 10)) {
            if (client.available()) {
                char c = client.read();
                lastdata = millis();
                ptr[pos] = c;
                pos++;
            }
            if (pos >= 512 - 1)
            break;
        }
        ptr[pos] = '\0'; //end the char array
        while (client.available()) client.read();
        client.flush();  //for safety
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
    while(WiFi.connecting()){Particle.process();}
  }// while (!WiFi.ready())

}//setup()

void loop() {
    complete = false;
    lastTime = millis();
    while ((!complete) &&  (millis() - lastTime < 10000)){
      if (client.connect( server, serverPort)) {
        if (client.connected()) {
          sprintf(outmsg,"%c",clientmsg);
          out(outmsg);
         lastTime = millis();
         while ((!client.available()) && (millis() - lastTime < 10000)) {Particle.process();}//wait for response
            in(inmsg,10);
            myInStr =inmsg;
            if (myInStr.indexOf(replymsg)  >= 0) {
              digitalWrite(D7, 1);          // Flashes the LED
              lastTime = millis();
              while ( millis()-lastTime < 5) { } //5
              digitalWrite(D7, 0);          // Flashes the LED
              complete = true;
            }//if (myInStr.indexOf(replymsg)  >= 0) {

        }//if (client.connected())
      }//if (client.connect( server, serverPort))
    }//while (!complete) //!!!!!!!!!!!!!!!!
  delay(1);
}//loop
